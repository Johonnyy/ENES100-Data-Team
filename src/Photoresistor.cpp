#include "Photoresistor.h"
#include "Arduino.h"

Photoresistor::Photoresistor()
{
}

bool Photoresistor::isMagnetic()
{
    int value = analogRead(A0);
    // When the magnet is attached, the value is ~960.
    return value > 450;
}

int Photoresistor::getValue()
{
    return analogRead(A0);
}
