// This code is intended to be a bootloader and should be compiled
// as a flat binary and placed at the beginning of a bootable device.

// Memory address where the bootloader will be loaded
#define BOOTLOADER_ADDRESS 0x7C00

void print_string(const char *str) {
    while (*str != '\0') {
        asm volatile (
            "mov $0x0E, %%ah\n\t" // ah set up to the display character function
            "mov %0, %%al\n\t"   //  al code of charater to display
            "int $0x10"         // BIOS Interrupt call 
            :
            : "r" (*str)         // operands should be placed in general-purpose register 
        );
        str++;
    }
}

void bootloader_main() {
    // Set up the stack
    asm volatile (
        "mov $0x9000, %ax\n\t" // setting up the data as ax to 0x9000
        "mov %ax, %ss\n\t"  // copying the data from the ax register to Stack segment which is 0x9000
        "mov $0xFFFF, %sp" // Setting up the stack pointer to top , so it can be increase downward 
    );
    // stack up is over now to print 

    print_string("Aryan Bootloader"); // value to be print or can be call any other function also 

    asm volatile ("1: jmp 1b");  // 1: as target and 1b as the jump so it is in a infinite loop
}

void _start() {
    // Call the bootloader main function
    bootloader_main();

    // Fill the rest of the 512-byte sector with zeros
    asm volatile (
        "xor %ax, %ax\n\t" // seting up the ax to zero
        "mov %ax, %ds\n\t"  // ds to zero 
        "mov %ax, %es\n\t"  // es to zero
        "mov %ax, %si\n\t"  // si to zero 
        "mov $510, %cx\n\t"  // set count to 510 so do for next 510 times more
        "rep stosw"    // repeat till cx times 
    );

    asm volatile ("mov $0xAA55, %ax\n\tint $0x10");  //seting up the ax value to 0xAA55 , as a sign of valid boot 

    asm volatile ("1: jmp 1b"); // 1: as target and 1b as the jump so it is in a infinite loop
}