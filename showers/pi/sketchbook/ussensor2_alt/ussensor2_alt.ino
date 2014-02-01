

#include <Wire.h>

#include <SRF02.h>

SRF02 srf02[] = {
  SRF02(0x71, SRF02_CENTIMETERS),
  SRF02(0x72, SRF02_CENTIMETERS),
  SRF02(0x73, SRF02_CENTIMETERS),
  SRF02(0x74, SRF02_CENTIMETERS)
  };
  unsigned long nextPrint = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  SRF02::setInterval(70);
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
  SRF02::initSequence();
  while (Serial.available() <=0) {
    Serial.println("Hallo");
  }
}
void loop() {
  unsigned long val; 
  if( Serial.available() >0) {
    int inByte = Serial.read();
  }
  for (int i=0; i < 4;i++) {
    SRF02::updateSequence();
    val=srf02[i].read(); 
    if (i<3) {
      Serial.print(val);
      Serial.print(",");
    } 
    else {
      Serial.println(val);
    }
  }
}







