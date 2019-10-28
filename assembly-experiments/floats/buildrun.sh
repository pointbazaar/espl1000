nasm -f elf floats.asm
ld -m elf_i386 -s -o a.out floats.o
./a.out
