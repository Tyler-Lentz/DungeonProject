#ifndef ITEM_H
#define ITEM_H

#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"

#include <string>

class Game;

class Item : public MapObject
{
public:
    Item(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool moveable,
        bool rawoutput,
        bool aggressive,
        dngutil::TID typeId,
        bool consumable
    );

    // Save constructor
    Item(const Item& other, Game* game);

    const bool& isConsumable() const;
private:
    // consumable tells it to delete if after using it in the inventory, or
    // something more specific for irregular items
    bool consumable;

};

//-------------------------------------------------------------
// Regular Items
class RItem : public Item
{
public:
    RItem(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool moveable,
        bool rawoutput,
        bool aggressive,
        dngutil::TID typeId,
        bool consumable
    ) :Item(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId, consumable) {}

    // Save Constructor
    RItem(const RItem& other, Game* game);

    // MapAction for regular items, adds it to the player's inventory
    virtual Collision mapAction(MapObject* collider);

    // The action done when the player uses the item in their inventory
    virtual void action() = 0;
};

class Primary : public RItem
{
public:
    Primary(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        dngutil::TID typeId,
        int dmgMultiplier,
        int attSpeed,
        int accuracy,
        bool startReady
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, typeId, false)
    {
        this->dmgMultiplier = dmgMultiplier;
        this->attSpeed = attSpeed;
        this->accuracy = accuracy;
        this->startReady = startReady;
    }

    // Save constructor
    Primary(const Primary& other, Game* game)
        :RItem(other, game)
    {
        this->dmgMultiplier = other.dmgMultiplier;
        this->attSpeed = other.attSpeed;
        this->accuracy = other.accuracy;
        this->startReady = other.startReady;
    }

    virtual MapObject* makeNew(Game* game, Coordinate coord);
    virtual MapObject* makeSave(Game* game);

    virtual void action();

    const int& getDmgMultiplier() const;
    const int& getAttSpeed() const;
    const int& getAccuracy() const;
    const bool& getStartReady() const;
private:
    // The damage multiplier that is given to the attackers attack during battle
    int dmgMultiplier;

    // How many seconds it takes to fully charge a weapon
    int attSpeed;

    // A number from 0 - 100 that is the weapons chance as a percentage to hit
    int accuracy;

    // If this is true, when entering a battle the weapon's charge meter
    // will be fully charged.
    bool startReady;
};

class Secondary : public RItem
{
public:
    Secondary(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        dngutil::TID typeId,
        int deflectTime,
        int dmgReductMult
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, typeId, false)
    {
        this->deflectTime = deflectTime;
        this->dmgReductMult = dmgReductMult;
    }

    // Save constructor
    Secondary(const Secondary& other, Game* game)
        :RItem(other, game)
    {
        this->deflectTime = other.deflectTime;
        this->dmgReductMult = other.dmgReductMult;
    }

    virtual MapObject* makeNew(Game* game, Coordinate coord);
    virtual MapObject* makeSave(Game* game);

    virtual void action();

    const int& getDmdReductMult() const;
    const int& getDeflectTime() const;
private:
    // The attackers attack is multiplied by this amount
    int dmgReductMult;

    // The amount of time in miliseconds you have to deflect
    int deflectTime;
};

class Potion : public RItem
{
public:
    Potion(Game* pgame, Coordinate coord, int healAmount);
    
    // Save constructor
    Potion(const Potion& other, Game* game);

    virtual MapObject* makeNew(Game* game, Coordinate coord);
    virtual MapObject* makeSave(Game* game);

    virtual void action();
private:
    int healAmount;
};

//-------------------------------------------------------------
#endif