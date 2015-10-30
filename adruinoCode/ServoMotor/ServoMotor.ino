#include <ax12.h>
#include <BioloidController.h>    // used for servos
//#include "poses.h"

BioloidController bioloid = BioloidController(1000000);

int temp = 0;
int input;
int id;
int pos;
int num = 0;
//int buff[5];
//int j=-1;

void setup(){
    pinMode(0, OUTPUT);
    
    // initialize variables
    id = 1;
    pos = 0;
    
    // open Serial Port
    Serial.begin(9600);
    delay (500);
    Serial.println("serial communication established");   // let the c++ code know when setup finished

    initializeServo();
}
 
void loop(){
  
  int incomingByte = Serial.read();
  int check;
  ax12SetRegister(1,7, 4095);
  ax12SetRegister(1,9, 4095);
  switch(incomingByte){
    case '1':
      //rotateServo();
      check = ax12GetRegister(1,6,2);
      Serial.print("register 6 ");
      Serial.println(check);
      check = ax12GetRegister(1,7,2);
      Serial.print("register 7");
      Serial.println(check);
      check = ax12GetRegister(1,8,2);
      Serial.print("register 8 ");
      Serial.println(check);
      check = ax12GetRegister(1,9,2);
      Serial.print("register 9 ");
      Serial.println(check);
      break;
  }
  
}

void initializeServo(){
  SetPosition(id, pos);
  delay(100);
}

void rotateServo(){
  id = 1;
//  for(int i = 0; i < 4099 ; i++){
//    ++pos;
//    Serial.println(pos);
    SetPosition(id, 4091);
    delay(20);
    int pos2 =  ax12GetRegister(id, 36, 2);
    if(pos2 != 4091){
      Serial.println("###");
      SetPosition(id, pos2 + 1);
      delay(30);
    }
    Serial.println(pos2);
//  }
}

/*
  if(Serial.available() > 0){
      
        incomingByte = Serial.read();
 
        if(incomingByte == 'r'){
            num = num+100;
            temp++;
            //j=-1;
            //Serial.println(num);
            Serial.println(temp);
            for(int i = 0; i < num; i++){
   //           SetPosition(1,num); //set the position of servo #1 to the current value of 'i'
              delay(10);//wait for servo to move
            }
        }
        else if(incomingByte == 'x'){
          Serial.println("exiting and setting motor position to 0");
          Serial.end();
          SetPosition(1,0);
          delay(10);
          
        }
    }*/
