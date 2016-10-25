int count=0;
double set_temp=225;  //This is the ideal temp of the smoker 
double meat_temp; //This is the set temp for the meat
int set=100;         //less then 100 is damper, more then 100 is fan
                    //100 is nutural   range is 0 to 200 
#define Kp 0.5    //Present coeficent 
#define Ki 0.002    //Integral coeficent
#define Kd 0.005    //Dirivative coeficent
/*
 * for slow loops like temp the suggested "values" are 
 * P-More
 * I-Some, too much will cause cycling
 * D-Some
 */


//Currently keeping these static, could make them variable to allow for
//run-time changes to learn best practice
int Der=0;
int Int=0;
int DerPrev=0;
int IntPrev=0;
int err;
int errPrev;
unsigned long T;

struct node
{
  struct node * node_fw=NULL;
  struct node * node_bk=NULL;
  int data;  
};

struct LL_list
{
 int sz;
 node * first=NULL;
 node * last=NULL;
 int max_sz; 
};

LL_list temp_list;  // Store the temp of smoker
LL_list set_list;   //Store set point of fan/damper;

LL_list LL_init(int num,int max_size)
{
  struct LL_list new_list;
  new_list.sz=1;
  new_list.max_sz=max_size;
  node *new_node=(node*)malloc(sizeof(node));
  new_list.first=new_node;
  new_list.last=new_node;
  new_node->data=num;
  new_node->node_fw=NULL;
  new_node->node_bk=NULL;
  //Serial.print("Initial Num: ");
  //Serial.println(new_list.first->data);
  return new_list;
}

void test(String text,int x)
{

  Serial.print(text);
  if(x==1){Serial.println(":\t\t PASS");}
  else{Serial.println(": \t\tFAIL");}
  
}

void LL_test()
{
  struct LL_list myList=LL_init(5,3);
  if(myList.first->data==5){test("Init w/ 5",1);}else{test("Init w/ 5",0);}
  LL_add(&myList,10);
  if(myList.first->data==10 && myList.first->node_fw->data==5){test("Added value 10",1);}else{test("Added value 10",0);}
  if(myList.sz==2){test("Size of 2",1);}else{test("Size of 2",0);}
  LL_add(&myList,4);//add 4   size should be 3 which is full
  if(myList.first->data==4 && myList.last->data==5){test("First/Last",1);}else{test("First/Last",0);}
  LL_add(&myList,2);//2 added    should be 2,4,10 now
  if(myList.sz==3){test("Size still 3",1);}else{test("Size still 3",0);}
  if(myList.first->data==2 && myList.last->data==10){test("OverFlow",1);}else{test("Overflow",0);}
  if(LL_sum(myList)==16){test("Sum of 16",1);}else{test("Sum of 16",0);}  
  while(1);
}

long LL_sum(LL_list list)
{
   long sum=0;
   node *current=list.first;
   sum=current->data;
   while(current->node_fw!=NULL)
   {
    current=current->node_fw;
    sum+=current->data;
   }
  return sum;
}

//add number to list, added to front, no order
void LL_add(LL_list *list,int num)
{
   node *new_node=(node*)malloc(sizeof(node));
  //new_list.first=new_node;
  new_node->data=num;
  new_node->node_fw=list->first;
  list->first=new_node;
  new_node->node_bk=NULL;
  new_node->node_fw->node_bk=new_node;
  list->sz++;
  //Serial.print("Adding Num: ");
  //Serial.println(list->first->data);
  if(list->max_sz!=0)//then there is a limit and need to check if full
  {
    if(list->sz>list->max_sz)//full, remove last node
    {
      list->last=list->last->node_bk;
      free(list->last->node_fw);//free the memory of removed node
      list->last->node_fw=NULL;
      list->sz--;
    }      
  }
}//End LL_add


void LL_print(LL_list list)
{
  struct node *current=list.first;
  Serial.println("******************");
  Serial.println("Printing List...");
  Serial.print("Size of List: ");
  Serial.println(list.sz);
  Serial.println(current->data);
  int cnt=0;
  while(current->node_fw!=NULL)
  {
    current=current->node_fw;
    cnt++;
    if(cnt>list.sz){Serial.println("Count ERROR");while(1);}
    Serial.println(current->data);
  }
  Serial.println("Done Printing list....\n******************\n\n");
}//End LL_print



/*
 * Implement linked list 
 * fill values at rate of 10 seconds, keeping 50?  500seconds   little over 8 min
 * this will be the range that the PID control is based on
 */

void PID()
{ //Calculate set based on List of temp values and set point 
  //need ability to Error out if unable to control temp (both high and low
  //Low is more likely and would likely signal the need for fuel 

  //ACTUALL READING
  //int cur=get_temp(0);
  int cur=temp_list.first->data+.1*(set_list.first->data-100);
  
  LL_add(&temp_list,cur);
  //LL_print(temp_list);
  err=set_temp-cur;
  Int=IntPrev+err;
  Der=err-errPrev;
  unsigned long dt=millis()-T;
  dt=dt/1000;
  set=Kp*err+(Ki*Int*dt)+((Kd*Der)/dt);
  if(set>200){set=200;}
  if(set<0){set=0;}
  LL_add(&set_list,set);
  errPrev=err;// reset variables for next reading
  IntPrev=Int;// reset variables for next reading
  //Serial.print("Test");
  Serial.print(cur);Serial.print(" ");Serial.print(set);Serial.print("\n");
}//End PID()

//Gets average temp reading over 10 readings from specific pin
//Should modify to throw out top and bottom
int get_temp(int probe_num)
{
  long probe_raw=0;
  double probe_temp;
  for(int i=0;i<100;i++)
  {
    probe_raw+=analogRead(probe_num);
    delay(10); 
  }
  probe_raw=probe_raw/100;
 
  probe_temp=(1/(double)probe_raw)*39000;
  //Serial.println(probe_raw);
  //Serial3.println(probe_raw);
  return (int)probe_temp; 
}//End get_temp

void get_volt()
{
  long volt_raw=0;
  double volt;
  for(int i=0;i<100;i++)
  {
    volt_raw+=analogRead(A1);
    delay(10); 
  }
  volt_raw=volt_raw/100;

  volt=(double)volt_raw*.00488;
  volt=volt*2;
  Serial.print("Voltage= ");
  Serial.println(volt);
  Serial3.print("Voltage= ");
  Serial3.println(volt);
}

void setup() {
 
  Serial.begin(9600); //Debug
  Serial3.begin(38400); //Output to Pi
  temp_list=LL_init(get_temp(0),50);  // Store the temp of smoker
  set_list=LL_init(0,50);   // Store the output set for fan/damper
  T=millis();
}

void loop() {
 delay(100);//Check if this is messing with millis() or not
 //get_temp(0);
 PID();
 //count++;
 //if(count>10){get_volt();count=0;} 
}
