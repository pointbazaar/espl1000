main: compile

compile: clean
	sd test_portb.dg ../../stdlib/avr/portb.dg

flash:
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:test_portb.hex

clean:
	rm -f *.hex *.obj *.cof *.asm *.png *.dot

