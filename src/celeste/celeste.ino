#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ntp.h"
#include "gimbal.h"
#include "horizons.h"

#ifndef STASSID
#define STASSID "gm"
#define STAPSK  "cocacola"
#endif

String cmd;
position target;

boolean newData = false;

const char* ssid = STASSID;
const char* password = STAPSK;

Gimbal gimbal(D0, D1, D2, D3, D5, D6, D7, D8, 900.0, 1000.0);

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

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

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
  
  for (int i=0;i<cmd.length();i++) {
    isCommand = true;
    switch (cmd[i]) {
      case 'u':
        gimbal.moveUp(5);
        break;
      case 'd':
        gimbal.moveDown(5);
        break;
      case 'r':
        gimbal.moveRight(5);
        break;
      case 'l':
        gimbal.moveLeft(5);
        break;
      case 'x':
        gimbal.setOrigin();
        break;
      default:
        isCommand = false;
    }
    if (!isCommand) break;
  }

  if (!isCommand) {
    long jdNow = (float)epochToJulian((float)(millis()-startupMillis)/1000+startupTime);
      target = getAzimuthAndElevation(cmd, jdNow);
  }

  gimbal.run();






/*
  if (cmd != ""){
    switch (cmd[0]) {
    case 'u':
    case 'd':
    case 'l':
    case 'r':
    case 'x':
      
        switch (cmd[i]) {
        case 'd':
          
          break;
        
        default:
          break;
        }
        if (cmd[i]=='u') gimbal.moveUp(5);
        else if (cmd[i]=='d') gimbal.moveDown(5);
        else if (cmd[i]=='r') gimbal.moveRight(5);
        else if (cmd[i]=='l') gimbal.moveLeft(5);
        else if (cmd[i]=='x') gimbal.setOrigin();
      }
      break;
    
    default:
      break;
    }


    if (cmd[0]=='s') {
      for (int i=0;i<cmd.length();i++) {
        if (cmd[i]=='u') gimbal.moveUp(5);
        else if (cmd[i]=='d') gimbal.moveDown(5);
        else if (cmd[i]=='r') gimbal.moveRight(5);
        else if (cmd[i]=='l') gimbal.moveLeft(5);
        else if (cmd[i]=='x') gimbal.setOrigin();
      }


      elevationTargetPosition = elevation.targetPosition();
      azimuthTargetPosition = azimuth.targetPosition();
      
      for (int i=0;i<cmd.length();i++) {
        if (cmd[i]=='u') elevationTargetPosition += 56.88889;
        else if (cmd[i]=='d') elevationTargetPosition -=56.88889;
        else if (cmd[i]=='r') azimuthTargetPosition +=56.88889;
        else if (cmd[i]=='l') azimuthTargetPosition -=56.88889;
        else if (cmd[i]=='x') {
          azimuth.setCurrentPosition(0);
          elevation.setCurrentPosition(0);
          azimuth.moveTo(0);
          elevation.moveTo(0);
        }
      }

      elevation.moveTo(elevationTargetPosition);
      azimuth.moveTo(azimuthTargetPosition);
    
      while (elevation.distanceToGo()!=0 || azimuth.distanceToGo()!=0) {
        elevation.run();
        azimuth.run();
      }
    }
    else {
      long jdNow = (float)epochToJulian((float)(millis()-startupMillis)/1000+startupTime);
      target = getAzimuthAndElevation(cmd, jdNow);
    }
    
  }
  cmd = "";
*/
}
