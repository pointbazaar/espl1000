main: compile

compile: clean
	sd led_blink.dg ../../stdlib/avr/portb.dg ../../stdlib/avr/timer0.dg

flash:
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:led_blink.hex

clean:
	rm -f *.hex *.obj *.cof *.asm *.png *.dot

