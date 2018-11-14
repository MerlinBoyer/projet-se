CXXFLAGS=-Wall -Wextra

.PHONY: all 
all: main

main: 
	avr-gcc -g -Os -mmcu=atmega328p main.c -o main.elf
	avr-objcopy -O binary main.elf main.bin
	avrdude -c arduino -p m328p -U main.bin -b 115200 jtagmkI -P /dev/ttyACM0 -v

.PHONY: clean
clean:
	rm *.elf *.bin

