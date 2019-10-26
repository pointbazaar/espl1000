nasm -f elf puti.asm
ld -m elf_i386 -s -o a.out puti.o
./a.out
