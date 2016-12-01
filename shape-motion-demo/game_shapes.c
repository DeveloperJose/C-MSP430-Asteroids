#include "shape.h"
#include "game_shapes.h"

 
/** Check function required by AbShape
 *  abRArrowGetBounds computes a right arrow's bounding box
 */
void 
abTriangleGetBounds(const AbTriangle *triangle, const Vec2 *centerPos, Region *bounds)
{
  int size = triangle->size, halfSize = size / 2;
  bounds->topLeft.axes[0] = centerPos->axes[0] - size;
  bounds->topLeft.axes[1] = centerPos->axes[1] - halfSize;
  bounds->botRight.axes[0] = centerPos->axes[0];
  bounds->botRight.axes[1] = centerPos->axes[1] + halfSize;
}

int abTriangleCheck(const AbTriangle *triangle, const Vec2 *centerPos, const Vec2 *pixelLoc)
{
  Vec2 relPos;
  int row, col, within = 0;
  int size = triangle->size;
  int halfSize = size/2, quarterSize = halfSize/2;;
  vec2Sub(&relPos, pixelLoc, centerPos); /* vector from center to pixel */
  row = -relPos.axes[0]; col = relPos.axes[1]; /* note that col is negated */
  row = (row >= 0) ? row : -row;/* row = |row| */
  if (col >= 0) {		/* not to right of arrow */
    if (col <= halfSize) {	/* within arrow tip */
      within = row <= col;
    } else if (col <= size) {	/* within arrow stem */
      //within = row <= quarterSize;
    }
  }
  return within;
}
