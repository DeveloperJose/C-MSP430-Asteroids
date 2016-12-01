#ifndef include_mov_layer
#define include_mov_layer

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
#include "shape.h"

typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

void movLayerDraw(MovLayer *movLayers, Layer *layers);
void movLayerAdvance(MovLayer *ml, Region *fence);

#endif
