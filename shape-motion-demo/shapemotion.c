/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "mov_layer.h"
#include "game_shapes.h"
#include "switches.h"

#define GREEN_LED BIT6
#define LANES 4

int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */


int redrawScore = 1;

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */ 
  P1OUT |= GREEN_LED;

  switch_init();
  
  configureClocks();
  lcd_init();
  shapeInit();

  layerInit(&layerMain);
  layerGetBounds(&layerMain, &regionMain); 
  layerDraw(&layerMain);


  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x18);	              /**< GIE (enable interrupts) */

  for(;;) {
    
    // while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
    //P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
    //or_sr(0x10);	      /**< CPU OFF */
    //}
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;

    //movLayerDraw(&ml0, &layer0);

    layerDraw(&layerMain);
    
    // Text for the scoreboard
    if(redrawScore){
      redrawScore = 1;
    }
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 0) {
    }
    //movLayerAdvance(&ml0, &fieldFence);
    //if (p2sw_read())
    //redrawScreen = 1;
    redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
