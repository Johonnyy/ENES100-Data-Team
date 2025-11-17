#ifndef Ultrasonic_h
#define Ultrasonic_h

class Ultrasonic {
public:

    Ultrasonic(int triggerPin, int echoPin);

    float getMedianDistance(int n);
    bool isObstacle(float triggerDist);




private:
    int triggerPin;
    int echoPin;
    float getDistance();
};

#endif