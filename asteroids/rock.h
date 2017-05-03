#ifndef include_rock
#define include_rock

/** AbShape rectangle
 *
 *  Vector halfSize must be to first quadrant (both axes non-negative).  
 *  Specifies extent in all four directions.
 */ 
typedef struct AbRock_s {
  void (*getBounds)(const struct AbRock_s *rock, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbRock_s *rock, const Vec2 *centerPos, const Vec2 *pixel);
  const Vec2 halfSize;
  unsigned char isActive;
} AbRock;

/** As required by AbShape
 */
void abRockGetBounds(const AbRock *rock, const Vec2 *centerPos, Region *bounds);

/** As required by AbShape
 */
int abRockCheck(const AbRock *rock, const Vec2 *centerPos, const Vec2 *pixel);

#endif
