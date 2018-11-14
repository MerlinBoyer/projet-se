F_CPU=16000000
COMPIL_OPT=-g -mmcu=atmega328p
PORT=/dev/ttyACM0

ALL=main
DEP= main.o bluetooth.o time.o

all: $(ALL)

main: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex	

%.elf: $(DEP)
	avr-gcc $(COMPIL_OPT) -o $@ $^

%.o: %.c
	avr-gcc $(COMPIL_OPT) -Os -c -DF_CPU=$(F_CPU) -o $@ $< 

clean:
	rm -rf *.o
	rm -rf *.elf

install: 
	avrdude -p atmega328p -P $(PORT) -c arduino -U flash:w:main.hex:i

mrproper: clean
	rm -rf test 
