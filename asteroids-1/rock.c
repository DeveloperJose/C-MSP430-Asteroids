#include <shape.h>
#include "rock.h"

static unsigned int count = 175;
// true if pixel is in rect centerPosed at rectPos
int 
abRockCheck(const AbRock *rock, const Vec2 *centerPos, const Vec2 *pixel)
{
  Region bounds;
  abStarGetBounds(rock, centerPos, &bounds);
  int within = 1, axis;
  for (axis = 0; axis < 2; axis ++) {
    int p = pixel->axes[axis];
    if (p > bounds.botRight.axes[axis] || p < bounds.topLeft.axes[axis])
      within = 0;
    else{
      count--;
      if(count == 0){
	within = 0;
	count = 120 + prand() % 50;
      }
    }
  }
  return within;
}

// compute bounding box in screen coordinates for rect at centerPos
void abRockGetBounds(const AbRock *rock, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &rock->halfSize);
  vec2Add(&bounds->botRight, centerPos, &rock->halfSize);
}
