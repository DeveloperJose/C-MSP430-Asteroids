#ifndef buzzer_included
#define buzzer_included

#define STATE_BUZZER_READY 0
#define STATE_BUZZER_PLAYING 1

void buzzer_init();
void buzzer_set_period(short cycles);
void buzzer_play();

extern char buzzer_state;

#endif // included
