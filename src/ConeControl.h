#ifndef ConeControl_h
#define ConeControl_h

class ConeControl {
public:

    ConeControl(int placeholder);
    void rotateToAngle(int targetAngle);
    double getAngle();
    void attachPin(int pin);
};

#endif