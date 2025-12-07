#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle {
private:
    float x;
    float y;
    bool checked;

public:
    // Constructor
    Obstacle(float x, float y);

    // Getters
    float getX() const;
    float getY() const;
    bool isChecked() const;

    // Mark as checked
    void check();
};

#endif
