#include <msp430.h>
#include <libTimer.h>
#include <shape.h>
#include "asteroids.h"

#define GREEN_LED BIT6
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */	       
  P1OUT |= GREEN_LED;

  configureClocks();
  shapeInit();
  lcd_init();
  p2sw_init(15);
  asteroids_init();

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;

    asteroids_draw();
  }
}
