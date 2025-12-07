#include "Arduino.h"
#include "ConeControl.h"

ConeControl::ConeControl(int pin) : servoPin(pin) {
}

void ConeControl::rotateToAngle(int targetAngle) {
    servoMotor.write(targetAngle);
}

void ConeControl::rotateMax() {
    rotateToAngle(150);
}

void ConeControl::rotateMin() {
    rotateToAngle(0);
}

void ConeControl::rotateHover() {
    rotateToAngle(30);
}

double ConeControl::getAngle() {
    return servoMotor.read();
}

void ConeControl::enable() {
    servoMotor.attach(servoPin);
}

void ConeControl::disable() {
    servoMotor.detach();
}

void ConeControl::moveServoSmooth(int startAngle, int endAngle, int stepDelayMs) {
  if(startAngle < endAngle){
    for(int pos = startAngle; pos <= endAngle; pos++){
      servoMotor.write(pos);
      delay(stepDelayMs);
    }
  } else {
    for(int pos = startAngle; pos >= endAngle; pos--){
      servoMotor.write(pos);
      delay(stepDelayMs);
    }
  }
}

