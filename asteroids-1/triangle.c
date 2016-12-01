#include "shape.h"
#include "triangle.h"

/** Check function required by AbShape
 *  abRArrowCheck returns true if the right arrow includes the selected pixel
 */
int 
abTriangleCheck(const AbTriangle *shape, const Vec2 *centerPos, const Vec2 *pixel)
{
  Vec2 relPos;
  int row, col, within = 0;
  int size = shape->size;
  
  vec2Sub(&relPos, pixel, centerPos);
  row = relPos.axes[1]; col = -relPos.axes[0];
  row = (row >= 0) ? row : -row;
  if (col >= 0 && col <= size) {
      within = row <= col;
    }
  }
  return within;
}
  
/** Check function required by AbShape
 *  abRArrowGetBounds computes a right arrow's bounding box
 */
void 
abTriangleGetBounds(const AbTriangle *shape, const Vec2 *centerPos, Region *bounds)
{
  int size = shape->size;
  bounds->topLeft.axes[0] = centerPos->axes[0] - size;
  bounds->topLeft.axes[1] = centerPos->axes[1] - size;
  bounds->botRight.axes[0] = centerPos->axes[0];
  bounds->botRight.axes[1] = centerPos->axes[1] + size;
}
