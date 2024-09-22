main: compile

compile:
	sd led_blink_no_timer.dg

flash:
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:led_blink_no_timer.hex
clean:
	rm -f *.hex *.obj *.cof *.asm *.png *.dot

