#ifndef mov_layer_included
#define mov_layer_included

#include <shape.h>

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

void movLayerDraw(MovLayer *movLayers, Layer *layers);
void movLayerAdvance(MovLayer *ml, Region *fence);

#endif
