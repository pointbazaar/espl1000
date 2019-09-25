nasm -f elf functional.asm
ld -m elf_i386 -s -o functional functional.o
