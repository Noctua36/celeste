#include "libs/AccelStepper.h"

// Define some steppers and the pins the will use
AccelStepper stepper1(5, D0, D1, D2, D3);
AccelStepper stepper2(5, D5, D6, D7, D8);

void setup(){
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  stepper1.setMaxSpeed(900.0);
  stepper1.setAcceleration(700.0);
  
  stepper2.setMaxSpeed(900.0);
  stepper2.setAcceleration(700.0);
}

void loop(){    
  if (stepper1.distanceToGo() == 0){
    stepper1.moveTo(random(-2048, 2048));//just an easy way to get the motors to move to random positions
  } 
  stepper1.run();

  if (stepper2.distanceToGo() == 0){
    stepper2.moveTo(random(-2048, 2048));//just an easy way to get the motors to move to random positions
  } 
  stepper2.run();
}