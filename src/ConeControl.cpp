#include "Arduino.h"
#include "ConeControl.h"
#include <Servo.h>
Servo servoMotor;
ConeControl::ConeControl(int placeholder){
}

void ConeControl::rotateToAngle(int targetAngle){
    servoMotor.write(targetAngle);
}

double ConeControl::getAngle(){
    return servoMotor.read();
}

void ConeControl::attachPin(int pin){
    servoMotor.attach(pin);
}