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
    void setName(std::string name);
    void changeMapChar(ColorChar Char);

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
    EmptyObject(Game* game, Coordinate coord) :
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
        ) {}
};

class WallObject : public MapObject
{
public:
    WallObject(Game* game, Coordinate coord) :
        MapObject(
            game,
            ColorChar(static_cast<unsigned char>(178), dngutil::BLACK),
            coord,
            "WALL",
            false,
            false,
            false,
            dngutil::TID::Wall,
            dngutil::P_WALL,
            dngutil::BTID::None,
            false
        ) {}
};

class GravestoneObject : public MapObject
{
public:
    GravestoneObject(Game* game, Coordinate coord) :
        MapObject(
            game,
            ColorChar(static_cast<unsigned char>(219), dngutil::WHITE),
            coord,
            "GRAVE",
            false,
            false,
            false,
            dngutil::TID::Wall,
            dngutil::P_WALL,
            dngutil::BTID::None,
            false
        ) {}
};

class EvilWallObject : public MapObject
{
public:
    EvilWallObject(Game* game, Coordinate coord) :
        MapObject(
            game,
            ColorChar(static_cast<unsigned char>(178), getColor(dngutil::MAGENTA, dngutil::LIGHTMAGENTA)),
            coord,
            "WALL",
            false,
            true,
            false,
            dngutil::TID::Wall,
            dngutil::P_WALL,
            dngutil::BTID::None,
            false
        ) {}
};

class CastleWallObject : public MapObject
{
public:
    CastleWallObject(Game* game, Coordinate coord) :
        MapObject(
            game,
            ColorChar(static_cast<unsigned char>(178), getColor(dngutil::DARKGRAY, dngutil::LIGHTGRAY)),
            coord,
            "WALL",
            false,
            true,
            false,
            dngutil::TID::Wall,
            dngutil::P_WALL,
            dngutil::BTID::None,
            false
        ) {}
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
        itemNeeded = dngutil::TID::Empty;
    }

    ExitObject(Game* game, Coordinate coord, bool up, ColorChar mapRep, dngutil::TID itemNeeded, bool rawoutput)
        :MapObject(
            game,
            mapRep,
            coord,
            "EXIT",
            true,
            rawoutput,
            false,
            dngutil::TID::Exit,
            dngutil::P_STAIRCASE,
            dngutil::BTID::None,
            false
        )
    {
        this->up = up;
        this->itemNeeded = itemNeeded;
    }

    // Defined in mapobject.cpp because it is big
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

    const bool& isUp() const
    {
        return up;
    }

private:
    bool up;
    dngutil::TID itemNeeded;
};

class HoleObject : public MapObject
{
public:
    HoleObject(Game* game, Coordinate coord);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};

class Water : public MapObject
{
public:
    Water(Game* game, Coordinate coord, bool deep);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
private:
    bool deep;
};

class TreeObject : public MapObject
{
public:
    TreeObject(Game* game, Coordinate coord);
};

class DoorObject : public MapObject
{
public:
    DoorObject(Game* game, Coordinate coord, ColorChar mapRep);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};

class DungeonCheck : public MapObject
{
public:
    DungeonCheck(Game* game, Coordinate coord, int harp);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

private:
    int harpNumber;
    bool allow5Cross;
};

class AltarObject : public MapObject
{
public:
    AltarObject(Game* game, Coordinate coord);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};


class HeroSpirit : public MapObject
{
public:
    HeroSpirit(Game* game, Coordinate coord);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};

class VenderObject : public MapObject
{
public:
    VenderObject(Game* game, Coordinate coord, ColorString name, ColorChar mapRep, dngutil::TID itemTid, int price);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

private:
    ColorString itemName;
    ColorChar mapRep;
    dngutil::TID itemTid;
    int price;
};

class HerosBladeStone : public MapObject
{
public:
    HerosBladeStone(Game* game, Coordinate coord);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};

class HouseDoorObject : public MapObject
{
public:
    HouseDoorObject(Game* game, Coordinate coord, Coordinate newMapCoord, Coordinate newRoomCoord, int floor);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

private:
    Coordinate newMapCoord;
    Coordinate newRoomCoord;
    int floor;
};


class HarpPiece : public MapObject
{
public:
    HarpPiece(Game* game, Coordinate coord, int pieceNumber);
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

private:
    int pieceNumber;
};

class Segboss;
class SegbossTrigger : public MapObject
{
public:
    SegbossTrigger(Game* game, Coordinate coord, Segboss* segboss, ColorChar colchar);
    ~SegbossTrigger();
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
private:
    Segboss* segboss;
};
//----------------------------------------------------
#endif