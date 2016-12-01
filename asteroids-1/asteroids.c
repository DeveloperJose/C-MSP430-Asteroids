#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>

#include "asteroids_shapes.h"

u_char redrawScreen = 1;
u_int bgColor = COLOR_BLACK;

// Number of possible rotation directions
#define DIRECTIONS 32
#define D_LENGTH DIRECTIONS / 8
#define GRAVITY 10

unsigned char warping = 0;
unsigned int acceleration = 0;
unsigned char side = 0;
Vec2 warpPos;

// These coordinates are used for rotation
int currentX = 0;
int currentY = -D_LENGTH;

// Region covering the playing field
Region regionField;

void asteroids_init(){
  layerInit(&layerPlayer);
  layerDraw(&layerPlayer);

  layerGetBounds(&layerField, &regionField);
}

void score_draw(){
    drawString5x7(3, 3, "Lives:", COLOR_WHITE, COLOR_BLACK);
    //itoa(angle, buf, 10);
    //drawString5x7(50, 3, "   ", COLOR_WHITE, COLOR_BLACK);
    //drawString5x7(50, 3, buf, COLOR_WHITE, COLOR_BLACK);
}

void asteroids_draw(){
  score_draw();
  movLayerDraw(&movLayerPlayer, &layerPlayer);
}

void asteroids_update(){
  movLayerAdvance(&movLayerPlayer, &regionField);
  int input = p2sw_read();

  if(!input) return;

  redrawScreen = 1;
  
  unsigned char top_s1_state_down = (input & 1) ? 0 : 1;
  unsigned char top_s2_state_down = (input & 2) ? 0 : 1;
  unsigned char top_s3_state_down = (input & 4) ? 0 : 1;
  unsigned char top_s4_state_down = (input & 8) ? 0 : 1;
    
  if(top_s1_state_down){
    //(&ml1)->velocity = leftVel;
    //drawString5x7(0,0, "S4", COLOR_WHITE, COLOR_BLACK);

    // Top
    if(side == 0){
      currentX--;

      if(currentX < -D_LENGTH){
	currentX = -D_LENGTH;
	side = 1;
      }
    }

    // Left
    if(side == 1){
      currentY++;

      if(currentY > D_LENGTH){
	currentY = D_LENGTH;
	side = 2;
      }
    }

    // Bottom
    if(side == 2){
      currentX++;

      if(currentX > D_LENGTH){
	currentX = D_LENGTH;
	side = 3;
      }
    }

    // Right
    if(side == 3){
      currentY--;

      if(currentY < D_LENGTH){
	currentY = D_LENGTH;
	side = 0;
      }
    }

	
  }
  else if(top_s2_state_down){
	
  }
  else if(top_s3_state_down){
    Vec2 newPos = {currentX, currentY};
    (&movLayerPlayer)->velocity = newPos;
  }
  else{
    (&movLayerPlayer)->velocity = vec2Zero;
  }

  if(warping){
    (&movLayerPlayer)->velocity = vec2Zero;
    warping = 0;
  }
  // They touched the left side, warp them right

  /*else if((&ml1)->layer->pos.axes[0] <= PLAYER_SIZE - 5){
    warping = 1;
    Vec2 pos = (&ml1)->layer->pos;
    Vec2 rightSide = {-screenWidth + PLAYER_SIZE + 20, 0};
    (&ml1)->velocity = rightSide;
	
    }
    else if((&ml1)->layer->pos.axes[0] >= PLAYER_SIZE + 5){
    warping = 1;
    Vec2 pos = (&ml1)->layer->pos;
    Vec2 leftSide = {(screenWidth - PLAYER_SIZE - 19), 0};
    (&ml1)->velocity = leftSide;
      	
    }*/
}
