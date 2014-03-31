#include <SoftwareSerial.h>
SoftwareSerial Color90(2, 3); // rx=2, tx=3


int buttonState;
float red;
float grn;
float blu;
float Kr;
float Kg;
float Kb;
float Wr;
float Wg;
float Wb;
float redAdj;
float grnAdj;
float bluAdj;

int gotcolor = 0;
int letter;

void setup()
{
  Serial.begin(9600); // Start communication with serial port read value
  Color90.begin(4800); // Send signal to led to read value


  pinMode(5,INPUT); // Pin that checks if button is pressed down
  pinMode(2,INPUT); // serial pin out from color pal
  pinMode(3,INPUT); // signal pulls up, sends, pulls down, reads
  digitalWrite(2,HIGH); // Enable the pull-up resistor
  digitalWrite(3,HIGH); // Enable the pull-up resistor


  pinMode(2,OUTPUT); // send signal out
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW); // turn pin off so pin 3 can go high
  digitalWrite(3,LOW);


  pinMode(2,INPUT); // Input signal to print
  pinMode(3,INPUT);
  
  Serial.println("Pass 1");
//  delay(20);


  while( digitalRead(2) != HIGH || digitalRead(3) != HIGH ) {
    Serial.println("In the loop");
    delay(50);
  }

  Serial.println("Pass 2");

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  delay(100); // spec is 80, but not all ColorPAL units work with 80


  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  delay(100); 
  
  //TAKE IN THE VALUE FOR WHITE REFERENCE
  Serial.println("Press Button for White Reference");
  buttonState=0;
  while (buttonState==0){
  buttonState=digitalRead(5);}
  readcolor();
  Wr=red;
  Wg=grn;
  Wb=blu;
  gotcolor=0;
  Serial.println(Wr);
  Serial.println(Wg);
  Serial.println(Wb);
  delay(1000);
  
  //TAKE IN THE VALUE FOR BLACK REFERENCE, push a button 
  Serial.println("Position Black Reference and push button");
  buttonState=0;
  while (buttonState==0){
  buttonState=digitalRead(5);}
  readcolor();
  Kr=red;
  Kg=grn;
  Kb=blu;
  gotcolor=0;
  Serial.println(Kr);
  Serial.println(Kg);
  Serial.println(Kb);
  delay(1000);
}


// This oscillates back and forth on one wire to turn off led, send signal,
// turn on led, read signal. very fast strobe read - arduino is not capable of
// one wire signal communication over digital ports, so this is a way around
// that over 2 wires communicating with 1 pin on the sensor.
//---------------------------------


void loop()
{
//  readcolor();
//  Serial.print("R");
//  Serial.print(red);
//  Serial.print("   G");
//  Serial.print(grn);
//  Serial.print("   B");
//  Serial.println(blu); 
//  gotcolor = 0;
//  delay(100);
  
  Serial.println("Push button to read color");
  buttonState=0;
  while (buttonState==0){
  buttonState=digitalRead(5);}
  readcolor();
  redAdj=(red-Kr)/(Wr-Kr)*(float)255;
  grnAdj=(grn-Kg)/(Wg-Kg)*(float)255;
  bluAdj=(blu-Kb)/(Wb-Kb)*(float)255;
  
  
 //White or black??

int colorSum = (redAdj + bluAdj +grnAdj);
if (colorSum <=-50)
  {Serial.println("Blank");}
  
  else if (colorSum >250)
    {Serial.println("The object is predominantly White");}
    
  else if (colorSum >-50 && colorSum <=4)
    {Serial.println("The object is predominantly Black");}
    
  else if (colorSum >4 && colorSum <=250 && redAdj>grnAdj && redAdj>bluAdj)
    {Serial.println("The object is predominantly Red");}
    
  else if (colorSum >4 && colorSum <=250 && grnAdj>bluAdj && grnAdj>redAdj)
   {Serial.println("The object is predominantly Green");}
  else
   {Serial.println("The object is predominantly Blue");}
  gotcolor = 0;
  delay(500);
  Serial.println();
//try to get yellow, purple
}

void readcolor() { // Reads ColorPAL, putting results in the red,grn,blu variables

  char rByte[9];
  char dummy[4];

  delay(20);
 Color90.begin(4800);
 Color90.print("= (00 $ m) !"); // set up loop to continuously send color data
  pinMode(3,INPUT);
  gotcolor = 0;
  while (gotcolor == 0) {
    rByte[0] = Color90.read();
    if( rByte[0] == '$' ) {
      gotcolor = 1;
      for(int i=0; i<9; i++) {
        rByte[i] = Color90.read();
        //Serial.print(rByte[i]);
      }
      //Serial.println("");
      dummy[0] = rByte[0];
      dummy[1] = rByte[1];
      dummy[2] = rByte[2];
      dummy[3] = 0;

      red = strtol(dummy,NULL,16);

      dummy[0] = rByte[3];
      dummy[1] = rByte[4];
      dummy[2] = rByte[5];

      grn = strtol(dummy,NULL,16);

      dummy[0] = rByte[6];
      dummy[1] = rByte[7];
      dummy[2] = rByte[8];

      blu = strtol(dummy,NULL,16); 
    }
  }
}
