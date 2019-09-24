#include <Arduino.h>
#include "kehopsboard.h"

Kehops kehops(13);
bool btnLoneShot = 0;
bool btnRoneShot = 0;

unsigned char i=0;
unsigned char pwmOutput =9;
char text[50];


void setup() {
  Serial.begin(9600);
  kehops.begin();
  
  kehops.setLED(LED0,0);
  kehops.setLED(LED1,0);
  kehops.setLED(LED2,0);
  kehops.setPWM(7,0);
  
}


void loop() {

 int btnR = kehops.getButton(RIGHT);
 int btnL = kehops.getButton(LEFT);


  if(btnR){
    if(!btnRoneShot){
      sprintf(text, "---- BTN Right: %d\n",btnR);
      Serial.write(text);      
      kehops.setMotorAction(RIGHT,100);
      kehops.stepperRotation(-20, 640);
      btnRoneShot =1;
    }
  }
  else{
    if(btnRoneShot){
      sprintf(text, "---- BTN Right: %d\n",btnR);
      Serial.write(text);      
      kehops.setMotorAction(RIGHT,0);
    }
    btnRoneShot=0;
  }


  if(btnL){
    if(!btnLoneShot){
      sprintf(text, "---- BTN Left: %d\n",btnL);
      Serial.write(text);
      kehops.setMotorAction(LEFT, 100);
      kehops.stepperRotation(20, 640);
      btnLoneShot =1;
    }
  }
  else{
    if(btnLoneShot){
      sprintf(text, "---- BTN Left: %d\n",btnL);
      Serial.write(text);
        kehops.setMotorAction(LEFT,0);
    }
    btnLoneShot=0;
  }

  int mV = kehops.getVoltage(0);
  int stepperState = kehops.getStepperState();
  sprintf(text, "Voltage value: %d  -- ", mV);
  Serial.write(text);
  Serial.write("  StepperState: 0x");
  Serial.println(stepperState, HEX);

  int distanceCm = kehops.getSonarDistance();
  sprintf(text, "Sonar distance : %d  -- ", distanceCm);
  Serial.write(text);

  int sensor = kehops.getSensor(0);
  sprintf(text, "Sensor : %d  -- ", sensor);
  Serial.write(text);


  if(sensor)
    kehops.setPWM(12,90);
    else kehops.setPWM(12,0);

  if(stepperState){
    kehops.setPWM(7,90);
    kehops.setLED(LED2,90);
  }
    else{
      kehops.setPWM(7,0);
      kehops.setLED(LED2,0);
    }



//---------------------------
  if(i>=100){
    i=0;
  }
  else {
    i+=10;
  }

    kehops.setLED(LED0,i);
    kehops.setPWM(PWM1,i);

  
  delay(500);
}
