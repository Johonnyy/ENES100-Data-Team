#include "Photoresistor.h"
#include "Arduino.h"

Photoresistor::Photoresistor()
{
}

bool isMagnetic()
{
    //UPDATE ANALOG PIN
    int value = analogRead(A0);
    return value < 450;
}