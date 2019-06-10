#ifndef GIMBAL_H
#define GIMBAL_H

#include "Arduino.h"
#include "AccelStepper.h"

#define STEPS_PER_DEGREE 11.3777777777

class Gimbal {
  public:
    Gimbal(uint ele_pin1, uint ele_pin2, uint ele_pin3, uint ele_pin4,
      uint azi_pin1, uint azi_pin2, uint azi_pin3, uint azi_pin4,
      float maxSpeed, float acc) {
  
        pinMode(ele_pin1, OUTPUT);
        pinMode(ele_pin2, OUTPUT);
        pinMode(ele_pin3, OUTPUT);
        pinMode(ele_pin4, OUTPUT);
        pinMode(azi_pin1, OUTPUT);
        pinMode(azi_pin2, OUTPUT);
        pinMode(azi_pin3, OUTPUT);
        pinMode(azi_pin4, OUTPUT);
  
        elevation = AccelStepper(5, ele_pin1, ele_pin2, ele_pin3, ele_pin4);
        azimuth = AccelStepper(5, azi_pin1, azi_pin2, azi_pin3, azi_pin4);
       
        azimuth.setMaxSpeed(900.0);
        azimuth.setAcceleration(1000.0);
          
        elevation.setMaxSpeed(900.0);
        elevation.setAcceleration(1000.0);

    };
    void run();
    void moveUp(uint deg);
    void moveDown(uint deg);
    void moveRight(uint deg);
    void moveLeft(uint deg);
    void azimuthTo(uint deg);
    void elevationTo(uint deg);
    void setOrigin();
  
  private:
    AccelStepper elevation;
    AccelStepper azimuth;
    long elevationTargetPosition;
    long azimuthTargetPosition;
};

#endif /* GIMBAL_H */
