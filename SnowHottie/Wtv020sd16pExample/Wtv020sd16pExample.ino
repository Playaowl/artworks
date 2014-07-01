/*
 Example: Control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by Diego J. Arevalo, August 6th, 2012.
 Released into the public domain.
 */

#include <Wtv020sd16p.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

void setup() {
  //Initializes the module.
  wtv020sd16p.reset();
}

void loop() {
  //Plays synchronously an audio file. Busy pin is used for this method.
  wtv020sd16p.asyncPlayVoice(0);
delay(5000);
  wtv020sd16p.asyncPlayVoice(1);
delay(5000);  wtv020sd16p.asyncPlayVoice(2);
delay(5000);wtv020sd16p.asyncPlayVoice(12);
delay(5000);wtv020sd16p.asyncPlayVoice(4);
delay(5000);wtv020sd16p.asyncPlayVoice(14);
delay(5000);wtv020sd16p.asyncPlayVoice(10);
delay(5000);wtv020sd16p.asyncPlayVoice(12);
delay(5000);wtv020sd16p.asyncPlayVoice(19);
delay(5000);  wtv020sd16p.asyncPlayVoice(3);delay(5000);
 }


