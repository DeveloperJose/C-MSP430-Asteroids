#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include "asteroids_shapes.h"

void score_draw();
void rotate_left();
void rotate_right();

// Specifies if the screen should be redraw. Boolean
u_char redrawScreen = 1;
// Color of the background
u_int bgColor = COLOR_BLACK;

// For warping purposes. Boolean
u_char isWarping = 0;


// Number of possible rotation directions
#define DIRECTIONS 24
#define D_LENGTH DIRECTIONS / 8
#define GRAVITY 10

// 0 = Top, 1 = Left, 2 = Bottom, 3 = Right
// Tells you what direction the player is looking at
u_char rotationSide = 1;

// These coordinates are used for rotation
int currentX = -D_LENGTH;
int currentY = -D_LENGTH;

// Region covering the playing field
Region regionField;

void asteroids_init(){
  layerInit(&layerPlayer);
  layerDraw(&layerPlayer);

  layerGetBounds(&layerField, &regionField);
}

unsigned char updateLives = 1;
unsigned char updateRot = 1;

void asteroids_draw(){
  movLayerDraw(&movLayerPlayer, &layerPlayer);
  score_draw();
}

void score_draw(){
  if(updateLives){
    drawString5x7(3, 3, "      ", COLOR_WHITE, COLOR_BLACK);
    updateLives = 0;
  }
  
  drawString5x7(3, 3, "Lives:", COLOR_WHITE, COLOR_BLACK);
  char buf[3];
  itoa(rotationSide, buf, 10);
  //drawString5x7(50, 3, "   ", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(50, 3, buf, COLOR_WHITE, COLOR_BLACK);

  char b2[3];
  char b3[3];
  itoa(currentX, b2, 10);
  itoa(currentY, b3, 10);

  drawString5x7(20, 25, b2, COLOR_WHITE, COLOR_BLACK);
  drawString5x7(20, 45, b3, COLOR_WHITE, COLOR_BLACK);
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

  int tempX = currentX;
  int tempY = currentY;

  if(tempX < -2)
    tempX = -2;
  if(tempY < -2)
    tempY = -2;
  if(tempX > 2)
    tempX = 2;
  if(tempY > 2)
    tempY = 2;
  
  Vec2 newPos = {tempX, tempY};
  
  (&movLayerPlayerGun)->velocity = newPos;
  
  // Gun rotation
  if(top_s1_state_down){
    rotate_left();  
  }
  else if(top_s2_state_down){
    rotate_right();
  }
  else{
    (&movLayerPlayerGun)->velocity = vec2Zero;
  }

  // Ship acceleration
  if(top_s3_state_down){
    // Accelerate to the new position
    Vec2 newPos = {currentX, currentY};
    (&movLayerPlayer)->velocity = newPos;

    Vec2 catch;
    vec2Sub(&catch, &layerPlayer.pos,  &layerPlayerGun.pos);
    //vec2Abs(&catch);
    (&movLayerPlayerGun)->velocity = catch;
  }
  else{
    // Stop moving
    (&movLayerPlayer)->velocity = vec2Zero;
  }

  
  if(isWarping){
    (&movLayerPlayer)->velocity = vec2Zero;
    isWarping = 0;
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

void rotate_left(){
  if(rotationSide == 0){
    currentX--;

    if(currentX <= -D_LENGTH){
      currentX = -D_LENGTH;
      rotationSide++;
    }
  }
  else if(rotationSide == 1){
    currentY++;

    if(currentY >= D_LENGTH){
      currentY = D_LENGTH;
      rotationSide++;
    }
  }
  else if(rotationSide == 2){
    currentX++;

    if(currentX >= D_LENGTH){
      currentX = D_LENGTH;
      rotationSide++;
    }
  }
  else if(rotationSide == 3){
    currentY--;

    if(currentY <= -D_LENGTH){
      currentY = -D_LENGTH;
      rotationSide = 0;
    }}
}

void rotate_right(){
  if(rotationSide == 0){
    currentX++;

    if(currentX >= D_LENGTH){
      currentX = D_LENGTH;
      rotationSide = 3;
    }
  }
  else if(rotationSide == 3){
    currentY++;

    if(currentY >= D_LENGTH){
      currentY = D_LENGTH;
      rotationSide--;
    }}
  
  else if(rotationSide == 2){
    currentX--;

    if(currentX <= -D_LENGTH){
      currentX = -D_LENGTH;
      rotationSide--;
    }
  }
  else if(rotationSide == 1){
    currentY--;

    if(currentY <= -D_LENGTH){
      currentY = -D_LENGTH;
      rotationSide--;
    }
  }
}
