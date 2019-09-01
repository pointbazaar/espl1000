nasm -f elf main.asm
ld -melf_i386 -s -o main main.o
