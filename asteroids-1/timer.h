#ifndef include_timer
#define include_timer

/* LCG constants */
#define M 49381 // Multiplier
#define I 8643 // Increment

void timer_start();
int timer_elapsed();
unsigned int prand();

void add_randomness(int value);
#endif
