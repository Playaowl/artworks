
#include <Wire.h>

#include <SRF02.h>

SRF02 srf02[] = {
  SRF02(0x71, SRF02_CENTIMETERS),
  SRF02(0x72, SRF02_CENTIMETERS),
  SRF02(0x73, SRF02_CENTIMETERS),
  SRF02(0x74, SRF02_CENTIMETERS)
  };
  unsigned long nextPrint = 0;
  int numSensors=4;
unsigned int limit=250;
unsigned long nachlauf=5000;
unsigned long time=millis()+nachlauf;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  SRF02::setInterval(70);
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  SRF02::initSequence();
  while (Serial.available() <=0) {
    Serial.println("Hallo");
  }

}
void loop() {
  unsigned long val[4];
  boolean lightOn;
  lightOn=false;
  for (int i=0; i < numSensors;i++) {
    SRF02::updateSequence();
    val[i]=srf02[i].read(); 
    //      Serial.print ("Sensor ");
    //      Serial.print (i);
    //      Serial.print(" Wert ");
    //      Serial.println(val);
    if (val[i] > 0 && val[i] < limit) {
      lightOn=true;
    }

  }
  if (lightOn||time>millis()) {
    digitalWrite(13, HIGH);
    if (lightOn){
      time=millis()+nachlauf;
    }
  } 
  else {
    digitalWrite(13, LOW);
  }
  if (Serial.available() > 0) {
    int inByte=Serial.read();
    for (int i=0; i < numSensors;i++) {
      if (i<numSensors-1) {
        Serial.print(val[i]);
        Serial.print(",");
      } 
      else {
        Serial.println(val[i]);
      }
    }
  }
}









