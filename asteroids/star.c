
#include <shape.h>
#include "star.h"
#include "timer.h"

static int distance = 500;
int 
abStarCheck(const AbStar *star, const Vec2 *centerPos, const Vec2 *pixel)
{
  Region bounds;
  abStarGetBounds(star, centerPos, &bounds);
  int within = 0, axis;
  for (axis = 0; axis < 2; axis ++) {
    int p = pixel->axes[axis];
    distance -= prand() % 5;
    if (p > bounds.botRight.axes[axis] || p < bounds.topLeft.axes[axis])
      within = 0;
    else if(distance <= 0){
      within = 1;
      distance = 650;
    }
  }
  return within;
}

// compute bounding box in screen coordinates for rect at centerPos
void abStarGetBounds(const AbStar *star, const Vec2 *centerPos, Region *bounds)
{
  vec2Sub(&bounds->topLeft, centerPos, &star->halfSize);
  vec2Add(&bounds->botRight, centerPos, &star->halfSize);
}
