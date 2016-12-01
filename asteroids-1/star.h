#ifndef include_star
#define include_star

/** AbShape rectangle
 *
 *  Vector halfSize must be to first quadrant (both axes non-negative).  
 *  Specifies extent in all four directions.
 */ 
typedef struct AbStar_s {
  void (*getBounds)(const struct AbStar_s *star, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbStar_s *star, const Vec2 *centerPos, const Vec2 *pixel);
  const Vec2 halfSize;	
} AbStar;

/** As required by AbShape
 */
void abStarGetBounds(const AbStar *star, const Vec2 *centerPos, Region *bounds);

/** As required by AbShape
 */
int abStarCheck(const AbStar *star, const Vec2 *centerPos, const Vec2 *pixel);

#endif
