
![Banner](screen_shot/wingOS.png)

# WingOS (64 bit)
WingOS is a 64 bit os/kernel. 

![sample](screen_shot/sample2_19_10_2020png.png)

It work with the concept of 'service' 
every thing is a service and may be later, replacable with user app, 
it has 4 syscall : 
```
SEND_SERVICE_SYSCALL            = 1        // send a message to a service

READ_SERVICE_SYSCALL            = 2        // read all message that you have

GET_RESPONSE_SERVICE_SYSCALL    = 3        // if the message sended has been responded

GET_PROCESS_GLOBAL_DATA         = 4        // get process global data, if arg1 (target) is nullptr, return self global data, else return a process global data return -1 if there is an error
```
the user app communicate with the kernel threw kernel service : 
    - graphic_buffer_service
    - kernel_process_service
    - memory_serrvice
    - print_service // may be changed
    - ps2_device_service
    - time_service
    ...

the user can create service too, like the graphic_system_service app, in app/graphic_system_service/

it is easy, and fast.

oh and the kernel support smp multithreading <3

and some service are called ORS (On Request Service)

ORS are only active when they receive a message.

for building WingOS go to Build_guide.md
## Implemented things :
 - com
 - gdt
 - idt
 - *pic* / ioapic
 - paging (pmm + vmm)
 - memory (thank lib alloc)
 - smp
 - multiprocessing
 - smp multiprocessing
 - ioapic timer
 - madt 
 - apic 
 - acpi
 - basic ATA driver
 - echfs support
 - programm launcher (only elf64 programm for the moment)
 - really basic pci table parser
 - process message
 - little gui system
 - ps2 mouse driver
 - \[insert something here]
 
 
