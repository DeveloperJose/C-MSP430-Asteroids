/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "star.h"
#include "triangle.h"

#define GREEN_LED BIT6


char* int_to_string(int num){
  if(num == 0)
    return "0";

  int i, rem, len = 0, n;
  n = num;
  while (n != 0)
    {
      len++;
      n /= 10;
    }
  char str[len+1];

  for (i = 0; i < len; i++)
    {
      rem = num % 10;
      num = num / 10;
      str[len - (i + 1)] = rem + '0';
    }
  str[len] = '\0';
  return str;
}

AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {(screenWidth/2)-1, (screenHeight/2)-1}
};

AbStar star = {
  abStarGetBounds, abStarCheck, {screenWidth, screenHeight}
};

Layer layer4 = {
  (AbShape *)&star,
  {0, 0}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  WHITE,
  0
};
  

Layer layer3 = {		/**< Layer with an orange circle */
  (AbShape *)&circle10,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_VIOLET,
  &layer4,
};


Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &layer3
};

#define PLAYER_SIZE 10
AbTriangle playerTriangle = {
  abTriangleGetBounds, abTriangleCheck, PLAYER_SIZE, 0
};

Layer layer1 = {		/**< Layer with a red square */
  (AbShape *)&playerTriangle,
  {screenWidth/2, screenHeight/2}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &fieldLayer,
};

Layer layer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle14,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &layer1,
};

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
MovLayer ml3 = { &layer3, {1,1}, 0 }; /**< not all layers move */
MovLayer ml1 = { &layer1, {0,0}, &ml3 }; 
MovLayer ml0 = { &layer0, {2,1}, &ml1 }; 

movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (velocity);
      }	/**< if outside of fence */
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}


u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


Vec2 leftVel = {-3, 0};
Vec2 rightVel = {3, 0};



/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);

  shapeInit();

  layerInit(&layer0);
  layerDraw(&layer0);

  layerGetBounds(&fieldLayer, &fieldFence);


  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */


  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);

    //drawString5x7(0,0, "                                ", COLOR_WHITE, COLOR_BLACK);
    
    int x = (&layer4)->pos.axes[0];
    char* str_x = int_to_string(x);
    //drawString5x7(0,0, str_x, COLOR_WHITE, COLOR_BLACK);

    int input = p2sw_read();
    char* str_input = int_to_string(input);
    //drawString5x7(15,0, str_input, COLOR_WHITE, COLOR_BLACK);

    drawString5x7(3, 3, "Lives: 0", COLOR_WHITE, COLOR_BLACK); 
  }
}
unsigned char warping = 0;
Vec2 warpPos;
/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {
    mlAdvance(&ml0, &fieldFence);

    u_int input = p2sw_read();

    if (input){
      redrawScreen = 1;
      
      unsigned char top_s1_state_down = (input & 1) ? 0 : 1;
      unsigned char top_s2_state_down = (input & 2) ? 0 : 1;
      unsigned char top_s3_state_down = (input & 4) ? 0 : 1;
      unsigned char top_s4_state_down = (input & 8) ? 0 : 1;
    
      if(top_s1_state_down){
	(&ml1)->velocity = leftVel;
	//drawString5x7(0,0, "S4", COLOR_WHITE, COLOR_BLACK);
      }
      else if(top_s2_state_down){
	(&ml1)->velocity = rightVel;
      }
      else{
	(&ml1)->velocity = vec2Zero;
      }      



      if(warping){
	(&ml1)->velocity = vec2Zero;
	warping = 0;
      }
      // They touched the left side, warp them right

      /*else if((&ml1)->layer->pos.axes[0] <= PLAYER_SIZE - 5){
	warping = 1;
	Vec2 pos = (&ml1)->layer->pos;
	Vec2 rightSide = {-screenWidth + PLAYER_SIZE + 20, 0};
	(&ml1)->velocity = rightSide;
	
      }
      else if((&ml1)->layer->pos.axes[0] >= PLAYER_SIZE + 5){
	warping = 1;
	Vec2 pos = (&ml1)->layer->pos;
	Vec2 leftSide = {(screenWidth - PLAYER_SIZE - 19), 0};
	(&ml1)->velocity = leftSide;
      	
	}*/
       
    }
    
    
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
