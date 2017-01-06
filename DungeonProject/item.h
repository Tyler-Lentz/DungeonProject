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