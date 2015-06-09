CC=avr-gcc
CFLAGS=-Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom $< $@

main.elf: main.o
	avr-gcc -o $@ $(CFLAGS) $(LDFLAGS) $<

program: main.hex
	avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:main.hex:i
