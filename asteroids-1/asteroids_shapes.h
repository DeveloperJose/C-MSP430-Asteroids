#ifndef include_game_draw
#define include_game_draw

#include <shape.h>
#include <abCircle.h>
#include "mov_layer.h"
#include "star.h"
#include "triangle.h"
#include "rock.h"

#define PLAYER_SIZE 10

// Invisible playing field
AbRectOutline shapeField = {
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {(screenWidth/2)-1, (screenHeight/2)-1}
};
Layer layerField = {
  (AbShape *) &shapeField,
  {screenWidth/2, screenHeight/2}, {0,0}, {0,0},
  COLOR_BLACK,
  0
};

// Starry background
AbStar shapeStar = {
  abStarGetBounds, abStarCheck, {screenWidth-2, screenHeight-2}
};
Layer layerStar = {
  (AbShape *)&shapeStar,
  {2, 2}, {0,0}, {0,0}, WHITE, &layerField
};

// Asteroids
AbRock asteroid1 = { abRockGetBounds, abRockCheck, {6, 7}, 1};
AbRock asteroid2 = { abRockGetBounds, abRockCheck, {8, 7}, 1};

// Asteroid 5
Layer layerAsteroid5 = {
  (AbShape *)&asteroid1,
  {(screenWidth/2)+30, (screenHeight/2)+15}, {0,0}, {0,0}, COLOR_WHITE,
  &layerStar,
};
MovLayer movLayerAsteroid5 = {&layerAsteroid5, {3, 1}, 0}; 

// Asteroid 4
Layer layerAsteroid4 = {
  (AbShape *)&asteroid1,
  {45, 30}, {0,0}, {0,0}, WHITE,
  &layerAsteroid5
};
MovLayer movLayerAsteroid4 = {&layerAsteroid4, {4, 3}, &movLayerAsteroid5}; 


// Asteroid 3
Layer layerAsteroid3 = {
  (AbShape *)&asteroid1,
  {20, 50}, {0,0}, {0,0}, WHITE,
  &layerAsteroid4
};
MovLayer movLayerAsteroid3 = {&layerAsteroid3, {3, 2}, &movLayerAsteroid4}; 


// Asteroid 2
Layer layerAsteroid2 = {
  (AbShape *)&asteroid2,
  {0, 0}, {0,0}, {0,0}, WHITE,
  &layerAsteroid3
};
MovLayer movLayerAsteroid2 = {&layerAsteroid2, {1, 3}, &movLayerAsteroid3}; 

// Asteroid 1
Layer layerAsteroid1 = {
  (AbShape *)&asteroid1,
  {(screenWidth/2)+10, (screenHeight/2)+5}, {0,0}, {0,0}, COLOR_WHITE,
  &layerStar,
};
MovLayer movLayerAsteroid = { &layerAsteroid1, {2, 1}, 0};

// Player Gun
AbRect shapePlayerGun = {
  abRectGetBounds, abRectCheck, {2,2}
};
Layer layerPlayerGun = {
  (AbShape *)&shapePlayerGun,
  {screenWidth/2, screenHeight/2 - 2}, {0,0}, {0,0}, COLOR_VIOLET,
  &layerAsteroid1
};
MovLayer movLayerPlayerGun = { &layerPlayerGun, {0,0}, &movLayerAsteroid};

// Player
AbTriangle playerShape = {
  abTriangleGetBounds, abTriangleCheck, PLAYER_SIZE
  };
Layer layerPlayer = {
  (AbShape *)&playerShape,
  {screenWidth/2, screenHeight/2}, {0,0}, {0,0}, COLOR_RED,
  &layerPlayerGun,
};
MovLayer movLayerPlayer = { &layerPlayer, {0,0}, &movLayerPlayerGun };


#endif
