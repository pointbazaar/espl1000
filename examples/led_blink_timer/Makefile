main: compile

compile: clean
	sd -stdlib led_blink.dg

flash:
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:led_blink.hex

clean:
	rm -f *.hex *.obj *.cof *.asm *.png *.dot

