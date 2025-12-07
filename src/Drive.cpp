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
    if(PWM > 0 )
    {
        left->run(FORWARD);
    } else
    {
        left->run(BACKWARD);
    }
    left->setSpeed(abs(PWM));
}

void Drive::setRight(float PWM) {
    PWM = constrain(PWM, -255,255);
    if(PWM > 0 )
    {
        right->run(FORWARD);
    } else
    {
        right->run(BACKWARD);
    }
     right->setSpeed(abs(PWM));
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
    const float Kp = 160;
    const float tol = 0.03;
    const int dt = 20.0;
    const float minPWM = 35.0;

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
    const float Kp_linear = 400.0;
    const float Kp_angular = 100.0;
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

            float linearOutput = constrain(error * Kp_linear,-255,255);
            float angularOutput = constrain(angularError * Kp_angular,-100,100);

            // Angular filtering
            float angularDeadband = 0.1;
            if(error >= angularDeadband)
            {
                // Starts lowering angularOutput as distance is below angularSlowThreshold
                float angularSlowThreshold = 0.2; //20cm
                float distFactor = constrain(error / angularSlowThreshold, 0.0, 1.0);
                angularOutput *= distFactor;
            } else
            {
                angularOutput = 0;
            }
        
            if(linearOutput < minPWM) linearOutput = minPWM;
        

            setLeft(constrain(linearOutput - angularOutput,-255,255));
            setRight(constrain(linearOutput + angularOutput,-255,255));
        }
        delay(dt);
    }
    stop();
}