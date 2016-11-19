int message=128;
int rec=0;
int cor=0;
void setup() {
  Serial.begin(115200);
  blink_test();
  Serial_setup(300); 
}

void loop() 
{
  //TX on 3 
  //RX on 2
  delay(200);

  //message++;
  Serial3.println(message);
  Serial3.flush();
  //delay(100);
  rec=int(Serial2.read());
  if(rec==message){cor++;}
  Serial.println(cor);
  
}


void blink_test()
{
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);
  delay(500);
  pinMode(17,INPUT);
  //Serial.print("Output off: ");
  //Serial.println(digitalRead(17));
  digitalWrite(14,HIGH);
  delay(500);
  //Serial.print("Output on: ");
  //Serial.println(digitalRead(17));
  
}

void Serial_setup(long baud)
{
  Serial2.begin(baud);
  Serial3.begin(baud);
}

