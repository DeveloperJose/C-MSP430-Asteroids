#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "mov_layer.h"
#include "game_shapes.h"

static enum {START, PLAYING, GAMEOVER} current_state = START;

unsigned char game_lives = 0;
unsigned int game_score = 0;
unsigned int game_time = 0;
unsigned char current_lane = 0;

// Scoreboard
#define SCORE_HEIGHT 20
unsigned int SCORE_BG_COLOR = COLOR_BLACK;
unsigned int SCORE_FG_COLOR = COLOR_WHITE;

AbRect shapeVLine = {abRectGetBounds, abRectCheck, 1, screenHeight - SCORE_HEIGHT};

// Scoreboard
AbRect shapeScore = {
  abRectGetBounds, abRectCheck, screenWidth, SCORE_HEIGHT}; 
Layer layerScore = {
  (AbShape *) &shapeScore,
  {0, screenHeight - SCORE_HEIGHT},
  {0,0}, {0,0},
  COLOR_BLACK,
  0
};


// Playing field
unsigned int FIELD_BG_COLOR = COLOR_RED;
Region regionField;
AbRect shapeField = {
  abRectGetBounds, abRectCheck,
  {screenWidth, screenHeight - SCORE_HEIGHT}
};
Layer layerField = {
  (AbShape *) &shapeField,
  {0,0},
  {0,0}, {0,0},
  COLOR_RED,
  &layerScore
};


// Lanes
#define LANES 4
#define distance screenWidth / LANES
Layer firstLane = {
  (AbShape *) &shapeVLine,
  {distance, 0},
  {0,0}, {0,0},
  COLOR_BLUE,
  &layerField
};

Layer secondLane = {
  (AbShape *) &shapeVLine,
  {distance*2, 0},
  {0,0}, {0,0},
  COLOR_BLUE,
  &firstLane
};

Layer thirdLane = {
  (AbShape *) &shapeVLine,
  {distance*3, 0},
  {0,0}, {0,0},
  COLOR_BLUE,
  &secondLane
};

Layer fourthLane = {
  (AbShape *) &shapeVLine,
  {distance*4, 0},
  {0,0}, {0,0},
  COLOR_BLUE,
  &thirdLane
};

// Player
AbTriangle shapeTriangle = { abTriangleGetBounds, abTriangleCheck, distance-3};
Layer layerPlayer = {
  (AbShape *) &shapeTriangle,
  {0, 0},
  {0,0}, {0,0},
  COLOR_ORANGE,
  &fourthLane
};
MovLayer movLayerPlayer = {
  &layerPlayer,
  {0,0},
  0
};


// Main drawing region
Region regionMain;
AbRectOutline shapeMain = {
  abRectOutlineGetBounds, abRectOutlineCheck, 0, 0
};
Layer layerMain = {
  (AbShape *) &shapeMain,
  {0,0}, {0,0}, {0,0}, COLOR_YELLOW,
  &layerPlayer
};

MovLayer movLayerMain = {
  &layerMain,
  {0,0},
  &movLayerPlayer
};


char* int_to_string(int num){
  if(num == 0)
    return "0";
  
  int i, rem, len = 0, n;

  n = num;
  while (n != 0)
    {
      len++;
      n /= 10;
    }
  char str[len+1];
  for (i = 0; i < len; i++)
    {
      rem = num % 10;
      num = num / 10;
      str[len - (i + 1)] = rem + '0';
    }
  str[len] = '\0';
  return str;
}


// External buzzer state
char buzzer_state;

int updateScreen = 1;
int updateScore = 1;
int updatePlayer = 1;

void state_update(){
  switch (current_state){
  case START:
  case PLAYING:{
    unsigned int input = p2sw_read();
    if(1 & input){
      current_lane--;
      if(current_lane < 0)
	current_lane = 0;

      updatePlayer = 1;
    }

    else if(8 & input){
      current_lane++;
      if(current_lane >= LANES)
	current_lane = LANES - 1;

      updatePlayer = 1;
    }

    if(updatePlayer){
      unsigned int playerX = current_lane * distance;
      unsigned int playerY = screenHeight - SCORE_HEIGHT - 15;
      Vec2 playerPos = {playerX, playerY};
    
      (&layerPlayer)->pos = playerPos;
      updatePlayer = 0;
      updateScreen = 1;

    }
    
    break;
  }
  case GAMEOVER:
    break;
  }
}

u_int bgColor = COLOR_GREEN;
#define GREEN_LED BIT6

void main(){
  p2sw_init(15);
  configureClocks();
  lcd_init();
  shapeInit();

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  state_update();
  
  layerInit(&layerPlayer);
  layerDraw(&layerPlayer);
  //layerInit(&layerMain);  
  //layerGetBounds(&layerMain, &regionMain);
  
  for(;;) {
    while (!updateScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    updateScreen = 0;
    //layerDraw(&layerMain);
    //layerDraw(&layerPlayer);
    //movLayerDraw(&movLayerMain, &layerMain);
    movLayerDraw(&movLayerPlayer, &layerPlayer);
    unsigned int height = screenHeight - (SCORE_HEIGHT);
      
    drawString5x7(0, height, "Lives", SCORE_FG_COLOR, SCORE_BG_COLOR);
    drawString5x7(45, height, "Score", SCORE_FG_COLOR, SCORE_BG_COLOR);
    drawString5x7(95, height, "Time", SCORE_FG_COLOR, SCORE_BG_COLOR);

    unsigned int height2 = height + (SCORE_HEIGHT / 2);
    drawString5x7(0, height2, int_to_string(game_lives), SCORE_FG_COLOR, SCORE_BG_COLOR);
    drawString5x7(45, height2, int_to_string(game_score), SCORE_FG_COLOR, SCORE_BG_COLOR);
    drawString5x7(95, height2, int_to_string(game_time), SCORE_FG_COLOR, SCORE_BG_COLOR);

  }
}

void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;      /**< Green LED on when cpu on */
  count ++;
  if (count == 17) {
    //mlAdvance(&ml0, &fieldFence);
    //if (p2sw_read())
    //updateScreen = 1;
    state_update();
    count = 0;
  }
  P1OUT &= ~GREEN_LED;    /**< Green LED off when cpu off */
}
