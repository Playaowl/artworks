
#include <Wire.h>

#include <SRF02.h>

SRF02 srf02[] = {
  SRF02(0x70, SRF02_CENTIMETERS)
  };
  unsigned long nextPrint = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  SRF02::configureDeviceId(0x70, 0x74);
}
void loop() {
  
}





