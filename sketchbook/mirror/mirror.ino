#include <Wtv020sd16p.h>

#include <SM.h>


#include <Ultrasonic.h>

#include "FastLED.h"
#include <Wire.h>

// How many leds in your strip?
#define NUM_LEDS 8

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 11
#define CLOCK_PIN 13
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

// Define the array of leds
CRGB leds[NUM_LEDS];
float temperaturen[64];
float temperaturenHottie[64];
//#define AREA_UEBERSCHRIFT_BEGIN 0
//#define AREA_UEBERSCHRIFT_ENDE 16
//#define AREA_FRAGE_BEGIN 16
//#define AREA_FRAGE_ENDE 32
//#define AREA_MATRIX_BEGIN 32
//#define AREA_MATRIX_ENDE 88
//#define AREA_ANTWORT1_BEGIN 88
//#define AREA_ANTWORT1_ENDE 92
//#define AREA_ANTWORT2_BEGIN 92
//#define AREA_ANTWORT2_ENDE 96
#define AREA_UEBERSCHRIFT_BEGIN 0
#define AREA_UEBERSCHRIFT_ENDE 2
#define AREA_FRAGE_BEGIN 2
#define AREA_FRAGE_ENDE 4
#define AREA_MATRIX_BEGIN 4
#define AREA_MATRIX_ENDE 6
#define AREA_ANTWORT1_BEGIN 6
#define AREA_ANTWORT1_ENDE 7
#define AREA_ANTWORT2_BEGIN 7
#define AREA_ANTWORT2_ENDE 8

byte pixelTempL;
byte pixelTempH;
int ledNum = 1;
char addr = 0x69;
float celsius;
Wtv020sd16p wtv020sd16p(resetPin, clockPin, dataPin, busyPin);
//Ultrasonic ultrasonic(trigPin,echoPin); //Ultrasonic ultrasonic(Trig,Echo);

SM Simple(detectPerson);
void setup() {
  wtv020sd16p.reset();

//  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
//
//  //Clear out the array
//  for (int i = 0 ; i < NUM_LEDS ; i++) {
//    leds[i] = CRGB::Black;
//  }
//  for (int pixel = AREA_UEBERSCHRIFT_BEGIN; pixel < AREA_UEBERSCHRIFT_ENDE; pixel++) {
//    leds[pixel] = CRGB::White;
//  }

  Wire.begin();
  //  Serial.begin(115200);
  //  byte clr;
  //  SPCR = B01010001; //SPI Registers
  //  SPSR = SPSR & B11111110; //make sure the speed is 125KHz
  //  clr=SPSR;
  //  clr=SPDR;
  Serial.begin(9600);           // set up Serial library at 9600 bps


  delay(10);
}


void loop() {
  EXEC(Simple);
}

State detectPerson()
{

    Serial.println(" detectPerson "); 
//int cm=ultrasonic.Ranging(CM);
//Serial.println(cm); 

//  if (cm < 20) {
    Simple.Set(askQuestion);
//  }

}




State askQuestion()
{
  wtv020sd16p.playVoice(0);
  Simple.Set(askQuestion1);
}

State askQuestion1()
{
Simple.Finish();
  Serial.println(" inAskQuestion "); 
  for (int pixel = AREA_FRAGE_BEGIN; pixel < AREA_FRAGE_ENDE; pixel++) {
    leds[pixel] = CRGB::White;
  }
  FastLED.show();

  //Audio Out, wait 5 sec
  if (Simple.Timeout(10000)) {
    for (int pixel = AREA_FRAGE_BEGIN; pixel < AREA_FRAGE_ENDE; pixel++) {
      leds[pixel] = CRGB::Black;
    }
    FastLED.show();

    Simple.Set(waitForQuestion);
  }
}

State  waitForQuestion() {
  Serial.println(" waitForQuestion "); 
  //Audio In, detect sound, wait 10 src
  if (Simple.Timeout(10000)) {
    Simple.Set(showPicture);
  }
}

State showPicture() {
  //show Picture for 10 src


  boolean hot = updatePicture();
  if (Simple.Timeout(10000)) {
    if (hot) {
      Simple.Set(answerHot);
    }  else {
      Simple.Set(answerNot);


    }
  }
}

State answerHot()
{
    wtv020sd16p.asyncPlayVoice(1);
        Simple.Set(answerHot1);

}
State answerHot1()
{
    Serial.println(" answerHot "); 

  for (int pixel = AREA_ANTWORT1_BEGIN; pixel < AREA_ANTWORT1_ENDE; pixel++) {
    leds[pixel] = CRGB::White;
  }
  FastLED.show();

  //Audio Out, wait 5 sec
  if (Simple.Timeout(10000)) {
    for (int pixel = AREA_ANTWORT1_BEGIN; pixel < AREA_ANTWORT1_ENDE; pixel++) {
      leds[pixel] = CRGB::Black;
    }  FastLED.show();

    Simple.Set(noop);
  }
}

State answerNot()
{
    Serial.println(" answerNot "); 
  for (int pixel = AREA_ANTWORT2_BEGIN; pixel < AREA_ANTWORT2_ENDE; pixel++) {
    leds[pixel] = CRGB::White;
  }
  FastLED.show();
  wtv020sd16p.asyncPlayVoice(2);

  //Audio Out, wait 5 sec
  if (Simple.Timeout(10000)) {
    for (int pixel = AREA_ANTWORT2_BEGIN; pixel < AREA_ANTWORT2_ENDE; pixel++) {
      leds[pixel] = CRGB::Black;
    }  FastLED.show();

    Simple.Set(noop);
  }
}


boolean updatePicture() {
  //First two data registers for the pixel temp data are 0x80 and 0x81
  pixelTempL = 0x80;
  pixelTempH = 0x81;
  //Get Temperature Data for each pixel in the 8x8 array. Will loop 64 times.
  float max = 0;
  float summe = 0;
  for (int pixel = AREA_MATRIX_BEGIN,  pos = 0; pixel < AREA_MATRIX_ENDE; pixel++, pos++) {
    //Get lower level pixel temp byte
    Wire.beginTransmission(addr);
    Wire.write(pixelTempL);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    byte lowerLevel = Wire.read(); //
    //Get upper level pixel temp byte
    Wire.beginTransmission(addr);
    Wire.write(pixelTempH);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    byte upperLevel = Wire.read();
    //Combine the two bytes together to complete the 12-bit temp reading
    int temperature = ((upperLevel << 8) | lowerLevel);
    //Temperature data is in two's compliment, do conversion.
    if (upperLevel != 0)
    {
      temperature = -(2048 - temperature);
    }
    celsius = temperature * 0.25;
    //     if (status==STATUS_FRAGE){
    if (celsius > max) {
      max = celsius;
    }
    summe += celsius;

    temperaturen[pos] = celsius;
    //     }
    //Determine LED color based on temperature of pixel
    //     if (status==STATUS_FRAGE||status==STATUS_ANTWORT1) {
    leds[pixel] = calcTemperatur(celsius);
    //      }
    //      else {
    //        leds[pixel]=CRGB::Black;
    //      }
    //Go to next pixel by advancing both the low and high bit two register values
    pixelTempL = pixelTempL + 2;
    pixelTempH = pixelTempH + 2;
  }
  //Thermistor Register - Optional
  Wire.beginTransmission(addr);
  Wire.write(0x0E);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);
  byte upperLevelTherm = Wire.read();
  Wire.beginTransmission(addr);
  Wire.write(0x0F);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);
  byte lowerLevelTherm = Wire.read();
  int temperatureTherm = ((lowerLevelTherm << 8) | upperLevelTherm);
  int celsiusTherm = temperatureTherm * 0.0625;

  FastLED.show();



}

State  noop() {
    Serial.println(" noop "); 
  //Audio In, detect sound, wait 10 src
  if (Simple.Timeout(20000)) {
    Simple.Set(detectPerson);
  }
}



CRGB calcTemperatur(float celsius) {
  CRGB color;

  if (celsius < 20.0)
  {
    color = CRGB::Black;
  }
  else if (celsius >= 20.0 && celsius <= 45.0)
  {
    int colorvalue = 160 + (celsius - 20) * 3.84;
    color = CHSV( colorvalue, 255, 255);
  }
  else
  {
    color = CHSV( 0, 255, 255);
  }

  return color;
}



//MIC-INPUT
//int currentValue;
//int maxValue;
//int minValue;
//unsigned long timer;
//int sampleSpan = 5; // Amount in milliseconds to sample data
//int volume; // this roughly goes from 0 to 700
//
//void setup()
//{
//    Serial.begin(9600);
//    resetValues();
//}
//
//void loop()
//{
//    currentValue = analogRead(A0);
//
//    if (currentValue < minValue) {
//        minValue = currentValue;
//    }
//    if (currentValue > maxValue) {
//        maxValue = currentValue;
//    }
//
//    if (millis() - timer >= sampleSpan) {
//        volume = maxValue - minValue;
//
//        Serial.println(volume);
//
//        resetValues();
//    }
//}
//
//void resetValues()
//{
//    maxValue = 0;
//    minValue = 1024;
//    timer = millis();
//}
//


//
//long ping() {
//  // establish variables for duration of the ping,
//  // and the distance result in inches and centimeters:
//  long duration, inches, cm;
//
//  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
//  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
//  pinMode(trigPin, OUTPUT);
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//
//  // The same pin is used to read the signal from the PING))): a HIGH
//  // pulse whose duration is the time (in microseconds) from the sending
//  // of the ping to the reception of its echo off of an object.
//  pinMode(echoPin, INPUT);
//  duration = pulseIn(echoPin, HIGH);
//
//  // convert the time into a distance
//  inches = microsecondsToInches(duration);
//  cm = microsecondsToCentimeters(duration);
//      Serial.println(cm); 
//
//  return cm;
//}
//
//long microsecondsToInches(long microseconds)
//{
//  // According to Parallax's datasheet for the PING))), there are
//  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
//  // second).  This gives the distance travelled by the ping, outbound
//  // and return, so we divide by 2 to get the distance of the obstacle.
//  return microseconds / 74 / 2;
//}
//
//long microsecondsToCentimeters(long microseconds)
//{
//  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
//  // The ping travels out and back, so to find the distance of the
//  // object we take half of the distance travelled.
//  return microseconds / 29 / 2;
//}

