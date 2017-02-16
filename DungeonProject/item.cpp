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
        if (!keypress(VK_RETURN))
        {
            Sleep(30);
        }
    }

    stopSound();

    std::string output = "You healed for ";
    output += std::to_string(amountHealed);
    output += " health";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    getPGame()->getVWin()->txtmacs.clearLine(healthbarLine);
}

//-------------------------------------------------------

//-------------------------------------------------------
// Flute Functions

Flute::Flute(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('|', dngutil::WHITE), coord, "Flute",
        true, false, false, dngutil::TID::Flute, false, "Damages everything in a room for 15 once per room.")
{

}


bool Flute::isUsed(Coordinate coord)
{
    for (auto& i : usedRooms)
    {
        if (coord == i)
        {
            return true;
        }
    }
    return false;
}

void Flute::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;
    Coordinate mapCoord = getPGame()->getActiveRoom()->getRoomInfo().mapCoord;
    if (player->getHp() <= HEALTH_DECREASE)
    {
        output = "Using the flute would kill you";
    }
    else if (!isUsed(mapCoord))
    {
        soundEffect("Flute.wav", false, false);
        usedRooms.push_back(mapCoord);

        for (auto& i : getPGame()->getActiveRoom()->getCreatureList())
        {
            i->decreaseHealth(HEALTH_DECREASE);
        }

        output = "You damage everything in the room for " + std::to_string(HEALTH_DECREASE) + " hp.";
    }
    else
    {
        output = "The flute has no effect here";
    }

    int line = getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), line);
}

//-------------------------------------------------------

//-------------------------------------------------------
// Heros Claim Functions

HerosClaim::HerosClaim(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('*', dngutil::WHITE), coord, "Hero's Claim",
        true, false, false, dngutil::TID::HerosClaim, true, "Gives an ultimate weapon for your class")
{

}

void HerosClaim::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;

    
    ColorChar colorchar;
    std::string primaryname, description;
    std::string hitsound;
    double dmgmult;
    int attSpeed;
    int accuracy;
    bool startReady;
    dngutil::ClassType classType;

    switch (getPGame()->getPlayer()->getPrimary().getClass())
    {
    case dngutil::ClassType::KNIGHT:
    colorchar = ColorChar('T', dngutil::WHITE);
    primaryname = "Hero's Sword";
    description = "The sword of a real hero";
    hitsound = "Attack3.wav";
    dmgmult = 1.85;
    attSpeed = 4;
    accuracy = 85;
    startReady = false;
    classType = dngutil::ClassType::KNIGHT;
    break;
    case dngutil::ClassType::WIZARD:
    colorchar = ColorChar('I', dngutil::WHITE);
    primaryname = "Hero's Staff";
    description = "The staff of a real hero";
    hitsound = "MagicAttack1.wav";
    dmgmult = 2;
    attSpeed = 4;
    accuracy = 100;
    startReady = false;
    classType = dngutil::ClassType::WIZARD;
    break;
    case dngutil::ClassType::RANGER:
    colorchar = ColorChar('t', dngutil::WHITE);
    primaryname = "Hero's Revolver";
    description = "The gun of a real hero";
    hitsound = "GunAttack1.wav";
    dmgmult = 1.8;
    attSpeed = 3;
    accuracy = 75;
    startReady = true;
    classType = dngutil::ClassType::RANGER;
    break;
    default: // you are adventurer??
        colorchar = ColorChar('|', dngutil::MAGENTA);
        primaryname = "Adventurer's Death Stick";
        description = "How did you do this.";
        hitsound = "GunAttack1.wav";
        dmgmult = 99;
        attSpeed = 1;
        accuracy = 101;
        startReady = false;
        classType = dngutil::ClassType::ADVENTURER;
        break;
    }

    Primary* primary = new Primary(
    getPGame(),
    colorchar,
    Coordinate(-1, -1),
    primaryname,
    false,
    dmgmult,
    attSpeed,
    accuracy,
    startReady,
    description,
    hitsound,
    classType
    );

    player->addToInventory(primary);

    output = "You gain a sense of power";

    int line = getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), line);
}

//-------------------------------------------------------


//-------------------------------------------------------
// Key Functions

Key::Key(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('l', dngutil::WHITE), coord, "Key",
        true, false, false, dngutil::TID::Key, false, "Unlocks a door")
{
}

void Key::action(Player* player, unsigned int inventoryIndex)
{
    std::string output = "You cannot use this right now";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

//-------------------------------------------------------

//-------------------------------------------------------
// Primary and Secondary Functions

void Primary::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;

    if (player->swapPrimary(player->getInventoryNotConst()[inventoryIndex]))
    {
        output = "Swapped active primary.";
    }
    else
    {
        output = "You cannot equip this primary.";
    }

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

const double& Primary::getDmgMultiplier() const
{
    return dmgMultiplier;
}

dngutil::ClassType Primary::getClass() const
{
    return classType;
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

Secondary::Secondary(
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
    this->deflectTime = static_cast<int>(deflectTime * pgame->getDifficulty().deflectModifier);
    this->defenseBoost = dmgReductMult;
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

const double& Secondary::getDefenseBoost() const
{
    return defenseBoost;
}

//-------------------------------------------------------

//-------------------------------------------------------
// MagicalPotion Functions

MagicalPotion::MagicalPotion(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('o', dngutil::YELLOW), coord, "Magical Potion",
        true, false, false, dngutil::TID::MagicalPotion, true, "Restores hp to max hp or heals 3/4 hp on death")
{
}

void MagicalPotion::action(Player* player, unsigned int inventoryIndex)
{
    int healthbarLine = getPGame()->getVWin()->txtmacs.DIVIDER_LINES[2] - 1;

    soundEffect("RefillHealth.wav", true, true);
    getPGame()->getVWin()->putcen(player->getHealthBar(), healthbarLine);
    Sleep(100);

    while (static_cast<unsigned int>(player->getHp()) < player->getMaxhp())
    {
        player->increaseHealth(1);
        getPGame()->getVWin()->putcen(player->getHealthBar(), healthbarLine);
        if (!keypress(VK_RETURN))
        {
            Sleep(30);
        }
    }
    stopSound();

    std::string output = "You healed to full health";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    getPGame()->getVWin()->txtmacs.clearLine(healthbarLine);
}

//-------------------------------------------------------