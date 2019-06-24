#ifndef HORIZONS_H
#define HORIZONS_H

#include "Arduino.h"
#include <ESP8266WiFi.h>


#define API_TIMEOUT 10000

typedef struct {
  float elevation;
  float azimuth;
} position;


position getAzimuthAndElevation(String id, double latitude, double longitude, double altitude, double julianDate);

#endif /* HORIZONS_H */
