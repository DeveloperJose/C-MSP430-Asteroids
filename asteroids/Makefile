# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h -I../h

#switch the compiler (for the internal make rules)
CC              = msp430-gcc
AS              = msp430-as -mmcu=${CPU} -c

all:shapemotion.elf

#additional rules for files
shapemotion.elf: main.o asteroids.o mov_layer.o timer.o str_util.o star.o triangle.o rock.o
	${CC} -mmcu=${CPU}   -o $@ $^ -L../lib -lTimer -lLcd -lShape -lCircle -lp2sw

load: shapemotion.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
