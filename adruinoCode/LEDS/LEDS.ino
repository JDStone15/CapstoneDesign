#include <ax12.h>
// testing
int led = 0;
int incomingByte = 0;
int off_on = 0;
int temp;
int sendByte = 1;
void setup(){
    Serial.begin(9600);
    pinMode(led,OUTPUT);
    delay(10);
    Serial.write(sendByte);
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {
    
  if(Serial.available() > 0){

    incomingByte = Serial.read();
    if(incomingByte == '1'){
      if(off_on == 0){
        off_on = 1;
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(10);
        Serial.write('y');
        //temp = Serial.read();
        
      }
      else{
        off_on = 0;
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW\
        delay(10);
        Serial.write('x');
        //temp = Serial.read();
      
      }
    }
    
    if(incomingByte == '0'){
      Serial.println("exiting");
      digitalWrite(led, LOW);
      delay(1000);
      resetFunc();
      delay(100);
      Serial.println("never happens");
    }
  }
}

