#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"

char bottom_s1_state_down;
char top_s1_state_down;
char top_s2_state_down;
char top_s3_state_down;
char top_s4_state_down;

__interrupt(PORT1_VECTOR) Port_1(){
  if (P1IFG & BOTTOM_SWITCHES) {
    P1IFG &= ~BOTTOM_SWITCHES;
    switch_interrupt_handler();
  }
}

__interrupt(PORT2_VECTOR) Port_2(){
  if (P2IFG & TOP_SWITCHES) {
    P2IFG &= ~TOP_SWITCHES;
    switch_interrupt_handler();
  }
}

static char 
switch_update_interrupt_sense_p1()
{
  char p1val = P1IN;
  /* update switch interrupt sensitivity */
  P1IES |= (p1val & BOTTOM_SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~BOTTOM_SWITCHES);	/* if switch down, sense up */
  return p1val;
}

static char
switch_update_interrupt_sense_p2(){
  char p2val = P2IN;
  /* update switch interrupt sensitivity */
  P2IES |= (p2val & TOP_SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~TOP_SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{ 
  // Set-up bottom switches
  P1REN |= BOTTOM_SWITCHES;		/* enables resistors for switches */
  P1IE |= BOTTOM_SWITCHES;		/* enable interrupts from switches */
  P1OUT |= BOTTOM_SWITCHES;		/* pull-ups for switches */
  P1DIR |= ~BOTTOM_SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense_p1();
  // Set-up top switches
  P2REN |= TOP_SWITCHES;		/* enables resistors for switches */
  P2IE |= TOP_SWITCHES;		/* enable interrupts from switches */
  P2OUT |= TOP_SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~TOP_SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense_p2();

  // Initially read all the switches
  switch_interrupt_handler();
}

void
switch_interrupt_handler()
{
  // Bottom switches
  char p1val = switch_update_interrupt_sense_p1();
  bottom_s1_state_down = (p1val & BOTTOM_S1) ? 0 : 1;

  // Top switches
  char p2val = switch_update_interrupt_sense_p2();
  top_s1_state_down = (p2val & TOP_S1) ? 0 : 1;
  top_s2_state_down = (p2val & TOP_S2) ? 0 : 1;
  top_s3_state_down = (p2val & TOP_S3) ? 0 : 1;
  top_s4_state_down = (p2val & TOP_S4) ? 0 : 1;
}
