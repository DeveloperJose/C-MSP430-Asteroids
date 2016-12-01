#include <shape.h>
#include "star.h"

static unsigned int count = 150;
// true if pixel is in rect centerPosed at rectPos
int 
abStarCheck(const AbStar *star, const Vec2 *centerPos, const Vec2 *pixel)
{
  Region bounds;
  abRectGetBounds(star, centerPos, &bounds);
  int within = 0, axis;
  for (axis = 0; axis < 2; axis ++) {
    int p = pixel->axes[axis];
    if (p > bounds.botRight.axes[axis] || p < bounds.topLeft.axes[axis])
      within = 0;
    else{
      count--;
      if(count == 0){
	within = 1;
	count = 150;
      }
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
