#include <SPI.h>

#include <Adafruit_WS2801.h>

int dataPin = 2;
int clockPin = 3;
 
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(8, dataPin, clockPin);

void setup() {
 
  strip.begin();
 
  // Update LED contents, to start they are all 'off'
  strip.show();
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
 
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
void loop()  {
  colorWipe(Color(255, 0, 0), 50);  // red fill
  colorWipe(Color(0, 255, 0), 50);  // green fill
  colorWipe(Color(0, 0, 255), 50);  // blue fill
}
