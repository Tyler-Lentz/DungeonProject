#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate
{
    int x;
    int y;

    // Default constructor, leaves the values undefined
    Coordinate() {}

    // Sets the x and y to the specified amounts
    Coordinate(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Coordinate& coord) const
    {
        return ((x == coord.x) && (y == coord.y));
    }

    bool operator<(const Coordinate& coord) const
    {
        if (x == coord.x)
        {
            return (y < coord.y);
        }
        else
        {
            return (x < coord.x);
        }
    }

    bool operator!=(const Coordinate& coord) const
    {
        return (x != coord.x || y != coord.y);
    }
};

#endif