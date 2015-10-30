#include <ax12.h>
// testing
int led = 0;
int input;
int temp = 0;
int num = 0;

void setup(){
    Serial.begin(9600);
    SetPosition(1,0); //set the position of servo # 1 to '0'
    delay(100);//wait for servo to move
}
 

 
void loop(){
   
    if(Serial.available()>0){
        input=Serial.read();
 
        if(input==','){
            num = num+10;
            //j=-1;
            Serial.println(num);
            SetPosition(1,num); //set the position of servo #1 to the current value of 'i'
            delay(10);//wait for servo to move
        }
        else if(input=='x'){
          Serial.println("exiting and setting motor position to 0");
          Serial.end();
          SetPosition(1,0);
          delay(10);
        }
    }
}
