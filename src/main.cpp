#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

void turnToAngle(double targetAngle);

//Turning PID loop 
float previous_error = 0;
float integral = 0;


void setup() {
    Enes100.begin("Simulator", DATA, 97, 1116, 52, 50);
    Tank.begin();
    
    Enes100.println("Starting driving");
    turnToAngle(-PI/2);
    Enes100.println("Success 1");
    turnToAngle(PI/2);
    Enes100.println("Success 2");
       turnToAngle(3*-PI/4);
    Enes100.println("Success 3");
}


void loop() {
}



void turnToAngle(double targetAngle)
{
    double curAngle = Enes100.getTheta();
    double error = atan2(sin(targetAngle-curAngle), cos(targetAngle-curAngle));

    const float Kp = 200;
    const float Ki = 0;
    const float Kd = 10;
    const float dt = 10;
    float t = 0;

    while(abs(curAngle-targetAngle)> 0.05) {

        if(Enes100.isVisible())
        {
            integral = integral + error * (dt);
            float derivative = (error - previous_error) / (dt);
            float output = Kp * error + Ki * integral + Kd * derivative;
            previous_error = error;
            delay(dt);
            t+= dt;
        
            //minTurnSpeed
            const float minTurnSpeed = 50;
            Tank.setRightMotorPWM(output + constant);
            Tank.setLeftMotorPWM(-1*output - constant);
        
        if(t > 250)
            {
                Enes100.println("Errror " + (String)error);
                Enes100.println("Output " + (String)output);
                 Enes100.println("X " + (String)Enes100.getX());
                Enes100.println("Y " + (String)Enes100.getTheta());
                t=0;
            }

            curAngle = Enes100.getTheta();
            error = atan2(sin(targetAngle-curAngle), cos(targetAngle-curAngle));

        }
    }
    Tank.setRightMotorPWM(0);
    Tank.setLeftMotorPWM(0);
    previous_error = 0;
    integral = 0;
}

