#include "AccelStepper.h"

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

long elevationTargetPosition;
long azimuthTargetPosition;

void loop(){
  while (Serial.available() > 0 ){
    cmd = Serial.readString();
  }
  
  elevationTargetPosition = elevation.targetPosition();
  azimuthTargetPosition = azimuth.targetPosition();
  
  for (int i=0;i<cmd.length();i++) {Serial.println(7);
    if (cmd[i]=='u') elevationTargetPosition += 56.88889;
    else if (cmd[i]=='d') elevationTargetPosition -=56.88889;
    else if (cmd[i]=='r') azimuthTargetPosition +=56.88889;
    else if (cmd[i]=='l') azimuthTargetPosition -=56.88889;
  }
  elevation.moveTo(elevationTargetPosition);
  azimuth.moveTo(azimuthTargetPosition);
  cmd = "";
  while (elevation.distanceToGo()!=0 || azimuth.distanceToGo()!=0) {
    elevation.run();
    azimuth.run();
  }

  
//    
//    {
//    Serial.print("u");
//    if (elevation.distanceToGo() == 0){
//      elevation.move(56.89);
//    }
//  } else if (cmd.equals("d")) {
//    Serial.print("d");
//    if (elevation.distanceToGo() == 0){
//      elevation.move(-56.89);
//    }
//  } else if (cmd.equals("r")) {
//    Serial.print("r");
//    if (azimuth.distanceToGo() == 0){
//      azimuth.move(56.89);
//    }
//  } else if (cmd.equals("l")) {
//    Serial.print("l");
//    if (azimuth.distanceToGo() == 0){
//      azimuth.move(-56.89);
//    }
//  }
//  }
//
//  
////  Serial.print("Command received: ");
// if (!cmd.equals("")) Serial.println(cmd);
//  
//  cmd = "";
  azimuth.run();
  elevation.run();
//  delay(100);
}
