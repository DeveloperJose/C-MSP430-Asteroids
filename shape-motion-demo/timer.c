#include <msp430.h>
#include "timer.h"
#include "mov_layer.h"
#include "game.h"

static unsigned int rand_seed = 0;

static unsigned short timer_start_time = 0;
static unsigned short timer_current_time = 0;

static unsigned short timer_state_time = 0;
static unsigned short timer_state_transition = 0;

void timer_start(){
  timer_start_time = timer_current_time;
}

unsigned short timer_elapsed(){
  rand_seed += timer_current_time % 5;
  return timer_current_time - timer_start_time;
}

/*__interrupt(WDT_VECTOR) WDT(){
  timer_state_time++;
  timer_current_time++;
  rand_seed++;

  led_set(0,0);
  if(timer_state_time >= timer_state_transition){
    state_update();
    led_set(1, 1);
    timer_state_time = 0;
    rand_seed += 5;
  }

}
*/

void timer_set_state_transition(unsigned short delay){
  timer_state_transition = delay;
}

/**
 * Pseudo-random number generator.
 *
 * From: https://raw.githubusercontent.com/0/msp430-rng/master/
 * Implemented by a 16-bit linear congruential generator.
 * NOTE: Only treat the MSB of the return value as random.
 *
 * @param state Previous state of the generator.
 * @return Next state of the generator.
 */

unsigned int prand() {
  return M * rand_seed + I;                     // Generate the next state of the LCG
}
