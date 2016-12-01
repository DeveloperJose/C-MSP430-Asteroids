#ifndef switches_included
#define switches_included

#define BOTTOM_S1 BIT3 // p1.3
#define BOTTOM_SWITCHES BOTTOM_S1

#define TOP_S1 BIT0 // p2.0
#define TOP_S2 BIT1 // p2.1
#define TOP_S3 BIT2 // p2.2
#define TOP_S4 BIT3 // p2.3
#define TOP_SWITCHES (TOP_S1 | TOP_S2 | TOP_S3 | TOP_S4)

void switch_init();
void switch_interrupt_handler();

extern char bottom_s1_state_down;
extern char top_s1_state_down;
extern char top_s2_state_down;
extern char top_s3_state_down;
extern char top_s4_state_down;

#endif // included
