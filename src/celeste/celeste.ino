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

  if (cmd.length()>0) {
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
        gimbal.azimuthTo(target.azimuth);
        gimbal.elevationTo(target.elevation);
        Serial.print("Moving to azimuth ");
        Serial.print(target.azimuth);
        Serial.print(" elevation: ");
        Serial.println(target.elevation);
    }
  }
  gimbal.run();
}
