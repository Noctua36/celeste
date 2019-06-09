/*
    HTTP over TLS (HTTPS) example sketch
    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.
    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#ifndef STASSID
#define STASSID "gm"
#define STAPSK  "cocacola"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "ssd.jpl.nasa.gov";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "C6 5D 37 E8 98 69 12 6A 0B 85 B4 53 A9 4B C9 38 45 6A 00 2D";

void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  const int API_TIMEOUT = 10000;  //keep it long if you want to receive headers from client
  //client.setInsecure();

client.setFingerprint(fingerprint);
client.setTimeout(API_TIMEOUT);
delay(500);
  Serial.print("connecting to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint,host)) {
    Serial.println("certificate matches");
  }
  else {
    Serial.println("certificate doesn't match");
  }

  String url = "/horizons_batch.cgi?batch=1&TABLE_TYPE=OBSERVER&QUANTITIES='4'&COMMAND=\"301\"&SOLAR_ELONG=\"0,180\"&LHA_CUTOFF=0&CSV_FORMAT=YES&CAL_FORMAT=JD&ANG_FORMAT=DEG&APPARENT=REFRACTED&REF_SYSTEM=J2000&CENTER=coord@399&COORD_TYPE=GEODETIC&SITE_COORD='-44.016950,-19.875750,0.818000'&TLIST=2458644.36954&SKIP_DAYLT=NO";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               //"Accept: */*\r\n" +
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
  String line;
  while(client.available()){        
    line = client.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}
