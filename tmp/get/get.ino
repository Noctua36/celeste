#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "gm";
const char* password = "cocacola";
 
void setup () {
 
Serial.begin(115200);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
Serial.print("Connecting..");
 
}
 
}
 
void loop() {
 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
HTTPClient http;  //Declare an object of class HTTPClient
 
http.begin("https://ssd.jpl.nasa.gov/horizons_batch.cgi?batch=1&TABLE_TYPE=OBSERVER&QUANTITIES='4'&COMMAND=\"301\"&SOLAR_ELONG=\"0,180\"&LHA_CUTOFF=0&CSV_FORMAT=YES&CAL_FORMAT=JD&ANG_FORMAT=DEG&APPARENT=REFRACTED&REF_SYSTEM=J2000&CENTER=coord@399&COORD_TYPE=GEODETIC&SITE_COORD='-44.016950,-19.875750,0.818000'&TLIST=2458644.36954&SKIP_DAYLT=NO", "C6 5D 37 E8 98 69 12 6A 0B 85 B4 53 A9 4B C9 38 45 6A 00 2D");  //Specify request destination
//http.addHeader("Content-Type", "application/x-www-form-urlencoded");
int httpCode = http.GET();                                                                  //Send the request
 
if (httpCode > 0) {
      http.writeToStream(&Serial);

      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] ... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] ... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}
 
delay(30000);    //Send a request every 30 seconds
 
}
