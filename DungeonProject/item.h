#ifndef ITEM_H
#define ITEM_H

#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"

#include <string>

class Game;
class Player;

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
        bool consumable,
        std::string description
    );

    // Save constructor
    Item(const Item& other, Game* game);

    // This is the same as make save, but it returns an Item. Used in inventory copying.
    virtual Item* makeSaveInv(Game* game) = 0;

    // The action done when the player uses the item in their inventory
    virtual void action(Player* player, unsigned int inventoryIndex) = 0;

    const bool& isConsumable() const;
    const std::string& getDescription() const;
private:
    // consumable tells it to delete if after using it in the inventory, or
    // something more specific for irregular items
    bool consumable;

    std::string description;
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
        bool consumable,
        std::string description
    );

    // Save Constructor
    RItem(const RItem& other, Game* game);

    // MapAction for regular items, adds it to the player's inventory
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
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
        double dmgMultiplier,
        int attSpeed,
        int accuracy,
        bool startReady,
        std::string description
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, typeId, false, description)
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

    Item* makeSaveInv(Game* game) override;
    MapObject* makeSave(Game* game) override;

    void action(Player* player, unsigned int inventoryIndex) override;

    const double& getDmgMultiplier() const;
    const int& getAttSpeed() const;
    const int& getAccuracy() const;
    const bool& getStartReady() const;

    bool hit() const;
private:
    // The damage multiplier that is given to the attackers attack during battle
    double dmgMultiplier;

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
        double dmgReductMult,
        std::string description
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, typeId, false, description)
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

    Item* makeSaveInv(Game* game) override;
    MapObject* makeSave(Game* game) override;

    void action(Player* player, unsigned int inventoryIndex) override;

    const double& getDmdReductMult() const;
    const int& getDeflectTime() const;
private:
    // The attackers attack is multiplied by this amount
    double dmgReductMult;

    // The amount of time in miliseconds you have to deflect
    int deflectTime;
};

class Potion : public RItem
{
public:
    Potion(Game* pgame, Coordinate coord, int healAmount);
    
    // Save constructor
    Potion(const Potion& other, Game* game);

    Item* makeSaveInv(Game* game) override;
    MapObject* makeSave(Game* game) override;

    void action(Player* player, unsigned int inventoryIndex) override;
private:
    int healAmount;
};

class Key : public RItem
{
public:
    Key(Game* pgame, Coordinate coord);

    Key(const Key& other, Game* game);

    Item* makeSaveInv(Game* game) override;
    MapObject* makeSave(Game* game) override;

    void action(Player* player, unsigned int inventoryIndex) override;
};

//-------------------------------------------------------------
#endif