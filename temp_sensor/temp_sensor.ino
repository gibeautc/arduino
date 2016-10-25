//#include <OneWire.h>
//OneWire ds(pin_number);
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
float temp=5.0;


boolean getTemp();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(getTemp){Serial.print("Temp is: ");Serial.println(temp);}
  else{Serial.println("Error");}
  delay(2000);  
}



//*******getTemp()***********
boolean getTemp()
{
/*
  byte i;
  byte present=0;
  byte data[12];
  byte data[8];

  if (!ds.search(addr)){
    ds.reset_search();
    return false;
  }
  if(OneWire::crc8(addr,7( !=addr[7])
  {
    return false;
  }
  if(addr[0]!=DS18S20_ID && addr[0]!=DS18B20_ID)
  {
    return false;
  }
  ds.reset();
  ds.select(addr);

  ds.write(0x44,1)
  delay(850);
  present=ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  for(i=0;i<9;i++)
  {
    data[i]=ds.read();
  }
  temp=((data[1]<<8)+data[0])*.0625;
  return true;
  */
}

