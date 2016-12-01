#ifndef game_shapes
#define game_shapes

typedef struct AbTriangle_s {
  void (*getBounds)(const struct AbTriangle_s *triangle, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbTriangle_s *triangle, const Vec2 *centerPos, const Vec2 *pixelLoc);
  int size;
} AbTriangle;


void abTriangleGetBounds(const AbTriangle *triangle, const Vec2 *centerPos, Region *bounds);

int abTriangleCheck(const AbTriangle *triangle, const Vec2 *centerPos, const Vec2 *pixelLoc);

#endif
