#include "gimbal.h"

void Gimbal::run() {
  elevation.moveTo(elevationTargetPosition);
  azimuth.moveTo(azimuthTargetPosition);
  while (elevation.distanceToGo()!=0 || azimuth.distanceToGo()!=0) {
    elevation.run();
    azimuth.run();
  }
}

void Gimbal::moveUp(uint deg) {
  elevationTargetPosition+=STEPS_PER_DEGREE*deg;
}

void Gimbal::moveDown(uint deg) {
  elevationTargetPosition-=STEPS_PER_DEGREE*deg;
}

void Gimbal::moveRight(uint deg) {
  azimuthTargetPosition+=STEPS_PER_DEGREE*deg;
}

void Gimbal::moveLeft(uint deg) {
  azimuthTargetPosition-=STEPS_PER_DEGREE*deg;
}

void Gimbal::setOrigin() {
  elevation.setCurrentPosition(0);
  azimuth.setCurrentPosition(0);
  azimuth.moveTo(0);
  elevation.moveTo(0);
}

void Gimbal::elevationTo(uint deg) {
  elevationTargetPosition = deg;
}

void Gimbal::azimuthTo(uint deg) {
  azimuthTargetPosition = deg;
}
