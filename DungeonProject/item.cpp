#include "item.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"

#include <string>
//-------------------------------------------------------
// Item Functions

Item::Item(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId,
    bool consumable
) :MapObject(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId)
{
    this->consumable = consumable;
}

Item::Item(const Item& other, Game* game)
    :MapObject(other, game)
{
    this->consumable = other.consumable;
}

const bool& Item::isConsumable() const
{
    return consumable;
}

//-------------------------------------------------------

//-------------------------------------------------------
// RItem Functions

RItem::RItem(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId,
    bool consumable
) :Item(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId, consumable) 
{

}

RItem::RItem(const RItem& other, Game* game)
    :Item(other, game)
{

}

Collision RItem::mapAction(MapObject* collider)
{
    // TODO: implement function where the item is added to inventory and then deleted
}

//-------------------------------------------------------

//-------------------------------------------------------
// Potion Functions

Potion::Potion(Game* pgame, Coordinate coord, int healAmount)
    :RItem(pgame, ColorChar('o', dngutil::RED), coord, "Potion", true, false, false, dngutil::TID::Potion, true)
{
    this->healAmount = healAmount;
}

Potion::Potion(const Potion& other, Game* game)
    :RItem(other, game)
{
    this->healAmount = other.healAmount;
}

void Potion::action()
{
    // TODO: implement this
    // increase players health by healAmount
    // display amount of health healed
}

Item* Potion::makeSaveInv(Game* game)
{
    return new Potion(*this, game);
}

MapObject* Potion::makeSave(Game* game)
{
    return new Potion(*this, game);
}

//-------------------------------------------------------

//-------------------------------------------------------
// Primary and Secondary Functions

Item* Primary::makeSaveInv(Game* game)
{
    return new Primary(*this, game);
}

MapObject* Primary::makeSave(Game* game)
{
    return new Primary(*this, game);
}

void Primary::action()
{
    // TODO:
    // call players swapprimary function
}

const int& Primary::getDmgMultiplier() const
{
    return dmgMultiplier;
}

const int& Primary::getAttSpeed() const
{
    return attSpeed;
}

const int& Primary::getAccuracy() const
{
    return accuracy;
}

const bool& Primary::getStartReady() const
{
    return startReady;
}



Item* Secondary::makeSaveInv(Game* game)
{
    return new Secondary(*this, game);
}

MapObject* Secondary::makeSave(Game* game)
{
    return new Secondary(*this, game);
}

void Secondary::action()
{
    // TODO: call players swap secondary function
}

const int& Secondary::getDeflectTime() const
{
    return deflectTime;
}

const int& Secondary::getDmdReductMult() const
{
    return dmgReductMult;
}

//-------------------------------------------------------