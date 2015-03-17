/*
 * Common linker file for all x86 environments
 */

/* Don't clobber the ld directive */
#undef i386

#if defined(__x86_64__)

OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

#elif defined(__i386__)

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

#else
# error Bad architecture to link with
#endif

ENTRY(_start)

SECTIONS
{
        . = 0x10000;

        .text : {
                *(.text)
        } = 0x9090

        .rodata : {
                *(.rodata)
                *(.rodata.*)
        }

        .note : {
                *(.note)
                *(.note.*)
        }
}