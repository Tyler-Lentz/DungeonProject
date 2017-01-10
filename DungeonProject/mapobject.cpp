#include "mapobject.h"
#include "utilities.h"
#include "game.h"
#include "room.h"

#include <list>

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
    dngutil::TID typeId,
    int priority,
    dngutil::BTID bTypeId
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
    this->priority = priority;
    this->bTypeId = bTypeId;
}

MapObject::MapObject(const MapObject& other, Game* newGame)
    :coord(other.coord)
{
    this->pgame = new Game(*newGame);
    this->mapRep = other.mapRep;
    this->name = other.name;
    this->moveable = other.moveable;
    this->rawoutput = other.rawoutput;
    this->aggressive = other.aggressive;
    this->typeId = other.typeId;
    this->priority = other.priority;
    this->bTypeId = other.bTypeId;
}

void MapObject::setPosition(Coordinate coord)
{
    this->coord = coord;
}

const Coordinate& MapObject::getCoord() const
{
    return coord;
}

const ColorChar& MapObject::getMapRep() const
{
    return mapRep;
}

const std::string& MapObject::getName() const
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

const dngutil::BTID& MapObject::getBTypeId() const
{
    return bTypeId;
}

const int& MapObject::getPriority() const
{
    return priority;
}

Game* MapObject::getPGame()
{
    return pgame;
}

void MapObject::removeFromMap(bool deleteit)
{
    getPGame()->getActiveRoom()->addCoordToList(coord);
    coord = Coordinate(-1, -1);
    if (deleteit)
    {
        getPGame()->getDeletionList().push_back(this);
    }
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Exit Object Functions

Collision ExitObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator it)
{
    // TODO: implement this
}

//---------------------------------------------------------------