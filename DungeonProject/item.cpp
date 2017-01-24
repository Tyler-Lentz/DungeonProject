#include "item.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "player.h"
#include "game.h"
#include "virtualwindow.h"
#include "room.h"

#include <list>
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
    bool consumable,
    std::string description
) :MapObject(
    pgame,
    mapRep,
    coord,
    name,
    moveable,
    rawoutput,
    aggressive,
    typeId,
    dngutil::P_ITEM,
    dngutil::BTID::Item,
    false
)
{
    this->consumable = consumable;
    this->description = description;
}

Item::Item(const Item& other, Game* game)
    :MapObject(other, game)
{
    this->consumable = other.consumable;
    this->description = other.description;
}

const bool& Item::isConsumable() const
{
    return consumable;
}

const std::string& Item::getDescription() const
{
    return description;
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
    bool consumable,
    std::string description
) :Item(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId, consumable, description) 
{

}

RItem::RItem(const RItem& other, Game* game)
    :Item(other, game)
{

}

Collision RItem::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        soundEffect("PickupItem.wav", false, true);
        getPGame()->getPlayer()->addToInventory(this);
        it++;
        getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
        removeFromMap(false);
        return Collision(false, false);
    }
    return Collision(false, true);
}

//-------------------------------------------------------

//-------------------------------------------------------
// Potion Functions

Potion::Potion(Game* pgame, Coordinate coord, int healAmount)
    :RItem(pgame, ColorChar('o', dngutil::RED), coord, "Potion",
        true, false, false, dngutil::TID::Potion, true, "Heals " + std::to_string(healAmount) + "hp")
{
    this->healAmount = healAmount;
}

Potion::Potion(const Potion& other, Game* game)
    :RItem(other, game)
{
    this->healAmount = other.healAmount;
}

void Potion::action(Player* player, unsigned int inventoryIndex)
{
    int healthbarLine = getPGame()->getVWin()->txtmacs.DIVIDER_LINES[2] - 1;
    int amountHealed = 0;

    soundEffect("RefillHealth.wav", true, true);       
    getPGame()->getVWin()->putcen(player->getHealthBar(), healthbarLine);
    Sleep(100);

    for (; amountHealed < healAmount &&
        static_cast<unsigned int>(player->getHp()) < player->getMaxhp();
        amountHealed++)
    {
        player->increaseHealth(1);
        getPGame()->getVWin()->putcen(player->getHealthBar(), healthbarLine);
        Sleep(100);
    }
    stopSound();

    std::string output = "You healed for ";
    output += std::to_string(amountHealed);
    output += " health";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    getPGame()->getVWin()->txtmacs.clearLine(healthbarLine);
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
// Key Functions

Key::Key(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('l', dngutil::BROWN), coord, "Key",
        true, false, false, dngutil::TID::Key, true, "Unlocks a door")
{
}

Key::Key(const Key& other, Game* game)
    :RItem(other, game)
{
}

void Key::action(Player* player, unsigned int inventoryIndex)
{
    std::string output = "You cannot use this right now";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

Item* Key::makeSaveInv(Game* game)
{
    return new Key(*this, game);
}

MapObject* Key::makeSave(Game* game)
{
    return new Key(*this, game);
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

void Primary::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;

    if (player->swapPrimary(player->getInventoryNotConst()[inventoryIndex]))
    {
        output = "Swapped active primary.";
    }
    else
    {
        output = "Failed to swap active primary, please report this bug.";
    }

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

const double& Primary::getDmgMultiplier() const
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

const std::string& Primary::getHitsound() const
{
    return hitsound;
}

bool Primary::hit() const
{
    return (random(99) < accuracy);
}

Item* Secondary::makeSaveInv(Game* game)
{
    return new Secondary(*this, game);
}

MapObject* Secondary::makeSave(Game* game)
{
    return new Secondary(*this, game);
}

void Secondary::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;

    if (player->swapSecondary(player->getInventoryNotConst()[inventoryIndex]))
    {
        output = "Swapped active secondary.";
    }
    else
    {
        output = "Failed to swap active secondary, please report this bug.";
    }

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

const int& Secondary::getDeflectTime() const
{
    return deflectTime;
}

const double& Secondary::getDmdReductMult() const
{
    return dmgReductMult;
}

//-------------------------------------------------------