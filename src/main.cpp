#include "Arduino.h"
#include "Drive.h"
#include "Ultrasonic.h"
#include "Enes100.h"
#include "Photoresistor.h"


VisionSystemClient Enes100;

// Variables 
bool isStartingA = false;

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
    Enes100.begin("Simulator", DATA, 275, 1116, 4, 5);
    delay(2000); 
    drive.begin();

    drive.turnToHeading(PI/2);

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

void movePastObstacles()
{

}

