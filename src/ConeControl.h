#ifndef ConeControl_h
#define ConeControl_h

#include <Servo.h>

class ConeControl {
public:
    ConeControl(int pin);  
    void rotateToAngle(int targetAngle);
    void rotateMax();
    void rotateMin();
    void rotateHover();
    double getAngle();
    void enable();
    void disable();

private:
    Servo servoMotor;     
    int servoPin;             
};

#endif
