#ifndef NTP_H
#define NTP_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void ntpInit();
unsigned long getCurrentTime();
void sendNTPpacket(IPAddress& address);
double epochToJulian(double epoch);

#endif /* NTP_H */
