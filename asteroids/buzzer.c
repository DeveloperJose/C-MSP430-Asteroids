#include <msp430.h>
#include "led.h"
#include "buzzer.h"
#include "pitches.h"

static int melody[] = {
  NOTE_C6, NOTE_C7, NOTE_A5, NOTE_A6, 
  0, -1
};

static int tempo[] = {
  6, 6, 6, 6, 6  };

// Default: 650
static unsigned short quarter_duration = 250;
static unsigned short current_note = 0;
static unsigned short duration = 0;

char buzzer_state = 0;
void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR |= BIT6;		/* enable output to speaker (P2.6) */

    duration = (int)(quarter_duration / tempo[0]);
    buzzer_state = STATE_BUZZER_READY;
    current_note = 0;
}

void buzzer_play(){
  // Set the state to playing
  buzzer_state = STATE_BUZZER_PLAYING;

  // Get the current note from the song
  int note = melody[current_note];
  
  // Check if the song has ended
  if(note < 0){
    buzzer_state = STATE_BUZZER_READY;
    current_note = 0;
    return;
  }

  // Turn on the red led when not playing
  if(note == 0){
    led_set(1, 0);
  }
  // Turn on the green led when playing
  else{
    led_set(0, 1);
  }

  // Play the current note
  buzzer_set_period(note);
  // Update the note duration
  duration--;

  // We finished playing the note
  if(duration <= 1){
    // Go to the next note
    current_note++;
    // Update the note duration
    duration = (int)(quarter_duration / tempo[current_note]);
  }  
  
}

void buzzer_set_period(short cycles)
{ 
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}


    
    
  

