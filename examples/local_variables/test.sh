avr-gcc -mmcu=atmega328p -o simple.bin simple.c
avr-objcopy -j .txt -j .data -O ihex simple.bin simple.hex
avr-objdump -d simple.bin
