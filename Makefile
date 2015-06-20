CC=avr-gcc
BAUD=57600
CFLAGS=-Os -std=c99 -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DBAUD=${BAUD} -MMD -DUSB_VID=null

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom $< $@

main.elf: main.o debug.o
	avr-gcc -o $@ $(CFLAGS) $(LDFLAGS) $^

program: main.hex
	avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b${BAUD} -Uflash:w:main.hex:i
