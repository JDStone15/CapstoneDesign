#include <ax12.h>
int firmware;
int resolution;
int CW;
int CCW;
//int ID = 1;  // ID used by microcontroller for rotating servo
int check; 
int i = 500;
int servoID;   // ID of actual servo
int incomingByte;

void setup(){

  // Initialize Serial Communication
  Serial.begin(9600);
  delay(500);   
  Serial.println("serial communication established."); 
  
  
  // Set Position of MX-12W to 2048
  SetPosition(1,500);
  delay(500);
  ax12SetRegister(1, 26, 4);
  delay(100);
  check = ax12GetRegister(1,  26, 1);
  delay(100);
  ax12SetRegister(1, 27, 4);
  delay(100);
  Serial.println(check);
  
  check = ax12GetRegister(1, 27, 1);
  delay(100);
  Serial.println(check);
 
}

//void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop(){
    // Perform all checks
    if(Serial.available() > 0){
  int incomingByte = Serial.read();
  switch(incomingByte){
    case '1':
    Serial.print("voltage = ");
    check = ax12GetRegister(1, 42, 1);
    delay(1000);
    Serial.println(check);
     Serial.print("i = ");
     Serial.println(i);
     SetPosition(1, i);
     delay(100);
     while(GetPosition(1) != i){
       Serial.println(GetPosition(1));
       delay(1000);
     }
     i++;
      Serial.print("Current Position = ");
      Serial.println(GetPosition(1));
      break;
    }
    }
}

/*
void checkID(){
  servoID =  ax12GetRegister(1, 3, 2);
  delay(10);
  Serial.print("Servo ID: ");
  delay(10);
  Serial.println(servoID);
  delay(10);
}

void checkanglelimit(){
  ax12SetRegister2(257, 6, 0);
  delay(10);
  CW = ax12GetRegister(1, 6, 2);
  delay(10);
  CCW =  ax12GetRegister(1, 8, 2);
  delay(10);
  Serial.print("Servo CW: ");
  Serial.println(CW);
  Serial.print("Servo CCW: ");
  Serial.println(CCW);
  delay(100);
}

void checkresolution(){
  ax12SetRegister(257, 22, 1);
  delay(10);
  resolution =  ax12GetRegister(257, 22, 2);
  delay(10);
  Serial.print("Servo resolution: ");
  Serial.println(resolution);
  delay(100);
}

void checkanglelimit(){
  //ax12SetRegister2(227, 6, 4095);
  delay(10);
  CW = ax12GetRegister(1, 6, 2);
  delay(10);
  CCW =  ax12GetRegister(1, 8, 2);
  delay(10);
  Serial.print("Servo CW: ");
  Serial.println(CW);
  Serial.print("Servo CCW: ");
  Serial.println(CCW);
  delay(100);
}

void servoScan(){
  for(int i = 0; i < 4095; ++i){
    SetPosition(ID, i);
    delay(10);
  }
  for(int i = 4095; i > 0; --i){
    SetPosition(ID, i);
    delay(10);
  }
}*/
