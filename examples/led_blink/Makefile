help:
	@echo "make timer, make notimer, make clean"

timer: clean
	sd -debug led_blink.dg
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:led_blink.hex

notimer:
	sd -debug led_blink_no_timer.dg
	sudo avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:led_blink_no_timer.hex
clean:
	rm -f *.hex *.obj *.cof *.asm

