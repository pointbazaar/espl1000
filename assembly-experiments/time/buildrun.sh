nasm -f elf time.asm
ld -m elf_i386 -s -o a.out time.o
./a.out
