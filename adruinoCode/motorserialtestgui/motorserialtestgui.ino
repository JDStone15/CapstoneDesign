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
int positionError;
void setup(){
  
  pinMode(0,OUTPUT);  
   
  //initialize variables 
  id = 1;
  pos = 0;
   
  //open serial port
  Serial.begin(9600);
  delay (500);   
  
  // let computer program know that the
  // serial port has been opened
  Serial.write(1);
  Serial.flush();
  
  // move the servo motor to the starting position
  MoveStartPos();
}

void loop(){

  // While there is data available in the serial port
  while (Serial.available()) {
    delay(10);  
    if (Serial.available() > 0) {
      char c = Serial.read();      // gets one byte from serial buffer
      readstring += c;             // truncates to readstring
    } 
  }
  
  // If we recieved data from the serial port
  if(readstring.length() > 0){
  
    char carray1[6];              // char array used to convert string to a number 
    readstring.toCharArray(carray1, sizeof(carray1)); 
    
    // Convert the char array to a number
    int goal = atoi(carray1);  
    
    // check to make sure goal is in the range 0-4095
    if(goal > 4095)
      goal = 4095;
    else if(goal < 0)
      goal = 0; 
    delay(10);
    
    // set the goal position of the servo motor
    MoveTest(goal);

    // Calculate the position error 
    positionError = currPosition - goal;
    if(positionError == 0)    
      positionError = 255;    // only sending a byte at a time so 255 is used for 0
    
    // write the position error 
    Serial.write(positionError);
    Serial.flush();
    
    // empty the read string
    readstring = "";
  }
  
}

void MoveStartPos(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Start);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
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
}


void MoveTest(int goal_pos){
  
  SetPosition(1, goal_pos);
  
  // wait for servo to reach goal position  
  for(int i = 0; i < 50; ++i){
    delay(10);
    currPosition = ax12GetRegister(id, 36, 2);
    if(currPosition == goal_pos)
      break;
  }

}



