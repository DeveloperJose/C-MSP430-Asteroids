# makfile configuration
SRC			= game.o timer.o switches.o buzzer.o led.o triangle.o mov_layer.o
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h -I../h

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: project.elf

#additional rules for files
project.elf: ${SRC} 
	${CC} -mmcu=${CPU}   -o $@ $^ -L../lib -lTimer -lLcd -lShape -lCircle -lp2sw

load: project.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
