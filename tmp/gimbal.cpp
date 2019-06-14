#include "gimbal.h"

// void Gimbal::run() {
//   elevation.moveTo(elevationTargetPosition);
//   azimuth.moveTo(azimuthTargetPosition);
//   while (elevation.distanceToGo()!=0 || azimuth.distanceToGo()!=0) {
//     //Serial.print('.');
//     elevation.run();
//     azimuth.run();
//   }
// }

// void Gimbal::moveUp(uint deg) {
//   elevationTargetPosition+=STEPS_PER_DEGREE*deg;
//   Serial.print("Elevation Target Position: ");
//   Serial.println(elevationTargetPosition);
//   Serial.print("Elevation Currentt Position: ");
//   Serial.println(elevation.currentPosition());
// }

// void Gimbal::moveDown(uint deg) {
//   elevationTargetPosition-=STEPS_PER_DEGREE*deg;
//   Serial.print("Elevation Target Position: ");
//   Serial.println(elevationTargetPosition);
//   Serial.print("Elevation Currentt Position: ");
//   Serial.println(elevation.currentPosition());
// }

// void Gimbal::moveRight(uint deg) {
//   azimuth.move(STEPS_PER_DEGREE*deg);
//   // azimuthTargetPosition+=STEPS_PER_DEGREE*deg;
//   // Serial.print("Azimuth Target Position: ");
//   // Serial.println(azimuthTargetPosition);
//   // Serial.print("Azimuth Currentt Position: ");
//   // Serial.println(azimuth.currentPosition());
// }

// void Gimbal::moveLeft(uint deg) {
//   azimuth.move(-STEPS_PER_DEGREE*deg);
//   // azimuthTargetPosition-=STEPS_PER_DEGREE*deg;
//   // Serial.print("Azimuth Target Position: ");
//   // Serial.println(azimuthTargetPosition);
//   // Serial.print("Azimuth Currentt Position: ");
//   // Serial.println(azimuth.currentPosition());
// }

// void Gimbal::setOrigin() {
//   elevation.setCurrentPosition(0);
//   azimuth.setCurrentPosition(0);
//   elevationTargetPosition = 0;
//   azimuthTargetPosition = 0;
// }

// void Gimbal::elevationTo(uint deg) {
//   elevation.moveTo()
//   elevationTargetPosition = deg;
// }

// void Gimbal::azimuthTo(uint deg) {
//   azimuthTargetPosition = deg;
// }
