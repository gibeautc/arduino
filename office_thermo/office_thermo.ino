#include <SPI.h>

#include <Wire.h>
#include "DS1631.h"
#include <EEPROM.h>
volatile float SET;

DS1631 Office_Temp(0);//init with address of zero
unsigned long sw_time=0;

void Set_Up()
{
  if(millis()-sw_time>200)
  {
    SET+=1.0;
    EEPROM.write(0,SET);
    sw_time=millis(); 
  }
 }

void Set_Down()
{
  if(millis()-sw_time>200)
  {
    SET-=1.0;
    EEPROM.write(0,SET);
    sw_time=millis();
  } 
}


void setup() {
  SET= float(EEPROM.read(0));
  if(SET>75 || SET<60){SET=60;}//Error checking the EEPROM read at start up
  pinMode(8,OUTPUT);
  pinMode(18,INPUT_PULLUP);
  pinMode(19,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), Set_Up, FALLING); 
  attachInterrupt(digitalPinToInterrupt(19), Set_Down, FALLING); 
  digitalWrite(8,LOW);
  Serial.begin(9600); //start serial
  Serial.flush();
  Wire.begin();
  //Serial.print("Startup Config:  ");
  int config=Office_Temp.readConfig();
  //Serial.println(config,BIN);
  Office_Temp.writeConfig(13); //Set to 12 bit, 1 shot mode
  config=Office_Temp.readConfig();
  //Serial.print("New Config:  ");
  //Serial.println(config,BIN);
  //Serial.print("Set Point is:  ");
  //Serial.println(SET);
}

void thermostat()
{
  float average=0.0;
  for(int x=0;x<10;x++)
  {
    float temp=Office_Temp.readTempOneShot();
    average+=temp;
    delay(200);
  }
  average=average/10.0;
  average=average*1.8;
  average=average+32;
  //Serial.print("Temp is:  ");
  Serial.print(average);
  Serial.print(",");
  Serial.println(SET);
  if (average<SET)
  {
    //Serial.println("ON");
    digitalWrite(8,HIGH);
  }
  else
  {
    digitalWrite(8,LOW);
    //Serial.println("OFF");
  }
  
}


void loop() {
  //String input=Serial.read();
  thermostat();
  delay(200);
  
}
