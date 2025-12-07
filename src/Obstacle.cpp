#include "Obstacle.h"

// Constructor
Obstacle::Obstacle(float x, float y)
    : x(x), y(y), checked(false) {}

// Getters
float Obstacle::getX() const {
    return x;
}

float Obstacle::getY() const {
    return y;
}

bool Obstacle::isChecked() const {
    return checked;
}

// Mark obstacle as checked
void Obstacle::check() {
    checked = true;
}
