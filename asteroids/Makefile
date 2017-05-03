# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS			= -L../lib -L/opt/ti/msp430_gcc/include/
#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as -mmcu=${CPU} -c

all:shapemotion.elf

#additional rules for files
shapemotion.elf: main.o asteroids.o mov_layer.o timer.o str_util.o star.o triangle.o rock.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape -lp2sw

load: shapemotion.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
