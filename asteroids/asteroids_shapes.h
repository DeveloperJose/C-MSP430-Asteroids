#ifndef include_game_draw
#define include_game_draw

#include <shape.h>
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
