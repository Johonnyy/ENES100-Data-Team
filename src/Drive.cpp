#include "Drive.h"
#include "Enes100.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left;
Adafruit_DCMotor *right;

Drive::Drive(int leftPort, int rightPort)
    : leftPort(leftPort), rightPort(rightPort)  // member initializer list
{
}

void Drive::begin() {
    AFMS.begin();
    left = AFMS.getMotor(leftPort);
    right = AFMS.getMotor(rightPort);
}

void Drive::setLeft(float PWM) {
    PWM = constrain(PWM, -255,255);
    left->setSpeed(PWM);
    if(PWM > 0 )
    {
        left->run(FORWARD);
    } else
    {
        left->run(BACKWARD);
    }
}

void Drive::setRight(float PWM) {
    PWM = constrain(PWM, -255,255);
    right->setSpeed(PWM);
    if(PWM > 0 )
    {
        right->run(FORWARD);
    } else
    {
        right->run(BACKWARD);
    }
}

float Drive::getHeading() {
    return Enes100.getTheta();
}

float Drive::getMinAngle(float initialAngle, float finalAngle) {
    return atan2f(sinf(finalAngle-initialAngle), cosf(finalAngle-initialAngle));
}

void Drive::stop() {
    left->run(RELEASE);
    right->run(RELEASE);
}

void Drive::turnToHeading(float heading)
{
    const float Kp = 10.0;
    const float tol = 0.03;
    const int dt = 20.0;
    const float minPWM = 0.0;

    float error = getMinAngle(getHeading(), heading);
  
    while (fabs(error) >= tol)
    {
        if(Enes100.isVisible())
        {
            error = getMinAngle(getHeading(), heading);
          
            float output = error * Kp;

            if (fabs(output) < minPWM)
                output = (output > 0 ? 1 : -1) * minPWM;

            setLeft(-output);
            setRight(output);
        }
         delay(dt);
    }
    stop();
}


void Drive::moveToPoint(float xf, float yf)
{
    const float Kp_linear = 100.0;
    const float Kp_angular = 160.0;
    const float minPWM = 50.0;   
    const float tol = 0.05;
    const float dt = 20.0;

     float dx = xf - Enes100.getX();
     float dy = yf - Enes100.getY();
     float error = sqrt(dx*dx + dy*dy);

     turnToHeading(atan2f(dy,dx));

    while(error >= tol)
    {
        if(Enes100.isVisible())
        {
            dx = xf - Enes100.getX();
            dy = yf - Enes100.getY();
            error = sqrt(dx*dx + dy*dy);

            float angularError = getMinAngle(Enes100.getTheta(), atan2f(dy,dx));

            float linearOutput = constrain(error * Kp_linear,-200,200);
            float angularOutput = constrain(angularError * Kp_angular,-100,100);
        

            setLeft(constrain(linearOutput - angularOutput,-255,255));
            setRight(constrain(linearOutput + angularOutput,-255,255));
        }
        delay(dt);
    }
    stop();
}