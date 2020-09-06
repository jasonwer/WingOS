#include <arch/64bit.h>
#include <arch/arch.h>
#include <arch/interrupt.h>
#include <arch/pic.h>
#include <arch/process.h>
#include <com.h>
#include <device/apic.h>
#include <device/local_data.h>
#include <device/pit.h>
#include <kernel.h>
#include <utility.h>

const char *exception_messages[] = {"Division By Zero",
                                    "Debug",
                                    "Non Maskable Interrupt",
                                    "Breakpoint",
                                    "Into Detected Overflow",
                                    "Out of Bounds",
                                    "Invalid Opcode",
                                    "No Coprocessor",

                                    "Double Fault",
                                    "Coprocessor Segment Overrun",
                                    "Bad TSS",
                                    "Segment Not Present",
                                    "Stack Fault",
                                    "General Protection Fault",
                                    "Page Fault",
                                    "Unknown Interrupt",

                                    "Coprocessor Fault",
                                    "Alignment Check",
                                    "Machine Check",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",

                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved"};

extern uintptr_t __interrupt_vector[128];
static idt_entry_t idt[IDT_ENTRY_COUNT];

static idtr_t idt_descriptor = {
    .size = sizeof(idt_entry_t) * IDT_ENTRY_COUNT,
    .offset = (uint64_t)&idt[0],
};


uint64_t rip_count[16];
uint32_t rip_counter = 0;

extern "C" void idt_flush(uint64_t);
extern "C" void syscall_asm_entry();

static idt_entry_t register_interrupt_handler(void *handler, uint8_t ist, uint8_t type)
{
    uint64_t p = (uint64_t)handler;
    idt_entry_t idt;
    idt.offset_low16 = (uint16_t)p;
    idt.cs = 0x08;
    idt.ist = ist;
    idt.attributes = type;
    idt.offset_mid16 = (uint16_t)(p >> 16);
    idt.offset_high32 = (uint32_t)(p >> 32);
    idt.zero = 0;

    return idt;
}

void init_idt()
{
    printf("loading idt \n");
    printf("loading idt table \n");
    for (int i = 0; i < 32 + 48; i++)
    {
        idt[i] = register_interrupt_handler((void *)__interrupt_vector[i], 0, 0x8e);
    }

    printf("loading idt idt_flush \n");
    asm volatile("lidt [%0]"
                 :
                 : "m"(idt_descriptor));
    printf("loading idt : OK \n");

    printf("turning on interrupt : OK \n");
};

void dumpregister(InterruptStackFrame *stck)
{
    // this is the least readable code EVER
    printf(" ===== cpu dump ===== \n");
    printf(" ===== cs and ss ===== \n");

    printf("cs = %x | ss = %x \n", stck->cs, stck->ss);
    printf(" ===== utility ===== \n");
    printf("rsp = %x | rbp = %x | rdi = %x \n", stck->rsp, stck->rbp, stck->rdi);
    printf("rsi = %x | rdx = %x | rcx = %x \n", stck->rsi, stck->rdx, stck->rcx);
    printf("rbx = %x | rax = %x |  \n", stck->rbx, stck->rax);
    printf(" ===== other ===== \n");
    printf("error code = %x \n", stck->error_code);
    printf("interrupt number = %x \n", stck->int_no);
    printf("rip = %x \n", stck->rip);
    printf("flags = %x \n", stck->rflags);

    uint64_t CRX;
    asm volatile("mov %%cr2, %0"
                 : "=r"(CRX));
    printf("CR2 = %x \n", CRX);
}

void pic_ack(int intno)
{
    if (intno >= 40)
    {
        outb(PIC2_OFFSET, 0x20);
    }

    outb(PIC1_OFFSET, 0x20);
}

bool is_error(int intno)
{
    if (intno > 31)
    {
        return false;
    }
    // yeah i should do an array
    if (intno == 1 || intno == 15 || (intno >= 21 && intno <= 29) ||
        intno == 31)
    {
        return false;
    }
    return true;
}

void add_rip(uint64_t addr)
{
    rip_count[rip_counter++] = addr;
    if (rip_counter == 15)
    {
        rip_counter = 0;
    }
}

extern "C" void interrupts_handler(InterruptStackFrame *stackframe)
{
    add_rip(stackframe->rip);
    if (is_error(stackframe->int_no))
    {
        for (int i = 0; i < stackframe->int_no * 320; i++)
        {
            is_error(stackframe->int_no);
        }

        printf("error fatal \n");
        printf("id : %x \n", stackframe->int_no);
        printf("type : %s\n", exception_messages[stackframe->int_no]);
        dumpregister(stackframe);

        while (true)
        {
            asm volatile ("hlt");
        }
    }
    if (stackframe->int_no == 32)
    {
        PIT::the()->update();
        irq_0_process_handler(stackframe);
    }
    else if (stackframe->int_no == 0xf0)
    {
        printf("apic : Nmi : possible hardware error :( \n");
        apic::the()->EOI();
    }
    else if (stackframe->int_no == 0xff)
    {
        printf("apic : spurr \n");
        apic::the()->EOI();
    }
    pic_ack(stackframe->int_no);
}