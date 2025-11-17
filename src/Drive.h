#ifndef Drive_h
#define Drive_h

#include <Adafruit_MotorShield.h>



class Drive {
public:

     Drive(int leftPort, int rightPort);

    void turnToHeading(float heading);
    void moveToPoint(float xf, float yf);
    void stop();
    float getHeading();
    float getMinAngle(float initialAngle, float finalAngle);

    void setLeft(float PWM);
    void setRight(float PWM);

    void begin();





private:
    int leftPort;
    int rightPort;

    // void setLeft(float PWM);
    // void setRight(float PWM);


};
#endif
