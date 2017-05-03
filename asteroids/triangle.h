#ifndef include_triangle
#define include_triangle

/** An AbShape Right Arrow with filled tip
 *
 *  size: width of the arrow.  Tip is a triangle with width=1/2 size.
 *  The "centerPos is at the arrow's tip.
 */
typedef struct AbTriangle_s {
  void (*getBounds)(const struct AbTriangle_s *shape, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbTriangle_s *shape, const Vec2 *centerPos, const Vec2 *pixelLoc);
  int size;
} AbTriangle;

/** As required by AbShape
 */
void abTriangleGetBounds(const AbTriangle *shape, const Vec2 *centerPos, Region *bounds);

/** As required by AbShape
 */
int abTriangleCheck(const AbTriangle *shape, const Vec2 *centerPos, const Vec2 *pixel);

#endif
