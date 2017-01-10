#ifndef COLLISION_H
#define COLLISION_H

struct Collision
{
    bool exitCollisions;
    bool returnTrue;
    bool iterate;

    Collision(bool exitCollisions, bool iterate)
    {
        this->exitCollisions = exitCollisions;
        this->iterate = iterate;
        this->returnTrue = false;
    }
    Collision(bool exitCollisions, bool iterate, bool returnTrue)
    {
        this->exitCollisions = exitCollisions;
        this->iterate = iterate;
        this->returnTrue = returnTrue;
    }

    Collision() {}
};

#endif