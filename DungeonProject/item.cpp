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
#include "creature.h"
#include "spell.h"

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
// Mana Potion Functions

ManaPotion::ManaPotion(Game* pgame, Coordinate coord, int healAmount)
    :RItem(pgame, ColorChar('o', dngutil::BLUE), coord, "Mana Potion",
        true, false, false, dngutil::TID::ManaPotion, true, "Restores " + std::to_string(healAmount) + "mana")
{
    this->healAmount = healAmount;
}

void ManaPotion::action(Player* player, unsigned int inventoryIndex)
{
    int healthbarLine = getPGame()->getVWin()->txtmacs.DIVIDER_LINES[2] - 1;
    int amountHealed = 0;

    playSound(WavFile("RefillHealth", true, true));
    getPGame()->getVWin()->putcen(player->getManaBar(), healthbarLine);
    Sleep(100);

    for (; amountHealed < healAmount &&
        static_cast<unsigned int>(player->getMana()) < player->getMaxMana();
        amountHealed++)
    {
        player->increaseMana(1);
        getPGame()->getVWin()->putcen(player->getManaBar(), healthbarLine);
        if (!keypress(VK_RETURN))
        {
            Sleep(30);
        }
    }

    stopSound(SoundType::WAV);

    std::string output = "You restored ";
    output += std::to_string(amountHealed);
    output += " mana";

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

    stopSound(SoundType::MP3);
    playSound(WavFile("BasiliskHorn", false, false));

    for (auto& i : getPGame()->getActiveRoom()->getCreatureList())
    {
        if (i->getTypeId() != dngutil::TID::Player)
        {
            i->setLastMoveTime(GetTickCount());
            i->increaseLastMoveTime(15000);
        }
    }
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
        player->upgradePrimary(dngutil::TID::HerosBlade);

        output = "The Hero's Blade has upgraded to level 2!";
    }
    else if (player->hasItem(dngutil::TID::HerosBlade2, true))
    {
        output = "You have already upgraded this item";
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

const bool& Primary::getQuickAttack() const
{
    return canQuickAttack;
}

const WavFile& Primary::getHitsound() const
{
    return hitsound;
}

bool Primary::hit(Creature* enemy)
{
    if (this != getPGame()->getPlayer()->getPrimaryMemory())
    {
        return (random(99) < this->accuracy);
    }
    else
    {  
        int adjustedAccuracy = (this->accuracy / 10);
        std::vector<char> hitAmount;
        int badSpaces = (10 - adjustedAccuracy);
        for (int i = 0; i < adjustedAccuracy; i++)
        {
            hitAmount.push_back('O');
        }
        int badSpacesHalved = (badSpaces / 2);
        int otherSideSpaces = (badSpaces - badSpacesHalved);
        // bad spaces leave it up to a chance
        for (int i = 0; i < badSpacesHalved; i++)
        {
            auto itr = hitAmount.begin();
            hitAmount.insert(itr, 'x');
        }
        for (int i = 0; i < otherSideSpaces; i++)
        {
            hitAmount.push_back('x');
        }

        // add extras which have a 0% chance for hitting
        for (int i = 0; i < 6; i++)
        {
            auto itr = hitAmount.begin();
            hitAmount.insert(itr, 'X');
        }
        for (int i = 0; i < 6; i++)
        {
            hitAmount.push_back('X');
        }

        unsigned int activeSlot = random(0, 19);
        VirtualWindow* vwin = getPGame()->getVWin();
        vwin->txtmacs.clearDivider("bottom");

        int timerLine = vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 2; // line on the screen where the timer is put
        int leftMostSlot = ((dngutil::CONSOLEX - hitAmount.size()) / 2);
        auto startTime = GetTickCount();
        int speedDifference = (getPGame()->getPlayer()->getSpd() - enemy->getSpd());
        int attackDuration = (1500 + (speedDifference * 20)); // in miliseconds
        if (attackDuration < 500)
        {
            attackDuration = 500;
        }

        vwin->putcen(ColorString("- SPACEBAR -", dngutil::LIGHTCYAN), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE +1);

        vwin->put(ColorChar('<', dngutil::LIGHTCYAN), Coordinate(leftMostSlot - 1, vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE));

        vwin->put(ColorChar('>', dngutil::LIGHTCYAN), Coordinate(leftMostSlot + hitAmount.size(), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE));

        int centerSpace = hitAmount.size() / 2;
        int currentCritSpot = centerSpace;
        auto lastCritReset = GetTickCount64();
        do
        {
            activeSlot++;
            if (activeSlot > (hitAmount.size() - 1))
            {
                activeSlot = 0;
            }
            for (unsigned int i = 0; i < hitAmount.size(); i++) //19
            {
                ColorChar slot;
                if (i == activeSlot)
                {
                    slot.color = dngutil::WHITE;
                }
                else
                {
                    switch (hitAmount[i])
                    {
                    case 'O':
                        slot.color = dngutil::GREEN;
                        break;

                    case 'x':
                        slot.color = dngutil::YELLOW;
                        break;

                    case 'X':
                        slot.color = dngutil::RED;
                        break;

                    default:
                        slot.color = dngutil::LIGHTGREEN;
                        break;
                    }
                }
                slot.character = hitAmount[i];
                if (activeSlot == 0 && lastCritReset + 20 < GetTickCount64()) // if it is reset and starting from the leftmost spot, reset the crit place half the time
                {
                    currentCritSpot += random(-1,1);
                    lastCritReset = GetTickCount64();
                }
                if (currentCritSpot == i) // if in the center
                {
                    slot.color = dngutil::LIGHTMAGENTA; // set equal to light magenta to signify it should be a crit
                }

                vwin->put(slot, Coordinate(leftMostSlot + i, vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE));
            }
            vwin->putcen(ColorString("   " + std::to_string(attackDuration - (GetTickCount() - startTime)) + "   ", dngutil::LIGHTGRAY), timerLine);
            if (hitAmount[activeSlot] == 'O')
            {
                Sleep(25);
            }
            else
            {
                Sleep(25);
            }

            if (GetTickCount() > (startTime + attackDuration))
            {
                return false;
            }

        } while (!keypress(VK_SPACE));

        vwin->txtmacs.clearLine(timerLine);
        vwin->txtmacs.clearLine(timerLine-1);
        if (hitAmount[activeSlot] == 'X')
        {
            return false;
        }
        else if (hitAmount[activeSlot] == 'x')
        {
            return (random(99) < this->accuracy);
        }
        else // (hitAmount[activeSlot] == 'O');
        {
            if (currentCritSpot == activeSlot) // if in the center on the crit space
            {
                getPGame()->getPlayer()->setCertainCrit(true);
            }
            return true;
        }
    }
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

//-------------------------------------------------------
// Spellbook Functions

Spellbook::Spellbook(Game* pgame, Coordinate coord)
    :RItem(pgame, ColorChar(static_cast<unsigned char>(222), dngutil::MAGENTA), coord, "Spellbook",
        true, false, false, dngutil::TID::Spellbook, false, "Used to cast spells one has heard of")
{
}

Spellbook::~Spellbook()
{
    for (auto& i : spellList)
    {
        delete i;
    }
}

std::vector<Spell*> Spellbook::getSpellList() const
{
    return spellList;
}

void Spellbook::addSpell(Spell* spell)
{
    spellList.push_back(spell);
}

void Spellbook::action(Player* player, unsigned int inventoryIndex)
{
    
    while (keypress(VK_RETURN));

    std::string output = "";
    
    VirtualWindow* vwin = getPGame()->getVWin();
    
    int currentLine = vwin->txtmacs.DIVIDER_LINES[1] + 3;

    vwin->txtmacs.clearMapArea(false, 0);
    

    int currentMana = player->getMana();
    int currentMaxMana = player->getMaxMana();
    std::string manaIntsToText = std::to_string(currentMana) + "/" + std::to_string(currentMaxMana);
    ColorString manaBar = player->getManaBar();
    ColorString manaTxt(manaIntsToText, dngutil::WHITE);

    vwin->putcen(manaBar, currentLine);
    vwin->putcen(manaTxt, ++currentLine);
    currentLine++;
    vwin->putcenSlowScroll(ColorString("SPELL BOOK", dngutil::MAGENTA), ++currentLine);
    vwin->putcen(ColorString("--------------------------------------------------------------", dngutil::BROWN), ++currentLine);

    int spellNumber = 1;

    for (Spell* spell : spellList)
    {
        std::string name = spell->getSpellName();
        std::string description = spell->getSpellDescription();
        int manaReq = spell->getManaReq();

        std::string firstLine = std::to_string(spellNumber) + ". " + name + "(" + std::to_string(manaReq) + " mana) " + description;
        vwin->putcen(ColorString(firstLine, dngutil::LIGHTGRAY), ++currentLine);
        spellNumber++;
    }
    vwin->putcen(ColorString("--------------------------------------------------------------", dngutil::BROWN), ++currentLine);

    vwin->put(ColorString("Spell Number: ", dngutil::LIGHTGRAY), Coordinate(dngutil::CONSOLEX / 4, vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE));
    std::string spell = safeInput(1);

    int spellIndex = -1;
    if (spell == "1" && spellList.size() >= 1)
    {
        spellIndex = 1;
    }
    else if (spell == "2" && spellList.size() >= 2)
    {
        spellIndex = 2;
    }
    else if (spell == "3" && spellList.size() >= 3)
    {
        spellIndex = 3;
    }
    else if (spell == "4" && spellList.size() >= 4)
    {
        spellIndex = 4;
    }
    else if (spell == "5" && spellList.size() >= 5)
    {
        spellIndex = 5;
    }
    else if (spell == "6" && spellList.size() >= 6)
    {
        spellIndex = 6;
    }
    else
    {
        // still equal -1
        playSound(WavFile("PuzzleError", false, true));
        output = "Invalid Spell";
    }

    if (spellIndex != -1)
    {
        Spell* theSpell = spellList[spellIndex-1];
        int manaNeeded = theSpell->getManaReq();

        if (player->useMana(manaNeeded)) // successful cast, mana is used
        {
            
            theSpell->playCastSound();
            output += theSpell->castSpell(player, player->getPGame());
        }
        else
        {
            output = "Not enough Mana...";
            playSound(WavFile("PuzzleError", false, true));
        }
    }

    vwin->txtmacs.clearLine(vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
    vwin->putcen(ColorString(output, dngutil::LIGHTMAGENTA), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

}

//-------------------------------------------------------