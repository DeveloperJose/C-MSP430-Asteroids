#include <msp430.h>
#include "asteroids.h"

void wdt_c_handler()
{
  static short count = 0;
  count ++;
  // 15 interrupts / second
  if (count == 15) {
    asteroids_update();
    count = 0;
  } 
}
