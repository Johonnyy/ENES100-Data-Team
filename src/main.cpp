#include "Arduino.h"
#include "Drive.h"
#include "Ultrasonic.h"
#include "Enes100.h"
#include "Photoresistor.h"


VisionSystemClient Enes100;

// Variables 
bool isStartingA = false;

//Change for wire to detect signal
int digitalIn = -1;

// left motor #1, right motor #2
Drive drive(1,2);
// trigger port 2, echo port 3
Ultrasonic ultrasonic(2,3);
// photoresistor
Photoresistor phototresistor();

void determineStartingPoint();
void moveToObjective();
void movePastObstacles();

void setup() {
    // INITIALIZATION DON'T ALTER!!
    // Serial.begin(9600); //uncomment for serial over usb
    pinMode(digitalIn, INPUT);
    Enes100.begin("Simulator", DATA, 15, 1116, 4, 5);
    delay(2000); 
    drive.begin();

    // Mission
    determineStartingPoint();
    moveToObjective();
}

void loop() {
}

void determineStartingPoint() {
    while (!Enes100.isVisible())
    {
        delay(30);
    }
    float Ay = 1.5;
    float By = 0.5;
    float curY = Enes100.getY();
    isStartingA = fabs(Ay - curY) < fabs(By - curY);
    Enes100.println("Starting Point: " + isStartingA ? "A" : "B");
}

void moveToObjective() {
    if(isStartingA)
    {
        Enes100.println("Moving to B");
        drive.moveToPoint(0.25,0.5);
    } else
    {
        Enes100.println("Moving to A");
        drive.moveToPoint(0.25,1.5);
    }
}

// void startObjective() {
//     // USE THESE
//     // Enes100.mission(CYCLE, i); i is the duty cycle percent (ex. 10, 30, 50, 70, 90)
//     // Enes100.mission(MAGNETISM, MAGNETIC);
//     // Enes100.mission(MAGNETISM, NOT_MAGNETIC);

//     //Lower Servo still above magnnet
//     //Raise back and if it has light its non magnetic
//     bool magnetic = photoresistor.isMagnetic();
//     if(magnetic) 
//     {
//         Enes100.mission(MAGNETISM, MAGNETIC);
//     } else
//     {
//         Enes100.mission(MAGNETISM, NOT_MAGNETIC);
//     }

//     //Go back down fully to pickup puck
    
//     //Report signal wave
//     float totalDuty = 0;

//     for (int i = 0; i < 5; i++)
//     {
//         unsigned long highTime = pulseIn(digitalIn, HIGH);
//         unsigned long lowTime  = pulseIn(digitalIn, LOW);

//         if (highTime > 0 && lowTime > 0) {
//             float duty = (float)highTime / (highTime + lowTime);
//             totalDuty += duty;
//     }

//     delay(20);
//     }

//     float dutyCycle = totalDuty / 5.0;
//     Enes100.mission(CYCLE, dutyCycle);
// }

void movePastObstacles()
{

}

