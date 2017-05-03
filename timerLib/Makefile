all: libTimer.a

CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os

#switch the compiler (for the internal make rules)
CC              = msp430-gcc
AS              = msp430-as
AR              = msp430-ar

libTimer.a: clocksTimer.o sr.o
	$(AR) crs $@ $^

install: libTimer.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

clean:
	rm -f timerLib.a *.o

