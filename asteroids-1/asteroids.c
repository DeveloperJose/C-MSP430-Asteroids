#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include "asteroids_shapes.h"
#include "rock.h"
#include "timer.h"
void score_draw();

void gameover_draw();
void gameover_update();

void update_invincibility();
void update_input();
void update_rocks();
void update_warp();

void rotate_left();
void rotate_right();
u_char point_overlap(Vec2* p, Region* r);
u_char region_overlay(Region* r1, Region* r2);

// Boolean
u_char isBulletActive = 0;
int currentShootFrames = 0;
#define SHOOTING_DISTANCE 10

// The number of lives given every timethe game restarts
u_char maxLives = 7;

// The current number of lives
u_char currentLives = 5;

// The current score
int currentScore = 0;

// Is the player currently unable to be hurt? Boolean
u_char isInvincible = 0;

// How much time has actually passed of invincibilty [Current]
int currentInvFrames = 0;

// For blinking while invincible
int currentInvColor = 0;
int invColors[4] = {COLOR_WHITE, COLOR_GRAY, COLOR_WHITE, COLOR_BLACK};
int totalInvColors = 4;

// How much time to give them for invincibility
#define INVINCIBILITY_INTERVAL 500

// Specifies if the screen should be redraw. Boolean
u_char redrawScreen = 1;

// Specifies if the score should be redrawn. Boolean 
unsigned char redrawLives = 1;

// Color of the background
u_int bgColor = COLOR_BLACK;

// Color of the player's ship
unsigned int shipColor = COLOR_RED;

// For warping purposes. Boolean
u_char isWarping = 0;
u_char warpRotation = 10;
int warpCooldown = 0;

// The destination of the warps
Vec2 rightSide = {110, 0};
Vec2 leftSide = {-112, 0};
Vec2 bottomSide = {0, 135};
Vec2 topSide = {0, -130};

// Number of possible rotation directions
#define DIRECTIONS 24
#define D_LENGTH DIRECTIONS / 8

// 0 = Top, 1 = Left, 2 = Bottom, 3 = Right
// Tells you what direction the player is looking at
u_char rotationSide = 3;

// These coordinates are used for rotation
int currentX = D_LENGTH;
int currentY = D_LENGTH;

// Region covering the playing field
Region regionField;

void asteroids_init(){
  layerInit(&layerPlayer);
  layerDraw(&layerPlayer);
  layerGetBounds(&layerField, &regionField);
  timer_start();
}


void asteroids_draw(){
  if(currentLives > 0){
    movLayerDraw(&movLayerPlayer, &layerPlayer);
    score_draw();
  }
  else{
    gameover_draw();
  }
}

void score_draw(){
  if(redrawLives){
    drawString5x7(3, 3, "      ", COLOR_WHITE, COLOR_BLACK);
    redrawLives = 0;
  }
  
  drawString5x7(3, 3, "Lives:", COLOR_WHITE, COLOR_BLACK);
  char buf[3];
  itoa(currentLives, buf, 10);
  drawString5x7(40, 3, buf, COLOR_WHITE, COLOR_BLACK);

  drawString5x7(65, 3, "Score: ", COLOR_WHITE, COLOR_BLACK);
  itoa(currentScore + timer_elapsed(), buf, 10);
  drawString5x7(105, 3, buf, COLOR_WHITE, COLOR_BLACK);
  
}

void gameover_draw(){
  drawString5x7(5, 100, "Gameover (x.x)", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(8, 120, "Continue?", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(11, 150, "(Press any button)", COLOR_WHITE, COLOR_BLACK);
}


void gameover_update(){
  int input = p2sw_read();
  unsigned char top_s1_state_down = (input & 1) ? 0 : 1;
  unsigned char top_s2_state_down = (input & 2) ? 0 : 1;
  unsigned char top_s3_state_down = (input & 4) ? 0 : 1;
  unsigned char top_s4_state_down = (input & 8) ? 0 : 1;

  if(top_s1_state_down || top_s2_state_down || top_s3_state_down || top_s4_state_down){
    currentLives = maxLives;
    //isInvincible = 0;
    //currentInvFrames = 0;
    redrawScreen = 1;
    redrawLives = 1;
    isWarping = 0;
    warpRotation = 10;
    warpCooldown = 0;
    rotationSide = 1;
    currentX = -D_LENGTH;
    currentY = -D_LENGTH;
    isBulletActive = 0;
    currentShootFrames = 0;
    currentScore = currentScore / 100;
    
    asteroids_init();
  }
}


void asteroids_update(){
  // Check if the game is playing
  if(currentLives <= 0){
    gameover_update();
    return;
  }

  // Advance movement layers
  movLayerAdvance(&movLayerPlayer, &regionField);

  // Invincibility management
  update_invincibility();

  // Asteroid management
  update_rocks();
  
  // Read input 
  int input = p2sw_read();
  if(!input) return;
  add_randomness(input);
  redrawScreen = 1;

  // Switch management
  update_input(input);

  // Warp management
  update_warp();

}

void update_invincibility(){
  if(isInvincible){
    // Decrease time
    currentInvFrames += 5;
    add_randomness(currentInvFrames);
    // Select the next blinking color
    currentInvColor++;
    if(currentInvColor > totalInvColors)
      currentInvColor = 0;

    (&layerPlayer)->color = invColors[currentInvColor];

    // If we run out of invincibility...
    if(currentInvFrames > INVINCIBILITY_INTERVAL){
      // Set everything back to normal for next time
      currentInvFrames = 0;
      isInvincible = 0;
      // Set the color back to the original
      (&layerPlayer)->color = shipColor;
    }    
  }
}
  

void update_input(int input){
  unsigned char top_s1_state_down = (input & 1) ? 0 : 1;
  unsigned char top_s2_state_down = (input & 2) ? 0 : 1;
  unsigned char top_s3_state_down = (input & 4) ? 0 : 1;
  unsigned char top_s4_state_down = (input & 8) ? 0 : 1;

  if(isWarping){
    (&movLayerPlayer)->velocity = vec2Zero;
    isWarping = 0;
  }
  
  // Rotation of ship
  if(top_s1_state_down){
    rotate_left();  
  }
  else if(top_s2_state_down){
    rotate_right();
  }

  // Ship heavy machine gun
  // So deadly it can only shoot one at a time 
  if(top_s4_state_down && !isBulletActive){
    isBulletActive = 1;

    // Move the bullet to where the player is
    (&movLayerPlayerGun)->velocity = vec2Zero;
    (&layerPlayerGun)->pos = (&layerPlayer)->pos;
    (&layerPlayerGun)->posNext = (&layerPlayer)->posNext;
    (&layerPlayerGun)->posLast = (&layerPlayer)->posLast;
  }

  if(!isBulletActive){
    // Hide the bullet
    (&movLayerPlayerGun)->velocity = vec2Zero;
    (&movLayerPlayerGun)->layer->color = COLOR_BLACK;
   }
  else{
    Vec2 newPos = {currentX, currentY};
    (&movLayerPlayerGun)->velocity = newPos;
    (&movLayerPlayerGun)->layer->color = COLOR_GREEN;
  
    
    currentShootFrames++;
    if(currentShootFrames >= SHOOTING_DISTANCE){
      currentShootFrames = 0;
      isBulletActive = 0;
      (&movLayerPlayerGun)->velocity = vec2Zero;
      (&movLayerPlayerGun)->layer->color = COLOR_BLACK;
    }
  } 

  // Ship acceleration
  if(top_s3_state_down){
    // Accelerate to the new position
    Vec2 newPos = {currentX, currentY};
    (&movLayerPlayer)->velocity = newPos;
    warpCooldown--;
  }
  else{
    // Stop moving
    (&movLayerPlayer)->velocity = vec2Zero;
  }

  add_randomness(warpCooldown);
  add_randomness(warpRotation);
}

void update_rocks(){
  Vec2 pos = (&layerPlayer)->pos;
  Vec2 bulletPos = (&layerPlayerGun)->pos;
  MovLayer *currentAsteroid = &movLayerAsteroid;
  while(currentAsteroid){
    AbRock* asteroidShape = (AbRock*)(currentAsteroid)->layer->abShape;

    if(asteroidShape->isActive){
      Region asteroidBoundary;
      layerGetBounds((currentAsteroid->layer), &asteroidBoundary);
    
      //if(check_overlap(&playerBoundary, &asteroidBoundary)){
      if(point_overlap(&pos, &asteroidBoundary) && !isInvincible){
	currentLives--;
	isInvincible = 1;
	currentInvFrames = 0;
	redrawLives = 1;
	currentScore -= 100;
	if(currentScore < 0)
	  currentScore = 0;
      }

      else if(point_overlap(&bulletPos, &asteroidBoundary)){
	asteroidShape->isActive = 0;
	currentAsteroid->layer->color = COLOR_BLACK;
	currentAsteroid->velocity = vec2Zero;
	currentScore += 200;
      }

      add_randomness(pos.axes[0] + pos.axes[1]);
    }
    
    currentAsteroid = currentAsteroid->next;
  }
}


void update_warp(){
  Vec2 pos = (&layerPlayer)->pos;
  // Check if they are facing the same direction they were during their last warp. If they are, it's likely that they just finished warping. Don't warp them back.
  if(rotationSide != warpRotation){
    // They touched the left side, warp them right
    if(pos.axes[0] <= PLAYER_SIZE + 5){
      isWarping = 1;
      (&movLayerPlayer)->velocity = rightSide;
      warpRotation = rotationSide;
      warpCooldown = 5;
    }
    // They touched the right side, warp them left
    else if(pos.axes[0] >= (125)){
      isWarping = 1;
      (&movLayerPlayer)->velocity = leftSide;
      warpRotation = rotationSide;
      warpCooldown = 5;
    }
    // Top, warp to the bottom
    else if(pos.axes[1] <= 15){
      isWarping = 1;
      (&movLayerPlayer)->velocity = bottomSide;
      warpRotation = rotationSide;
      warpCooldown = 5;
      
    }
    // Bottom, warp to the top
    else if(pos.axes[1] >= 110){
      isWarping = 1;
      (&movLayerPlayer)->velocity = topSide;
      warpRotation = rotationSide;
      warpCooldown = 5;
    }
  }
  // Since they are facing the same direction we don't want to warp
  // them as we might end up with infinite warps back and forth.
  // Wait for them to cross half of the screen to let them warp again
  // (or for them to change directions)
  else if (warpCooldown <= 0){
    warpRotation = 10;
  }

  add_randomness(warpRotation + warpCooldown);
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
  add_randomness(rotationSide + currentY + currentX);
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
  add_randomness(rotationSide + currentY + currentX);
}

u_char point_overlap(Vec2* p, Region* r){
  if(p->axes[0] < r->topLeft.axes[0] || p->axes[0] > r->botRight.axes[0])
    return 0;

  if(p->axes[1] < r->topLeft.axes[1] || p->axes[1] > r->botRight.axes[1])
    return 0;
  
  return 1;
}

u_char region_overlap(Region* r1, Region* r2){

  return 0;
}
