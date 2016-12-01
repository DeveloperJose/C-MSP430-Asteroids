#include <msp430.h>
#include "asteroids.h"
#include "timer.h"

static unsigned int rand_seed = 0;

static unsigned int timer_start_time = 0;
static unsigned int timer_current_time = 0;

static unsigned short total_interrupts = 0;
void wdt_c_handler()
{
  total_interrupts++;
  if(total_interrupts % 3){
    timer_current_time += 1;
  }
  // 15 interrupts / second
  else if (total_interrupts == 15) {
    asteroids_update();
    rand_seed += 3;
    total_interrupts = 0;
  } 
}

void timer_start(){
  timer_start_time = timer_current_time;
  rand_seed += 1;
}

unsigned int timer_elapsed(){
  rand_seed += timer_current_time % 5;
  return (timer_current_time - timer_start_time) / 15;
}



void add_randomness(int val){
  rand_seed += val % 255;
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
