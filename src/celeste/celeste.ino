#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "ntp.h"
//#include "gimbal.h"
#include "AccelStepper.h"
#include "horizons.h"

#define STEPS_PER_DEGREE 11.3777777777

#ifndef STASSID
#define STASSID "gm"
#define STAPSK  "cocacola"
#endif

String cmd;
position target;

AccelStepper elevation(5, D0, D1, D2, D3);
AccelStepper azimuth(5, D5, D6, D7, D8);

const char* ssid = STASSID;
const char* password = STAPSK;

double startupTime;
long startupMillis;

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setCurrentTime() {
 ntpInit();
 startupTime = getCurrentTime();
 startupMillis = millis();
 Serial.print("Current time: ");
 Serial.println(startupTime);
 Serial.print("Current millis: ");
 Serial.println(startupMillis);
}

void setupSteppers() {
  azimuth.setMaxSpeed(1500.0);
  azimuth.setAcceleration(900.0);
  
  elevation.setMaxSpeed(1500.0);
  elevation.setAcceleration(900.0);
}

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  setupSteppers();
  connectWiFi();
  setCurrentTime();

  Serial.println("Started");
  delay(100);
   
}

void loop() {
  cmd = "";
  bool isCommand = false;

  while (Serial.available() > 0 ){
    cmd = Serial.readString();
  }

  if (cmd.length()>0) {
    float elevationTarget = 0;
    float azimuthTarget = 0;
    for (int i=0;i<cmd.length();i++) {
      isCommand = true;
      switch (cmd[i]) {
        case 'u':
          elevationTarget-=5*STEPS_PER_DEGREE;
          break;
        case 'U':
          elevationTarget-=30*STEPS_PER_DEGREE;
          break;
        case 'd':
          elevationTarget+=5*STEPS_PER_DEGREE;
          break;
        case 'D':
          elevationTarget+=30*STEPS_PER_DEGREE;
          break;
        case 'r':
          azimuthTarget+=5*STEPS_PER_DEGREE;
          break;
        case 'R':
          azimuthTarget+=30*STEPS_PER_DEGREE;
          break;
        case 'l':
          azimuthTarget-=5*STEPS_PER_DEGREE;
          break;
        case 'L':
          azimuthTarget-=30*STEPS_PER_DEGREE;
          break;
        case 'x':
          elevation.setCurrentPosition(0);
          azimuth.setCurrentPosition(0);
          //elevationTarget = 0;
          //elevation.moveTo(0);
          //azimuth.moveTo(0);
          break;
        default:
          isCommand = false;
      }
      if (!isCommand) break;
    }
  
    if (!isCommand) {
      long jdNow = (float)epochToJulian((float)(millis()-startupMillis)/1000+startupTime);
      target = getAzimuthAndElevation(cmd, jdNow);
      if (target.azimuth>180) target.azimuth-=360;
      azimuth.moveTo(target.azimuth*STEPS_PER_DEGREE);
      elevation.moveTo(-target.elevation*STEPS_PER_DEGREE);
      Serial.print("Moving to azimuth ");
      Serial.print(target.azimuth);
      Serial.print(" elevation: ");
      Serial.println(target.elevation);
    }
    else {
      azimuth.move(azimuthTarget);
      elevation.move(elevationTarget);
//    elevation.setCurrentPosition(0);
//    azimuth.setCurrentPosition(0);
    }
  }
  
  //while (azimuth.distanceToGo()!=0 || elevation.distanceToGo()!=0) {
    azimuth.run();
    elevation.run();
  //}
  
}
