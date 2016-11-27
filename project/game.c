#include <msp430.h>
#include "timer.h"
#include "game.h"
#include "buzzer.h"
#include "led.h"
#include "switches.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"
#include "mov_layer.h"

static enum {START, PLAYING, GAMEOVER} current_state = START;

// External buzzer state
char buzzer_state;

void state_update(){
  switch (current_state){
  case START:
    break;
  case PLAYING:
    break;
  case GAMEOVER:
    break;
  }
}

int redrawScreen = 1;

void main(void) 
{
  switch_init();
  
  configureClocks();
  buzzer_init();
  
  led_init();
  led_set(1, 1);

  lcd_init();
  shapeInit();
  
  enableWDTInterrupts();
  
  or_sr(0x8);  // CPU off, GIE on

  while(1){
    while(!redrawScreen){
      or_sr(0x10);
    }
    redrawScreen = 0;
    movLayerDraw(&m10, &layer0);
  }
  redrawScreen = 0; 
}
