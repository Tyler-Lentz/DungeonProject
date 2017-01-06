#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "colorstring.h"
#include "coordinate.h"

#include <string>

class Game;

class MapObject
{
public:
    // Regular Constructor
    MapObject(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool moveable,
        bool rawoutput,
        bool aggressive,
        int typeId
    );

    // Save Constructor, used when saving the game. Takes the new game
    MapObject(const MapObject& other, Game* newGame);

    const Coordinate& getCoord() const;
    const ColorChar& getMapRep() const;
    const std::string getName() const;
    const bool& isMoveable() const;
    const bool& isRawoutput() const;
    const bool& isAggressive() const;
    const int& getTypeId() const;
    
    virtual MapObject* makeNew(Game* game) = 0;
    virtual MapObject* makeSave(Game* game) = 0;
private:
    // Pointer to the game
    Game* pgame;

    // The position on the map
    Coordinate coord;

    // The color and character on the map
    ColorChar mapRep;

    // Name to be displayed on screen
    std::string name;

    // Additional information about the object
    bool moveable;
    bool rawoutput;
        // tells the grid when drawing that it should draw this to the map
        // exactly as the ColorChar is
    bool aggressive;

    // Identification to tell apart different types of objects
    int typeId;
};

#endif