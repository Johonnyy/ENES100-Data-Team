#include "Arduino.h"
#include "Drive.h"
#include "Ultrasonic.h"
#include "Enes100.h"
#include "ConeControl.h"
#include "Photoresistor.h"


VisionSystemClient Enes100;

// Variables 
bool isStartingA = false;

//Change for wire to detect signal
int digitalIn = 8;

// left motor #1, right motor #2
Photoresistor photoresistor;
Drive drive(1,2);
Ultrasonic ultrasonic(2,3);
ConeControl cone(10);


void determineStartingPoint();
void moveToObjective();
void movePastObstacles();
void startObjective();
void dutyCycle();
float getClosestY();

void setup() {
    // INITIALIZATION DON'T ALTER!!
    delay(2000);
    pinMode(digitalIn, INPUT);
    cone.attachPin(10);
    Enes100.begin("Mikerodata", DATA, 15, 1116, 4, 5);
    drive.begin();

    dutyCycle();

    // Mission
    // determineStartingPoint();
    // moveToObjective();
    // delay(1000);
    // movePastObstacles();
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

void startObjective() {
    // USE THESE
    // Enes100.mission(CYCLE, i); i is the duty cycle percent (ex. 10, 30, 50, 70, 90)
    // Enes100.mission(MAGNETISM, MAGNETIC);
    // Enes100.mission(MAGNETISM, NOT_MAGNETIC);

    //Lower Servo still above magnnet
    //Raise back and if it has light its non magnetic
    // bool magnetic = photoresistor.isMagnetic();
    // if(magnetic) 
    // {
    //     Enes100.mission(MAGNETISM, MAGNETIC);
    // } else
    // {
    //     Enes100.mission(MAGNETISM, NOT_MAGNETIC);
    // }

    //Goes fully down and reports cycle
    dutyCycle();
}


void dutyCycle()
{
    const float minConeAngle = 30;
    const float maxConeAngle = 150;

    // Lower Cone
    cone.rotateToAngle(minConeAngle);
    // Wait for contact
    delay(2000);

    float totalDuty = 0;

    for (int i = 0; i < 5; i++)
    {
        unsigned long highTime = pulseIn(digitalIn, HIGH);
        unsigned long lowTime  = pulseIn(digitalIn, LOW);

        if (highTime > 0 && lowTime > 0) {
            float duty = (float)highTime / (highTime + lowTime);
            totalDuty += duty;
    }

    delay(20);
    }

    float dutyCycle = totalDuty / 5.0;
    Enes100.println("Duty Cycle: " + String(dutyCycle));
    Enes100.mission(CYCLE, dutyCycle);

    cone.rotateToAngle(maxConeAngle);
}

void movePastObstacles()
{
    const int objectThreshhold = 25;

    //If started at bottom go upward, if at top go downward
    int direction = getClosestY() == 0.5 ? 1 : -1;
    for(int i = 0; i <= 2; i++)
    {
        // move backward and up/down if obstacle detected
        if(i != 0)
        {
            Enes100.println("up or down!");
            drive.moveToPoint(Enes100.getX(), Enes100.getY() + 0.5 * direction);      
        }
        Enes100.println("moving to testing point");
        drive.moveToPoint(0.8, getClosestY());
        Enes100.println("aligning");
        drive.turnToHeading(0);
        if(!ultrasonic.isObstacle(objectThreshhold))
        {
            Enes100.println("Distance: " + String(ultrasonic.getMedianDistance(5)) + " cm");
            Enes100.println("No object");
                break; //Exits for loop
        }
        Enes100.println("Object detected!");
        Enes100.println("LOOP");
    }
     
    //Move to second row 
    drive.moveToPoint(1.75, getClosestY());

    float closestY = getClosestY();
    bool startedMiddle = closestY == 1;
    direction = (closestY == 0.5 || startedMiddle) ? 1 : -1;
    for(int i = 0; i <= 2; i++)
    {
        // move backward and up/down if obstacle detected
        if(i != 0)
        {
            Enes100.println("up or down!");
            if(startedMiddle && i == 2)
            {
                //Now middle and top are blocked go through bottom
                   drive.moveToPoint(Enes100.getX(), Enes100.getY() - 1);     
            } else
            {
                   drive.moveToPoint(Enes100.getX(), Enes100.getY() + 0.5 * direction);     
            }       
        }
        Enes100.println("moving to testing point");
        drive.moveToPoint(1.75, getClosestY());
        Enes100.println("aligning");
        drive.turnToHeading(0);
        if(!ultrasonic.isObstacle(objectThreshhold))
        {
            Enes100.println("Distance: " + String(ultrasonic.getMedianDistance(5)) + " cm");
            Enes100.println("No object");
                break; //Exits for loop
        }
        Enes100.println("Object detected!");
        Enes100.println("LOOP");
    }

    //Move past obstacle
    drive.moveToPoint(2.8,Enes100.getY());
    //Move before bridge
    drive.moveToPoint(3.3, 1.5);
    //Move under bridge
    drive.turnToHeading(0);
    cone.rotateToAngle(30);
    drive.setLeft(255);
    drive.setRight(255);
    delay(3000);
    drive.stop();
    delay(100);

    //DANCE WE WON!
    int left = 255;
    int right = -255;
    while(true)
    {
     
        drive.setLeft(left);
        drive.setRight(right);
        delay(1000);
        left = -left;
        right = -right;
    }
    
}



float getClosestY(){
    while(!Enes100.isVisible())
    {
        delay(30);
    }
    float ypos = Enes100.getY();
    float dyBottom = abs(ypos - 0.5);
    float dyMiddle = abs(ypos - 1);
    float dyTop = abs(ypos - 1.5);

      // bottom is closest
    if (dyBottom <= dyMiddle && dyBottom <= dyTop) {
        return 0.35;
    }
    // middle is closest
    else if (dyMiddle <= dyBottom && dyMiddle <= dyTop) {
        return 1.0;
    }
    // top is closest
    else {
        return 1.7;
    }
}