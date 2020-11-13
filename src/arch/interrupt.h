#pragma once
#include <arch/arch.h>
#include <int_value.h>

#define IDT_ENTRY_COUNT 256
#define INTGATE 0x8e
#define TRAPGATE 0xeF

struct idt_entry
{
    uint16_t offset_low16;
    uint16_t cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_mid16;
    uint32_t offset_high32;
    uint32_t zero;
} __attribute__((packed));
struct idtr
{
    uint16_t size;   // size of the IDT
    uint64_t offset; // address of the IDT
} __attribute__((packed));

void pic_ack(int intno);
void init_idt(void);
