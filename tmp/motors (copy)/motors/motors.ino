#include "AccelStepper.h"

#define STEPS_PER_DEGREE 11.3777777777


String cmd;

boolean newData = false;

// Define some steppers and the pins the will use
AccelStepper elevation(5, D0, D1, D2, D3);
AccelStepper azimuth(5, D5, D6, D7, D8);

void setup(){
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  azimuth.setMaxSpeed(900.0);
  azimuth.setAcceleration(1000.0);
  
  elevation.setMaxSpeed(900.0);
  elevation.setAcceleration(1000.0);
  
}

void loop(){
  while (Serial.available() > 0 ){
    cmd = Serial.readString();
  }
  
  for (int i=0;i<cmd.length();i++) {
    if (cmd[i]=='u') elevation.move(5*STEPS_PER_DEGREE);
    else if (cmd[i]=='d') elevation.move(-5*STEPS_PER_DEGREE);
    else if (cmd[i]=='r') azimuth.move(5*STEPS_PER_DEGREE);
    else if (cmd[i]=='l') azimuth.move(-5*STEPS_PER_DEGREE);
    else if (cmd[i]=='x') {
      azimuth.setCurrentPosition(0);
      elevation.setCurrentPosition(0);
      azimuth.moveTo(0);
      elevation.moveTo(0);

    }
  }
  cmd = "";
  while (elevation.distanceToGo()!=0 || azimuth.distanceToGo()!=0) {
    elevation.run();
    azimuth.run();
  }

}
