#include "FastLED.h"
#include <Wire.h>

// How many leds in your strip?
#define NUM_LEDS 8

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 11
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
float temperaturen[64];
float temperaturenHottie[64];

#define STATUS_START 0
#define STATUS_FRAGE 1
#define STATUS_ANTWORT1 2
#define STATUS_ANTWORT2 3
#define STATUS_ENDE 4
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
int ueberschriftState = LOW;
int frageState = LOW;
int antwort1State = LOW;
int antwort2State = LOW;
int status=STATUS_START;
unsigned long startMillis = millis();
unsigned long startZyklus = millis();
long interval = 1000; 

long dauerStatusStart= 10000;
long dauerStatusFrage= 20000;
long dauerStatusAntwort= 30000;
long dauerStatusEnde= 60000;

unsigned long previousMillis = 0;


void setup() {
  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);

  //Clear out the array
  for(int i = 0 ; i < NUM_LEDS ; i++){
    leds[i] = CRGB::Black;
  }
  for(int pixel = AREA_UEBERSCHRIFT_BEGIN; pixel < AREA_UEBERSCHRIFT_ENDE; pixel++) {
    leds[pixel] = CRGB::White;
  }

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
  unsigned long currentMillis = millis();
  delay(1);
  unsigned long laufzeit=currentMillis - startZyklus;
  if (laufzeit < dauerStatusStart) {
    status=STATUS_START;
  } 
  else if  (laufzeit < dauerStatusFrage) {
    status=STATUS_FRAGE;
  } 
  else if  (laufzeit < dauerStatusAntwort) {
    status=STATUS_ANTWORT1;
  } 
  else if  (laufzeit < dauerStatusEnde) {
    status=STATUS_ENDE;
  } 
  else{
    startZyklus=millis();
  }
  if(status==STATUS_FRAGE) {

    for(int pixel = AREA_FRAGE_BEGIN; pixel < AREA_FRAGE_ENDE; pixel++) {
      leds[pixel]=CRGB::White;
    }
  } 
  else{
    for (int pixel=AREA_FRAGE_BEGIN;pixel < AREA_FRAGE_ENDE;pixel++) {
      leds[pixel]=CRGB::Black;
    }
  }
  if(status==STATUS_ANTWORT1) {
    for(int pixel = AREA_ANTWORT1_BEGIN; pixel < AREA_ANTWORT1_ENDE; pixel++) {
      leds[pixel]=CRGB::White;
    }
  } 
  else{
    for (int pixel=AREA_ANTWORT1_BEGIN;pixel < AREA_ANTWORT1_ENDE;pixel++) {
      leds[pixel]=CRGB::Black;
    }
  }
  if(status==STATUS_ANTWORT2) {
    for(int pixel = AREA_ANTWORT2_BEGIN; pixel < AREA_ANTWORT2_ENDE; pixel++) {
      leds[pixel]=CRGB::White;
    }
  } 
  else{
    for (int pixel=AREA_ANTWORT2_BEGIN;pixel < AREA_ANTWORT2_ENDE;pixel++) {
      leds[pixel]=CRGB::Black;
    }
  }
  if (status!=STATUS_ENDE) {
    //First two data registers for the pixel temp data are 0x80 and 0x81
    pixelTempL=0x80;
    pixelTempH=0x81;
    //Get Temperature Data for each pixel in the 8x8 array. Will loop 64 times.
    float max=0;
    float summe=0;
    for(int pixel = AREA_MATRIX_BEGIN,  pos=0; pixel < AREA_MATRIX_ENDE; pixel++, pos++){
      //Get lower level pixel temp byte
      Wire.beginTransmission(addr);
      Wire.write(pixelTempL);
      Wire.endTransmission();
      Wire.requestFrom(addr,1);
      byte lowerLevel = Wire.read(); //
      //Get upper level pixel temp byte
      Wire.beginTransmission(addr);
      Wire.write(pixelTempH);
      Wire.endTransmission();
      Wire.requestFrom(addr,1);
      byte upperLevel = Wire.read();
      //Combine the two bytes together to complete the 12-bit temp reading
      int temperature = ((upperLevel << 8) | lowerLevel);
      //Temperature data is in two's compliment, do conversion.
      if (upperLevel != 0)
      {
        temperature = -(2048 - temperature);
      }
      celsius = temperature*0.25;
      if (status==STATUS_FRAGE){
        if (celsius > max) {
          max=celsius;
        }
        summe+=celsius;

        temperaturen[pos]=celsius;
      }
      //Determine LED color based on temperature of pixel
      if (status==STATUS_FRAGE||status==STATUS_ANTWORT1) {
        leds[pixel]=calcTemperatur(celsius);
      } 
      else {
        leds[pixel]=CRGB::Black;
      }
      //Go to next pixel by advancing both the low and high bit two register values
      pixelTempL=pixelTempL+2;
      pixelTempH=pixelTempH+2;
    }
    //Thermistor Register - Optional
    Wire.beginTransmission(addr);
    Wire.write(0x0E);
    Wire.endTransmission();
    Wire.requestFrom(addr,1);
    byte upperLevelTherm = Wire.read();
    Wire.beginTransmission(addr);
    Wire.write(0x0F);
    Wire.endTransmission();
    Wire.requestFrom(addr,1);
    byte lowerLevelTherm = Wire.read();
    int temperatureTherm = ((lowerLevelTherm << 8) | upperLevelTherm);
    int celsiusTherm = temperatureTherm*0.0625;


  } 
  else {
    for(int pixel = AREA_MATRIX_BEGIN; pixel < AREA_MATRIX_ENDE; pixel++){
      leds[pixel]=CRGB::Black;
    }
  }
  FastLED.show();
}




CRGB calcTemperatur(float celsius){
  CRGB color;

  if (celsius < 20.0)
  {
    color=CRGB::Black;
  } 
  else if (celsius >=20.0 && celsius <=45.0)
  {
    int colorvalue=160+(celsius-20)*3.84;
    color=CHSV( colorvalue, 255, 255);
    } 
  else
  {
    color=CHSV( 0, 255, 255);
  }
 
  return color;
}








