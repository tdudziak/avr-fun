CC=avr-gcc
BAUD=57600
ATOM_PATH=../atomthreads/

CFLAGS=-Os -std=c99 -I$(ATOM_PATH)/kernel/ -I$(ATOM_PATH)/ports/avr -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DBAUD=$(BAUD) -MMD -DUSB_VID=null

KERNEL_OBJECTS = atomkernel.o atomsem.o atommutex.o atomtimer.o atomqueue.o
PORT_OBJECTS = atomport.o atomport-asm.o
APP_OBJECTS = main.o debug.o

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom $< $@

main.elf: $(APP_OBJECTS) $(KERNEL_OBJECTS) $(PORT_OBJECTS)
	avr-gcc -o $@ $(CFLAGS) $(LDFLAGS) $^

$(KERNEL_OBJECTS): %.o: $(ATOM_PATH)/kernel/%.c
	$(CC) -c $(CFLAGS) $< -o $(notdir $@)

atomport.o: $(ATOM_PATH)/ports/avr/atomport.c
	$(CC) -c $(CFLAGS) $< -o $(notdir $@)

atomport-asm.o: $(ATOM_PATH)/ports/avr/atomport-asm.s
	$(CC) -x assembler-with-cpp -c $(CFLAGS) $< -o $(notdir $@)

$(APP_OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

program: main.hex
	avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b$(BAUD) -Uflash:w:main.hex:i
