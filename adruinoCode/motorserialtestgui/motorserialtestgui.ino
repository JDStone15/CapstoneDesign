#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"

BioloidController bioloid = BioloidController(1000000);


int id;
int pos;
int goal;
boolean IDCheck;
boolean RunCheck;
int currPosition;
String readstring;

void setup(){
   pinMode(0,OUTPUT);  
   
   //initialize variables 
   id = 1;
   pos = 0;
   IDCheck = 1;
   RunCheck = 0;
  //open serial port
   Serial.begin(9600);
   delay (500);   
   Serial.println("###########################");    
   Serial.println("Serial Communication Established.");    

  MoveStartPos();
}

void loop(){
  // read the sensor:
    // While there is data available in the USB port
/*  while (Serial.available()) {
    delay(10);  
    if (Serial.available() > 0) {
      char c = Serial.read();      // gets one byte from serial buffer
      readstring += c;             // truncates to readstring
    } 
  }*/
  //if(readstring.length() > 0){
  if(Serial.available()){
  /*
    char carray1[6];              // char array used to convert string to a number 
    readstring.toCharArray(carray1, sizeof(carray1)); 
    // Convert the char array to a number
    int goal = atoi(carray1);   
      */
      Serial.read();
      for(int i = 0; i < 163; ++i){
        goal = i *10;
      delay(10);
      Serial.println("-----------");
      Serial.print("goal pos : ");
      Serial.println(goal);
      MoveTest(goal);
      Serial.print("curr pos: ");
      Serial.println(currPosition);
      Serial.println("-----------");
      
      //readstring = "";
    }
  }
}

void MoveStartPos(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Start);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    Serial.println("###########################");
    Serial.println("Moving servos to start position");
    Serial.println("###########################");    
    delay(1000);
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
    currPosition = ax12GetRegister(id, 36, 2);
    if(pos != 0){
      SetPosition(id, 1);
      delay(10);
    }
    currPosition = ax12GetRegister(id, 36, 2);
    Serial.print("Start position: ");
    Serial.println(currPosition);
}


void MoveTest(int goal_pos){

  SetPosition(1, goal_pos);
  
  for(int i = 0; i < 50; ++i){
    delay(10);
    currPosition = ax12GetRegister(id, 36, 2);
    if(currPosition == goal_pos)
      break;
  }

}



