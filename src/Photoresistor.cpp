#include "Photoresistor.h"
#include "Arduino.h"

Photoresistor::Photoresistor()
{
}

bool Photoresistor::isMagnetic()
{
    int value = analogRead(A0);
    return value < 450;
}
