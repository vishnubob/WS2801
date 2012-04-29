#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "WS2801.h"

LPD8806 strip = LPD8806("/dev/spidev2.0", 160);


/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.get_length(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.refresh();   // write all the pixels out
    usleep(wait * 10000);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.get_length(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.get_length() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.get_length()) + j) % 384) );
    }  
    strip.refresh();   // write all the pixels out
    usleep(wait * 1000);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.get_length(); i++) {
      strip.setPixelColor(i, c);
      strip.refresh();
      usleep(wait * 10000);
  }
}

// Chase a dot down the strip
// good for testing purposes
void colorChase(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.get_length(); i++) {
    strip.setPixelColor(i, 0);  // turn all pixels off
  } 
  
  for (i=0; i < strip.get_length(); i++) {
      strip.setPixelColor(i, c);
      if (i == 0) { 
        strip.setPixelColor(strip.get_length()-1, 0);
      } else {
        strip.setPixelColor(i-1, 0);
      }
      strip.refresh();
      usleep(wait * 100);
  }
}

int main(int argc, char* argv[]) 
{

  while(1)
  {
  rainbowCycle(1);  // make it go through the cycle fairly fast
  colorChase(strip.Color(255,255,255), 10);

  // Send a simple pixel chase in...
  colorChase(strip.Color(255,0,0), 10);  	// full brightness red
  colorChase(strip.Color(255,255,0), 10);	// orange
  colorChase(strip.Color(0,255,0), 10);		// green
  colorChase(strip.Color(0,255,255), 10);	// teal
  colorChase(strip.Color(0,0,255), 10);		// blue
  colorChase(strip.Color(255,0,255), 10);	// violet


  colorWipe(strip.Color(0, 0, 0), 0);
  // fill the entire strip with...
  colorWipe(strip.Color(0xff,0,0), 1);		// red
  colorWipe(strip.Color(0, 0, 0), 1);
  colorWipe(strip.Color(0, 0xff,0), 1);		// green
  colorWipe(strip.Color(0, 0, 0), 1);
  colorWipe(strip.Color(0,0,0xff), 1);		// blue
  colorWipe(strip.Color(0, 0, 0), 1);

  rainbow(20);
  }
}

