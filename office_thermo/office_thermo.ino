#include <OneWire.h>

#include <SPI.h>

#include <Wire.h>
#include "DS1631.h"
#include <EEPROM.h>
volatile float SET;
OneWire ds(10);  // on pin 10
DS1631 Office_Temp(0);//init with address of zero
unsigned long sw_time=0;
float HS=0;
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
    delay(100);
  }
  average=average/10.0;
  average=average*1.8;
  average=average+32;
  //Serial.print("Temp is:  ");
  Serial.print(average);
  Serial.print(",");
  Serial.print(SET);
  if (average<SET && HS<125)
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
  HeatSinkTemp();
  //delay(200);
  
}


void HeatSinkTemp()
{
int HighByte,LowByte,TReading,SignBit,Tc_100,Whole,Fract;
byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  ds.reset_search();
  if ( !ds.search(addr)) {
      //Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {return;}
  if ( addr[0] == 0x10) {}
  else if ( addr[0] == 0x28) {}
  else {return;}

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  delay(750);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  LowByte=data[0];
  HighByte=data[1];
  TReading=(HighByte<<8)+LowByte;
  SignBit=TReading&0x8000;
  if(SignBit)
  {
    TReading=(TReading ^ 0xffff)+1;
  }
  Tc_100=(6*TReading)+TReading/4;
  Whole=Tc_100/100;
  Fract=Tc_100%100;
float HSTemp=((float)Tc_100)/100;
HSTemp=HSTemp*1.8;
  HSTemp=HSTemp+32;
if(SignBit)
{
  HSTemp=HSTemp*-1;
}
Serial.print(",");
Serial.print(HSTemp);
Serial.println(",125");
HS=HSTemp;
}
