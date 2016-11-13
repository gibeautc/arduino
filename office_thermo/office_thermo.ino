#include <Wire.h>
#include "DS1631.h"
#include <EEPROM.h>
float SET;

DS1631 Office_Temp(0);//init with address of zero


void setup() {
  SET= float(EEPROM.read(0));
  if(SET>75 || SET<60){SET=60;}//Error checking the EEPROM read at start up
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  Serial.begin(9600); //start serial
  Wire.begin();
  Serial.print("Startup Config:  ");
  int config=Office_Temp.readConfig();
  Serial.println(config,BIN);
  Office_Temp.writeConfig(13); //Set to 12 bit, 1 shot mode
  config=Office_Temp.readConfig();
  Serial.print("New Config:  ");
  Serial.println(config,BIN);
  Serial.print("Set Point is:  ");
  Serial.println(SET);
}

void thermostat()
{
  float temp=Office_Temp.readTempOneShot();
  temp=temp*1.8;
  temp=temp+32;
  Serial.print("Temp is:  ");
  Serial.println(temp);
  if (temp<SET)
  {
    Serial.println("ON");
    digitalWrite(8,HIGH);
  }
  else
  {
    digitalWrite(8,LOW);
    Serial.println("OFF");
  }
  
}


void loop() {

  thermostat();
  delay(1000);
  
}
