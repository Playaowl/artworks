
#include <Wire.h>
//define our colors for LED Array
#define GREEN 0x01
#define RED 0x02
#define ORANGE 0x03
//define the SPI pins for the LED Array
#define DATAOUT 11//MOSI
#define DATAIN 12//MISO
#define SPICLOCK 13//sck
#define SLAVESELECT 10//ss
char ledArray [64];
byte pixelTempL;
byte pixelTempH;
int ledNum = 1;
char addr = 0x69;
int celsius;
void setup() {
  Wire.begin();
  Serial.begin(115200);
  byte clr;
  SPCR = B01010001; //SPI Registers
  SPSR = SPSR & B11111110; //make sure the speed is 125KHz
  clr=SPSR;
  clr=SPDR;
  delay(10);
  //Set the pin modes for the RG matrix
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);
  //Make sure the RG matrix is deactivated
  digitalWrite(SLAVESELECT,HIGH);
}


void loop()
{
  //First two data registers for the pixel temp data are 0x80 and 0x81
  pixelTempL=0x80;
  pixelTempH=0x81;
  //Get Temperature Data for each pixel in the 8x8 array. Will loop 64 times.
  for(int pixel = 0; pixel <= 63; pixel++){
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
    //Determine LED color based on temperature of pixel
    if (celsius < 26)
    {
      ledArray[pixel]=GREEN;
    } 
    else if (celsius >=26 && celsius <=30)
    {
      ledArray[pixel]=ORANGE;
    } 
    else
    {
      ledArray[pixel]=RED;
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
  //Transfer color contents of LED array for display
  for(int pixel=0; pixel<=63; pixel++){
    digitalWrite(SLAVESELECT, LOW);
    delayMicroseconds(500);
    spi_transfer(ledArray[pixel]);
    delayMicroseconds(500);
    digitalWrite(SLAVESELECT, HIGH);
  }
} //end loop
//LED array send function
char spi_transfer(volatile char data)
{
  SPDR = data; // Start the transmission
  while (!(SPSR & (1<<SPIF))) // Wait for the end of the transmission
  {
  };
  return SPDR; // return the received byte
}

