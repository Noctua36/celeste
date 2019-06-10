#include "horizons.h"

const char* host = "ssd.jpl.nasa.gov";
const int httpsPort = 443;

// SHA1 fingerprint for ssd.jpl.nasa.gov
const char fingerprint[] PROGMEM = "C6 5D 37 E8 98 69 12 6A 0B 85 B4 53 A9 4B C9 38 45 6A 00 2D";

WiFiClientSecure client;

position getAzimuthAndElevation(String id, float julianDate) {
  position pos;
  pos.azimuth = 0;
  pos.elevation = 0;
  client.setFingerprint(fingerprint);
  client.setTimeout(API_TIMEOUT);

  Serial.print("connecting to ");
  Serial.println(host);
  Serial.printf("Using fingerprint '%s'\n", fingerprint);  

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return pos;
  }

  if (client.verify(fingerprint,host)) {
    Serial.println("certificate matches");
  }
  else {
    Serial.println("certificate doesn't match");
  }

  String url = "/horizons_batch.cgi?batch=1&TABLE_TYPE=OBSERVER&QUANTITIES='4'&COMMAND=\"";
  url += id;
  url += "\"&SOLAR_ELONG=\"0,180\"&LHA_CUTOFF=0&CSV_FORMAT=YES&CAL_FORMAT=CAL&ANG_FORMAT=DEG&APPARENT=REFRACTED&REF_SYSTEM=J2000&CENTER=coord@399&COORD_TYPE=GEODETIC&SITE_COORD='-44.016950,-19.875750,0.818000'&TLIST=";
  url += String(julianDate);
  url += "&SKIP_DAYLT=NO";
  
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               //"Cache-Control: no-cache\r\n" +
               //"User-Agent: BuildFailureDetectorESP8266\r\n" +
               //"accept-encoding: gzip, deflate" +
               //"cache-control: no-cache" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
 
  Serial.println("reply was:");
  Serial.println("==========");
  String line, data;
  while (client.available() && line != "$$SOE"){        
    line = client.readStringUntil('\n');  //Read Line by Line
    //Serial.println(line); //Print response
  }
  while (client.available()){
    line = client.readStringUntil('\n');  //Read Line by Line
    if (line == "$$EOE") break;
    data += line;  
  }
  client.stop();
  
  int commas = 0;
  for (int i = 0; i < data.length(); i++) {
    if (data.substring(i, i+1) == ",") {
      commas++;
      if (commas == 3) {
        pos.azimuth = data.substring(i+1).toFloat();
      }
      if (commas == 4) {
        pos.elevation = data.substring(i+1).toFloat();
        break;
      }
    }
  }
  
  Serial.print("Azimuth: ");
  Serial.println(pos.azimuth);
  Serial.print("Elevation: ");
  Serial.println(pos.elevation);

  Serial.println("==========");
  
  return pos;
}
