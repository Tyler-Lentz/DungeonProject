#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate
{
    int x;
    int y;

    // Sets the x and y to the specified amounts
    Coordinate(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    // Moves the coordinates x to the right by 1, and if it will go off the screen,
    // moves it to the next line
    void moveRight(int vwinXSize)
    {
        if (x + 1 < vwinXSize)
        {
            x++;
        }
        else
        {
            x = 0;
            y++;
        }
    }
};

#endif