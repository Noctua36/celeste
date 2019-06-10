#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>


#ifndef STASSID
#define STASSID "gm"
#define STAPSK  "cocacola"
#endif

#define API_TIMEOUT 10000


const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "ssd.jpl.nasa.gov";
const int httpsPort = 443;

// SHA1 fingerprint for ssd.jpl.nasa.gov
const char fingerprint[] PROGMEM = "C6 5D 37 E8 98 69 12 6A 0B 85 B4 53 A9 4B C9 38 45 6A 00 2D";


WiFiClientSecure client;


unsigned int localPort = 2390; // local port to listen for UDP packets

IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //b uffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void ntpInit() {
  udp.begin(localPort);
}

//returns epoch format
unsigned long getCurrentTime() {
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb;
  Serial.print("getting current time by NTP");
  do {
   cb = udp.parsePacket();
   delay(500);
   Serial.print('.');
  } while (!cb);
  Serial.println();
  
  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  // the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;

  return epoch;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

double epochToJulian(long epoch) {
  return (epoch/86400.0)+2440587.5;
}




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
  
  client.setFingerprint(fingerprint);
  client.setTimeout(API_TIMEOUT);
  delay(100);
  
  
  /*Serial.print("connecting to ");
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

  String url = "/horizons_batch.cgi?batch=1&TABLE_TYPE=OBSERVER&QUANTITIES='4'&COMMAND=\"301\"&SOLAR_ELONG=\"0,180\"&LHA_CUTOFF=0&CSV_FORMAT=YES&CAL_FORMAT=CAL&ANG_FORMAT=DEG&APPARENT=REFRACTED&REF_SYSTEM=J2000&CENTER=coord@399&COORD_TYPE=GEODETIC&SITE_COORD='-44.016950,-19.875750,0.818000'&TLIST=2458644.36954&SKIP_DAYLT=NO";
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
  String line;
  while(client.available() && line != "$$SOE"){        
    line = client.readStringUntil('\n');  //Read Line by Line
    //Serial.println(line); //Print response
  }
  line = client.readStringUntil('\n');  //Read Line by Line
  client.stop();
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  */
}

void loop() {
}
