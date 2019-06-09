#include <ESP8266WiFi.h>

WiFiClient client;

void telnetConnect() {
  Serial.println("Connecting telnet...");
  client.stop();
  int status = client.connect("ssd.jpl.nasa.gov", 6775);
  Serial.print("status: ");
  Serial.println(status); 
  if (status) {
    Serial.println("connected");
    client.println("?");
     client.println();
     delay(100);
      while (client.available() > 0)  Serial.print(client.read());
  } 
  else {
    Serial.println("connection failed");
  }
}

void setup(){
  Serial.begin(115200);
  WiFi.begin("gm", "cocacola");

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting wifi...");
  }
  delay(1000);
  telnetConnect();
}

void loop(){
  if (WiFi.status() == WL_CONNECTED){
    if (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }

      while (Serial.available() > 0) {
        char inChar = Serial.read();
        if (client.connected()) {
          client.print(inChar);
        }
      }
  
    }
    else {
      telnetConnect();

    }
    
  }
  delay(10000); //Send a request every 30 seconds
}
