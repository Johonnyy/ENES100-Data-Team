#include "Arduino.h"
#include "Drive.h"
#include "Ultrasonic.h"
#include "Enes100.h"
#include "ConeControl.h"
#include "Photoresistor.h"
#include "Obstacle.h"



// Variables
int digitalIn = 8; // duty cycle detection

// Objects
VisionSystemClient Enes100;
Photoresistor photoresistor;
Drive drive(1,2);
Ultrasonic ultrasonic(2,3);
ConeControl cone(6);

// Define functions
void moveToObjective();
void movePastObstacles();
void startObjective();
void reportMagnetism();
void reportDutyCycle();
void movePastRow(Obstacle obstacles[]);
void moveUnderBar();  
void dance();

void setup() {
    Serial.begin(9600);
    pinMode(digitalIn, INPUT);
    cone.enable();
    // Enes100.begin("Mikerodata", DATA, 15, 1116, 4, 5);
    // drive.begin();


    // Mission
    moveToObjective();    
    reportMagnetism();
    reportDutyCycle();
    delay(1000);
    movePastObstacles();
    moveUnderBar();
    // dance();
}

void loop() {
}

// Functions
void moveToObjective() {
    if(Enes100.getY() < 1)
    {
        drive.moveToPoint(0.25,1);
        drive.moveToPoint(0.25,1.35);
    } else
    {
        drive.moveToPoint(0.25,1);
        drive.moveToPoint(0.25,0.5);
    }
}

void reportMagnetism()
{
    cone.rotateHover();
    delay(2000);
    cone.rotateMax();
    delay(2000);
    bool magnetic = photoresistor.isMagnetic();
    if(magnetic) 
    {
        Enes100.mission(MAGNETISM, MAGNETIC);
    } else
    {
        Enes100.mission(MAGNETISM, NOT_MAGNETIC);
    }
}

void reportDutyCycle()
{

    // Lower Con
    cone.rotateMin();
    // Wait for contact
    delay(2000);
    drive.setLeft(-255);
    drive.setRight(-255);
    delay(500);


    float totalDuty = 0;

    for (int i = 0; i < 5; i++)
    {
       
        unsigned long highTime = pulseIn(digitalIn, HIGH);
        unsigned long lowTime  = pulseIn(digitalIn, LOW);
        
        Serial.println("High " + String(highTime));
        Serial.println("Low " + String(lowTime)); 

        if (highTime > 0 && lowTime > 0) {
            float duty = (float)highTime / (highTime + lowTime);
            totalDuty += duty;
    }

    delay(20);
    }

    float dutyCycle = totalDuty / 5.0;
    float dutyPercent = dutyCycle * 100.0f;
    float rounded = round(dutyPercent / 10.0f) * 10.0f;

    Enes100.println("Duty Cycle: " + String(rounded));
    Enes100.mission(CYCLE, rounded);
    drive.stop();
    cone.rotateMax();
}



void movePastRow(Obstacle obstacles[])
{
    const int objectThreshhold = 25; //cm

    for (int i = 0; i <= 2; i++)
    {
        Obstacle* closestObstacle = nullptr;
        float closestDistance = 100;
        for(int j = 0; j <= 2; j++)
        {
            Obstacle& obstacle = obstacles[j];
            if(!obstacle.isChecked())
            {
                while(!Enes100.isVisible())
                {
                    delay(30);
                }
                float dx = obstacle.getX() - Enes100.getX();
                float dy = obstacle.getY() - Enes100.getY();
                float distance = sqrt(dx*dx + dy*dy);
                if(distance < closestDistance)
                {
                    closestDistance = distance;
                    closestObstacle = &obstacle;
                }
            }
        }
        if (closestObstacle == nullptr)
        break;

        drive.moveToPoint(closestObstacle->getX(),closestObstacle->getY());
        drive.turnToHeading(0);
        if(!ultrasonic.isObstacle(objectThreshhold)) break;
        closestObstacle -> check();
    }
}

void movePastObstacles()
{
    drive.setLeft(-255);
    drive.setRight(-255);
    delay(1500);

    Enes100.println("moving past");
    Obstacle firstRow[3] = {
    Obstacle(0.8f, 0.35f),
    Obstacle(0.8f, 1.0f),
    Obstacle(0.8f, 1.7f)
    };

     Obstacle secondRow[3] = {
    Obstacle(1.75f, 0.35f),
    Obstacle(1.75f, 1.0f),
    Obstacle(1.75f, 1.7f)
    };

    movePastRow(firstRow);
    movePastRow(secondRow);    
}

void moveUnderBar()
{
    //Move past obstacle
    drive.moveToPoint(2.8,Enes100.getY());
    //Move before bridge
    drive.moveToPoint(3.3, 1.5);
    //Move under bridge
    drive.turnToHeading(0);
    cone.rotateMin();
    drive.setLeft(255);
    drive.setRight(255);
    delay(3000);
    drive.stop();
    delay(100);
}

void dance()
{
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
