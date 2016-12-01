#include <msp430.h>
#include <libTimer.h>
#include <shape.h>
#include "asteroids.h"
#include "led.h"
#include "buzzer.h"

void main()
{
  configureClocks();
  led_init();
  buzzer_init();
  shapeInit();
  lcd_init();
  p2sw_init(15);
  asteroids_init();

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      or_sr(0x10);	      /**< CPU OFF */
    }
    redrawScreen = 0;
    asteroids_draw();
  }
}
