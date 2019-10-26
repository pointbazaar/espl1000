nasm -f elf floats.asm
ld -m elf_i386 -s -o floats floats.o
