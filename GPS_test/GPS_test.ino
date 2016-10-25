
char buffer[200];
int count=0;
int Hour,Min,Sec,Day,Month,Year;
char Status,NS,EW;
float Lat,Long,Speed,Course;
int Lat_Deg,Long_Deg,Lat_Dec_Min,Long_Dec_Min;
int TZ_off=-7;   //Time zone offset
int error;
int up=0;
//******Define Functions*******
void Read_GPS();



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);//Debug
Serial3.begin(4800);//GPS
}

//Looking for $GPRMC  
    //$GPRMC,time,A/V,LAT,N/S,LONG,E/W,SPEED,COURSE,DATE,VAR,VAR DIR?,CHECKSUM

void loop() {

  Read_GPS();
  if(Lat_Deg==45){up=1;}
  if(up){
  if(Lat_Deg != 45)
  {
    error++;
    Serial.print(Lat_Deg);
    Serial.print("  :Lat ERROR: ");
    Serial.println(error);
    delay(10000);
    Serial3.flush();
   }
  }
  //if(Long_Deg!=122)
  //{error++;Serial.print("Long ERROR: ");Serial.println(error);delay(10000);}
}

void Read_GPS()
{ 
  if(Serial3.available())
  {
    //Serial.print(".");
    //delay(5);
    buffer[count]=Serial3.read();
    count++;
  }
  //Serial.println(buffer);
  if(buffer[count-1]=='$')
  {
    
  if(buffer[2]=='R')
  {
    char *tok;
    
    //Serial.print("String: ");
    //Serial.println(buffer);
    count=0;
    tok=strtok(buffer,",");
    while(tok !=NULL)
    {
        /* Notes
         * Lat/long are 4426.98 and 12304.3991   have to pull deg off front side
         * Time is in 2 digit increments hour/min/sec
         * Status can be boolean V=no go  A=go
         * N/S and E/W are single char, prob leave then as such
         * Speed is in knots i think, need multiple conversion functions
         * Date is Day/Month/Year, all 2 digits
         * How to deal with date roll over with time zone shift???
         */
      char temp[10];
      switch(count)
      {
        case 0: break;//GPRMC
        case 1: for(int x=0;x<10;x++){temp[x]=NULL;}
                //Serial.print("Time: ");
                //Serial.println(tok); 
                temp[0]=tok[0];
                temp[1]=tok[1];
                Hour=atoi(temp);
                temp[0]=tok[2];
                temp[1]=tok[3];
                Min=atoi(temp);
                temp[0]=tok[4];
                temp[1]=tok[5];
                Sec=atoi(temp);
                //Serial.print("Hour: ");Serial.println(Hour);
                Hour+=TZ_off;
                if(Hour>23){Hour-=24;}
                if(Hour<0){Hour+=24;}//Time Zone correction
                Serial.print("Hour: ");Serial.println(Hour);
                Serial.print("Min: ");Serial.println(Min);
                Serial.print("Sec: ");Serial.println(Sec);
                break;//time
        case 2: for(int x=0;x<10;x++){temp[x]=NULL;}
                Status=tok[0];
                Serial.print("Status: ");Serial.println(Status);
                break;//Status A/V
        case 3: for(int x=0;x<10;x++){temp[x]=NULL;}
                temp[0]=tok[0];
                temp[1]=tok[1];
                Lat_Deg=atoi(temp);
                Serial.print("Lat (Deg): ");Serial.println(Lat_Deg);
                temp[0]=tok[2];
                temp[1]=tok[3];
                temp[2]=tok[4];
                temp[3]=tok[5];
                temp[4]=tok[6];
                temp[5]=tok[7];
                temp[6]=tok[8];
                Lat=atof(temp);
                Serial.print("Lat (Dec Min): ");Serial.println(Lat);
                break;//Lat
        case 4: for(int x=0;x<10;x++){temp[x]=NULL;}
                NS=tok[0];
                Serial.print("N/S: ");Serial.println(NS);
                break;//N/S
        case 5: for(int x=0;x<10;x++){temp[x]=NULL;}
                temp[0]=tok[0];
                temp[1]=tok[1];
                temp[2]=tok[2];
                Long_Deg=atoi(temp);
                Serial.print("Long (deg): ");Serial.println(Long_Deg);
                temp[0]=tok[3];
                temp[1]=tok[4];
                temp[2]=tok[5];
                temp[3]=tok[6];
                temp[4]=tok[7];
                temp[5]=tok[8];
                temp[6]=tok[9];
                Long=atof(temp);
                Serial.print("Long (Dec Min): ");Serial.println(Long);
                break;//Long
        case 6: for(int x=0;x<10;x++){temp[x]=NULL;}
                EW=tok[0];
                Serial.print("W/E: ");Serial.println(EW);
                break;//W/E
        case 7: Speed=atof(tok);
                Serial.print("Speed: ");Serial.println(Speed);
                break;//Speed
        case 8: Course=atof(tok);
                Serial.print("Course: ");Serial.println(Course);
                break;//Course
        case 9: for(int x=0;x<10;x++){temp[x]=NULL;}
                temp[0]=tok[0];
                temp[1]=tok[1];
                Day=atoi(temp);
                temp[0]=tok[2];
                temp[1]=tok[3];
                Month=atoi(temp);
                temp[0]=tok[4];
                temp[1]=tok[5];
                Year=atoi(temp);
                Serial.print("Month: ");Serial.println(Month);
                Serial.print("Year: ");Serial.println(Year);
                Serial.print("Day: ");Serial.println(Day);
                break;//Date
        case 10: break;//Var
      }//end switch
      tok=strtok(NULL,",");
      count++;
    }//end while
    Serial.println("\n\n");
  } 
  
  for(int x=0;x<100;x++){buffer[x]=NULL;}
  count=0;//buffer counter
  }
}//End Read_GPS

