#include "libs/AccelStepper.h"

// Define some steppers and the pins the will use
AccelStepper elevation(5, D0, D1, D2, D3);
AccelStepper azimuth(5, D5, D6, D7, D8);

void setup(){
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  azimuth.setMaxSpeed(900.0);
  azimuth.setAcceleration(700.0);
  
  elevation.setMaxSpeed(900.0);
  elevation.setAcceleration(700.0);

  
}

void loop(){    
  if (azimuth.distanceToGo() == 0){
    azimuth.moveTo(random(-2048, 2048));
  } 
  azimuth.run();

  if (elevation.distanceToGo() == 0){
    elevation.moveTo(random(-2048, 2048));
  } 
  elevation.run();
}