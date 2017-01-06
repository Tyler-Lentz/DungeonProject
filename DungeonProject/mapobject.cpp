#include "mapobject.h"
#include "utilities.h"

class Game; // TODO: change to include when game is implemented

//---------------------------------------------------------------
// MapObject functions

MapObject::MapObject(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId
)
    :coord(coord.x, coord.y)
{
    this->pgame = pgame;
    this->mapRep = mapRep;
    this->name = name;
    this->moveable = moveable;
    this->rawoutput = rawoutput;
    this->aggressive = aggressive;
    this->typeId = typeId;
}

MapObject::MapObject(const MapObject& other, Game* newGame)
    :coord(other.coord)
{
    // TODO: put real code here when game is implemented
    /* this->pgame = new Game(newGame); */
    this->mapRep = other.mapRep;
    this->name = other.name;
    this->moveable = other.moveable;
    this->rawoutput = other.rawoutput;
    this->aggressive = other.aggressive;
    this->typeId = other.typeId;
}

const Coordinate& MapObject::getCoord() const
{
    return coord;
}

const ColorChar& MapObject::getMapRep() const
{
    return mapRep;
}

const std::string MapObject::getName() const
{
    return name;
}

const bool& MapObject::isMoveable() const
{
    return moveable;
}

const bool& MapObject::isRawoutput() const
{
    return rawoutput;
}

const bool& MapObject::isAggressive() const
{
    return aggressive;
}

const dngutil::TID& MapObject::getTypeId() const
{
    return typeId;
}

//---------------------------------------------------------------