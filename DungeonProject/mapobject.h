#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "colorstring.h"
#include "coordinate.h"
#include "collision.h"
#include "utilities.h"

#include <string>
#include <list>

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
        dngutil::TID typeId,
        int priority,
        dngutil::BTID bTypeId,
        bool prematureCheck
    );

    // Save Constructor, used when saving the game. Takes the new game
    MapObject(const MapObject& other, Game* newGame);

    const Coordinate& getCoord() const;
    const ColorChar& getMapRep() const;
    const std::string& getName() const;
    const bool& isMoveable() const;
    const bool& isRawoutput() const;
    const bool& isAggressive() const;
    const bool& hasPrematureCheck() const;
    const dngutil::TID& getTypeId() const;
    const int& getPriority() const;
    const dngutil::BTID& getBTypeId() const;

    Game* getPGame();

    void setPosition(Coordinate coord);

    virtual MapObject* makeSave(Game* game) = 0;

    virtual Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
    {
        return Collision(false, true);
    }

    void removeFromMap(bool deleteit);
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
    bool prematureCheck; // if true, will call mapaction when something tries to move on it

    // Identification to tell apart different types of objects
    dngutil::TID typeId;

    // A broader TID
    dngutil::BTID bTypeId;

    // The higherest priority on a tile will be the object that is drawn
    int priority;
};

//----------------------------------------------------
// Simple MapObjects

class EmptyObject : public MapObject
{
public:
    EmptyObject(Game* game, Coordinate coord):
        MapObject(
            game,
            ColorChar(' ', dngutil::BACKGROUND_COLOR),
            coord,
            "EMPTY",
            true,
            false,
            false,
            dngutil::TID::Empty,
            dngutil::P_EMPTY,
            dngutil::BTID::None,
            false
        )
    {

    }

    virtual MapObject* makeSave(Game* game)
    {
        return new EmptyObject(game, getCoord());
    }
};

class WallObject : public MapObject
{
public:
    WallObject(Game* game, Coordinate coord):
        MapObject(
            game,
            ColorChar('#', dngutil::BLACK),
            coord,
            "WALL",
            false,
            false,
            false,
            dngutil::TID::Wall,
            dngutil::P_WALL,
            dngutil::BTID::None,
            false
        )
    {

    }

    virtual MapObject* makeSave(Game* game)
    {
        return new WallObject(game, getCoord());
    }
};

class ExitObject : public MapObject
{
public:
    ExitObject(Game* game, Coordinate coord, bool up, ColorChar mapRep)
        :MapObject(
            game,
            mapRep,
            coord,
            "EXIT",
            true,
            false,
            false,
            dngutil::TID::Exit,
            dngutil::P_STAIRCASE,
            dngutil::BTID::None,
            false
        )
    {
        this->up = up;
    }

    ExitObject(const ExitObject& other, Game* game)
        :MapObject(other, game)
    {
        this->up = other.up;
    }

    virtual MapObject* makeSave(Game* game)
    {
        return new ExitObject(*this, game);
    }

    // Defined in mapobject.cpp because it is big
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

    const bool& isUp() const
    {
        return up;
    }

private:
    bool up;
};

class HoleObject : public MapObject
{
public:
    HoleObject(Game* game, Coordinate coord);
    virtual Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it);
    virtual MapObject* makeSave(Game* game)
    {
        return new HoleObject(game, getCoord());
    }
};

class DoorObject : public MapObject
{
public:
    DoorObject(Game* game, Coordinate coord, ColorChar mapRep);
    virtual Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it);
    virtual MapObject* makeSave(Game* game)
    {
        return new HoleObject(game, getCoord());
    }
};

//----------------------------------------------------
#endif