#ifndef include_game_draw
#define include_game_draw

#include <shape.h>
#include <abCircle.h>
#include "mov_layer.h"
#include "star.h"
#include "triangle.h"

#define PLAYER_SIZE 10

// Invisible playing field
AbRectOutline shapeField = {
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {(screenWidth/2)-1, (screenHeight/2)-1}
};
Layer layerField = {
  (AbShape *) &shapeField,
  {screenWidth/2, screenHeight/2}, {0,0}, {0,0},
  COLOR_RED,
  0
};

// Starry background
AbStar shapeStar = {
  abStarGetBounds, abStarCheck, {screenWidth, screenHeight}
};
Layer layerStar = {
  (AbShape *)&shapeStar,
  {0, 0}, {0,0}, {0,0}, WHITE, &layerField
};

// Asteroids
Layer layerCircle = {
  (AbShape *)&circle10,
  {(screenWidth/2)+10, (screenHeight/2)+5}, {0,0}, {0,0}, COLOR_VIOLET,
  &layerStar,
};
MovLayer movLayerCircle = { &layerCircle, {2, 1}, 0};

// Player Gun
AbRect shapePlayerGun = {
  abRectGetBounds, abRectCheck, {2,2}
};
Layer layerPlayerGun = {
  (AbShape *)&shapePlayerGun,
  {screenWidth/2, screenHeight/2 - 2}, {0,0}, {0,0}, COLOR_GREEN,
  &layerCircle
};

// Player
AbTriangle playerShape = {
  abTriangleGetBounds, abTriangleCheck, PLAYER_SIZE
};
Layer layerPlayer = {
  (AbShape *)&playerShape,
  {screenWidth/2, screenHeight/2}, {0,0}, {0,0}, COLOR_RED,
  &layerPlayerGun,
};
MovLayer movLayerPlayer = { &layerPlayer, {0,0}, &movLayerCircle };


#endif
