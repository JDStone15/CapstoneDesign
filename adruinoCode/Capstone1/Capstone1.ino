#include <ax12.h>

int led = 0;
int sendByte = 1;
int servoID = 1;
int currPosition, positionError;
int pos;

String readstring;


void setup(){
    Serial.begin(9600);
    pinMode(led,OUTPUT);
    delay(10);
    

    
    // Write to USB port to let computer know serial communication established
    Serial.write(sendByte);
    delay(10);
        // Set the position of servoID to 0
    SetPosition(1, 0); 
    delay(500);                //wait for servo to move
}

void loop() {
    
  // While there is data available in the USB port
  while (Serial.available()) {
    delay(10);  
    if (Serial.available() > 0) {
      char c = Serial.read();      // gets one byte from serial buffer
      readstring += c;             // truncates to readstring
    } 
  }
  
  // If there is data stored in readstring
  if (readstring.length() > 0) {
     
    char carray1[6];              // char array used to convert string to a number 
    readstring.toCharArray(carray1, sizeof(carray1));
     
    // Convert the char array to a number
    pos = atoi(carray1);      
    delay(100);
    
    // Set the position of the servo motor
    SetPosition(1, pos);
    delay(100);
    
    // Try to reach the desired servo motor position
    for(int i = 0; i < 2; ++i){
      currPosition = GetPosition(1);
      delay(100);
      if(currPosition == pos)
        i = 2;
      //delay(100);
    }
    
    // Calculate the position error 
    positionError = pos - currPosition;
    if(positionError == 0)    
      positionError = 255;    // only sending a byte at a time so 255 is used for 0
      
    // Write the position error to the USB  
    Serial.write(positionError);
    
    // Delete readstring data
    readstring="";
  } 

}
