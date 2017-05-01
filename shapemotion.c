 
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
#include "shapemotion.h"
//#include "temp.s"

#define GREEN_LED BIT6

//#define SW1 BIT3	/* switch1 is p1.3 */
#define SW0 BIT0
#define SW1 BIT1
#define SW2 BIT2
#define SW3 BIT3

//#define SWITCHES SW1		/* only 1 switch on this board */
#define SWITCHES (SW0|SW1|SW2|SW3)

//#define leftPos 30

AbRect bar = {abRectGetBounds, abRectCheck, {8,1}}; /**< 10x10 rectangle layerBar   */
AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle   big rectangle */
AbRect rect7 = {abRectGetBounds, abRectCheck, {7,7}}; /**< 10x10 rectangle   medium rectangle */
AbRect rect3 = {abRectGetBounds, abRectCheck, {7,7}}; /**< 10x10 rectangle   small rectangle */
AbRect end = {abRectGetBounds, abRectCheck, {150,150}}; /**< 10x10 rectangle   small rectangle */

AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 10, screenHeight/2 - 14}
};

Layer layerBar = {		/**< redBar */
  (AbShape *)&bar,
  {60,140},                  /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  0,//&fieldLayer,
};

Layer layer5 = {        /** Big2 rectagle **/
  (AbShape *)&circle7,//rect10,
  {100,70}, //{(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layerBar,//&layer6,
};

Layer layer4 = {        /** Big1 rectagle **/
  (AbShape *)&circle7,//rect10,
  {50,25}, //{(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layer5,
};
  

Layer layer3 = {		/**< Layer medium orange square*/ 
  (AbShape *)&circle7,//rect7,//circle8,
  {20,20},              /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layer4,
};


Layer layer0 = {        /**< Layer small yellow cube */
  (AbShape *)&circle7,//rect3,//circle14,
  {80,50},               /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layer3,
  
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layer0
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
/**< not all layers move */

MovLayer ml5 = { &layer5, {0,1}, 0 }; 
MovLayer ml4 = { &layer4, {0,2}, &ml5 }; 
MovLayer ml3 = { &layer3, {0,3}, &ml4 }; 
MovLayer redBar = { &layerBar, {0,0}, &ml3 }; // red bar
MovLayer ml0 = { &layer0, {0,3}, &redBar };  //layer0

int count = 0;
short score = '0';
int gameOver = 1;
int life = 3;

u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */
Region fieldFence;		/**< fence around playing field  */

void movLayerDraw(MovLayer *movLayers, Layer *layers){
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

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence, MovLayer *bar){
  
  Vec2 newPos;
  Vec2 newPosBar1;
  //Vec2 newPosBar2;
  u_char axis;
  Region shapeBoundary;
  Region redBar1;
  
  for (; ml; ml = ml->next) {
      
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    vec2Add(&newPosBar1, &bar->layer->pos, &bar->velocity); //redBar
    
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    abShapeGetBounds(bar->layer->abShape, &newPosBar1, &redBar1); // redBard Boundaries
    
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) || 
          (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis])){
          int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
          newPos.axes[axis] = 25;
      }	/**< if outside of fence */
      
      if(ml != &redBar){
          if(shapeBoundary.topLeft.axes[0] < (redBar1.topLeft.axes[0] + 10) &&
            (shapeBoundary.topLeft.axes[0] + 14) > redBar1.topLeft.axes[0] &&
             shapeBoundary.botRight.axes[1] < (redBar1.botRight.axes[1] + 8) &&
            (shapeBoundary.botRight.axes[1] + 7) > redBar1.botRight.axes[1]){
              
              buzzer_set_period(800);
              drawString5x7(10, 150, "1 Life's left!", COLOR_GREEN, COLOR_BLACK);
              newPos.axes[1] = 25;
              life = life - 1;
          } // Colision happen
      }
      if(life == 0){
        gameOver = 0;
      }
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}

void buzzer_init(){ //Initialize the buzzer
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */

  //  buzzer_advance_frequency();	/* start buzzing!!! */
}


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */

void main(){
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  buzzer_init();
  shapeInit();
  p2sw_init(15);

  shapeInit();

  layerInit(&fieldLayer);
  layerDraw(&fieldLayer);

  layerGetBounds(&fieldLayer, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

   drawString5x7(10,5, "Avoid the Circles!", COLOR_RED, COLOR_BLACK);
   drawString5x7(10, 150, "2 Life's left!", COLOR_GREEN, COLOR_BLACK);
 // drawString5x7(10,150, "<- SW1  &  SW4 ->!", COLOR_RED, COLOR_BLACK);
  for(;;) { 
      
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &fieldLayer ); 
  }
}

/*************************8 PLAY MUSIC ***********************/

void turnOff(){
    buzzer_set_period(0);
}

void buzzer_set_period(short cycles){    // set the note for the buzzer
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

void play(){ //unsigned int newPeriod, unsigned int duration){
    for(int i = 4000; i < 2 * 10000000; i++){
        for(int j = 0; j < 2 * 10000; j++){ // play the note for a set duration in sec.
            buzzer_set_period(i);
        }
    }
}

void rest(unsigned int duration){
    for(int i = 0; i < duration * 10000; i++){ // dont play not for a set duration
     buzzer_set_period(0);
    }
}

/***************************** PLAY MUSIC **************************************/
int sound = 10000;

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler(){
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  static short flag = 0;
    if (gameOver == 0){
        drawString5x7(10, 150, "0 Life's left!", COLOR_GREEN, COLOR_BLACK);
        drawString5x7((screenWidth/2) -25, (screenHeight/2), "Game Over", COLOR_GREEN, COLOR_BLACK);
    }
    while (count == 15 && gameOver != 0) {
        u_int switches = p2sw_read();
        mlAdvance(&ml0, &fieldFence, &redBar);
        
        if (~switches & SW0){
            redBar.velocity.axes[0] = -5;
            redBar.velocity.axes[1] = 0;
        }
        else if (~switches & SW1){
           // buzzer_set_period(50000);
            sound = 5000;
        }
        else if (~switches & SW2){
            sound = 0;
        }
        else if (~switches & SW3){
            redBar.velocity.axes[0] = 5;
            redBar.velocity.axes[1] = 0;
        }
        else{
            buzzer_set_period(sound);
            redBar.velocity.axes[0] = 0;
            redBar.velocity.axes[1] = 0;
            }
        if(p2sw_read())
            redrawScreen = 1;
        count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

