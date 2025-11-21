#include "Ultrasonic.h"
#include "Arduino.h"

Ultrasonic::Ultrasonic(int triggerPin, int echoPin)
    : triggerPin(triggerPin), echoPin(echoPin)  // member initializer list
{   
}

float Ultrasonic::getDistance()
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;  // cm
  return distance;
}

float Ultrasonic::getMedianDistance(int n)
{
    if (n <= 0) return -1; //times must be positive

    float readings[n];

    // Collect readings
    for (int i = 0; i < n; i++)
    {
        readings[i] = getDistance();
        delay(50);  // small delay between samples
    }

    // Sort readings
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (readings[j] < readings[i])
            {
                float temp = readings[i];
                readings[i] = readings[j];
                readings[j] = temp;
            }
        }
    }

    if (n % 2 == 0)//if number of elements is even return average of middle values
        return (readings[n/2 - 1] + readings[n/2]) / 2.0;
    else
        return readings[n/2];
}

/* triggerDist: cm */
bool Ultrasonic::isObstacle(float triggerDist)
{
    return getMedianDistance(5) <= triggerDist;
}

