main: compile

compile: clean
	sd \
		main.dg \
		../../stdlib/avr/timer0.dg \
		../../stdlib/avr/usart.dg

flash:
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:main.hex

clean:
	rm -f *.hex *.obj *.cof *.asm *.png *.dot

