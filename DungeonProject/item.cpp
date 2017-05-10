#include "item.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "player.h"
#include "game.h"
#include "virtualwindow.h"
#include "room.h"
#include "soundfile.h"

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
        playSound(WavFile("PickupItem", false, true));
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

    playSound(WavFile("RefillHealth", true, true));       
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

    stopSound(SoundType::WAV);

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
    :RItem(pgame, ColorChar('!', dngutil::MAGENTA), coord, "Spirit Flute",
        true, false, false, dngutil::TID::Flute, false, "Damages everything in a room for 30 once")
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
        playSound(WavFile("Flute", false, false));
        usedRooms.push_back(mapCoord);

        for (auto& i : getPGame()->getActiveRoom()->getCreatureList())
        {
            i->decreaseHealth(HEALTH_DECREASE);
            if (i->isDead())
            {
                i->setHp(1);
            }
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
// BasiliskHorn Functions

BasiliskHorn::BasiliskHorn(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('!', dngutil::GREEN), coord, "Basilisk Horn",
        true, false, false, dngutil::TID::BasiliskHorn, false, "When blown, stuns all enemies in the room for 15 seconds.")
{

}

void BasiliskHorn::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;
    Coordinate mapCoord = getPGame()->getActiveRoom()->getRoomInfo().mapCoord;

    playSound(WavFile("BasiliskHorn", false, false));
    output = "You stun everything in the room for 15 seconds starting now.";

    for (auto& i : getPGame()->getActiveRoom()->getCreatureList())
    {
        if (i->getTypeId() != dngutil::TID::Player)
        {
            i->setLastMoveTime(GetTickCount());
            i->increaseLastMoveTime(15000);
        }
    }

    int line = getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), line);
}

//-------------------------------------------------------

//-------------------------------------------------------
// GodStone Functions

GodStone::GodStone(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('*', dngutil::YELLOW), coord, "God's Stone",
        true, false, false, dngutil::TID::GodStone, false, "Enhances the Hero's Blade when used")
{

}

void GodStone::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;
    Coordinate mapCoord = getPGame()->getActiveRoom()->getRoomInfo().mapCoord;

    if (player->hasItem(dngutil::TID::HerosBlade, true))
    {
        if (player->getPrimary().getAccuracy() != 100)
        {
            playSound(WavFile("FindItem", false, false));
            player->getPrimaryMemory()->setAccuracy(100);
            output = "The Hero's Blade now has 100% accuracy!";
        }
        else
        {
            output = "You have already enhanced the blade";
        }
    }
    else
    {
        output = "You must have the blade equipped for the stone to work.";
    }

    int line = getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), line);
}

//-------------------------------------------------------



//-------------------------------------------------------
// SunCharm Functions

SunCharm::SunCharm(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar('o', dngutil::YELLOW), coord, "Sun Charm",
        true, false, false, dngutil::TID::SunCharm, true, "Guarentees your next attack to hit. 1 time use.")
{

}

void SunCharm::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;
    Coordinate mapCoord = getPGame()->getActiveRoom()->getRoomInfo().mapCoord;

    getPGame()->getPlayer()->setCanMiss(false);
    output = "Your next hit is guarenteed to hit.";

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
    WavFile hitsound;
    double dmgmult;
    int attSpeed;
    int accuracy;
    bool startReady;
    dngutil::ClassType classType;

    switch (getPGame()->getPlayer()->getClass())
    {
    case dngutil::ClassType::KNIGHT:
    colorchar = ColorChar('T', dngutil::WHITE);
    primaryname = "Hero's Sword";
    description = "The sword of a real hero";
    hitsound = WavFile("Attack3", false, false);
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
    hitsound = WavFile("MagicAttack1", false, false);
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
    hitsound = WavFile("GunAttack1", false, false);
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
        hitsound = WavFile("GunAttack1", false, false);
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
        output = "An error occured trying to swap weapons - please report this bug";
    }

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

void Primary::setAccuracy(int amount)
{
    accuracy = amount;
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

const WavFile& Primary::getHitsound() const
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

void Secondary::adjustDeflectTime(double modifier)
{
    deflectTime = static_cast<int>(deflectTime * modifier);
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

    playSound(WavFile("RefillHealth", true, true));
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
    stopSound(SoundType::WAV);

    std::string output = "You healed to full health";

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    getPGame()->getVWin()->txtmacs.clearLine(healthbarLine);
}

//-------------------------------------------------------