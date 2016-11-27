#include <msp430.h>
#include "led.h"
#include "switches.h"

void led_init()
{
  P1DIR |= LEDS;		// bits attached to leds are output
}

int led_set(int red_state, int green_state){
  // Only allow booleans
  if(red_state < 0 || red_state > 1 || green_state < 0 || red_state > 1)
    return 0;

  char ledFlags = 0;
  ledFlags |= red_state ? LED_RED : 0;
  ledFlags |= green_state ? LED_GREEN : 0;
  
  P1OUT &= (0xff - LEDS) | ledFlags;
  P1OUT |= ledFlags;
}

void led_update(){
  // red, green
  //led_set(!top_s1_state_down, bottom_s1_state_down);
}

