nasm -f elf template.asm
ld -m elf_i386 -s -o template template.o
