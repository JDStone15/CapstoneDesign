#include <ax12.h>
// testing
int led = 0;
int incomingByte = 0;
int off_on = 0;
int temp;
int sendByte = 1;
String truncate;
String inString;
String sending;

String readstring;

int lowbyte;
int highbyte;

void setup()
{
    Serial.begin(9600);
    pinMode(led,OUTPUT);
    delay(10);
    Serial.write(sendByte);
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {
    
 while (Serial.available()) {
   delay(10);  
   if (Serial.available() > 0) {
     char c = Serial.read();  //gets one byte from serial buffer
     readstring += c; //makes the string readString
   } 
 }
  if (readstring.length() >0) {
   // Serial.println(readstring);
     
     int n1; //declare as number  
     int n2;
     
     char carray1[6]; //magic needed to convert string to a number 
     readstring.toCharArray(carray1, sizeof(carray1));
     
     n1 = atoi(carray1);      
     if(n1 == 0)
       n1 = 255;

     Serial.write(n1);
     readstring="";
 } 

}
 


