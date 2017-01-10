#include "enemy.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "item.h"
#include "game.h"
#include "mapobject.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "room.h"

#include <Windows.h>
#include <string>

class Game;

//----------------------------------------------------------------
// Enemy Functions
Enemy::Enemy(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool rawoutput,
    dngutil::TID typeId,
    int hp,
    size_t att,
    size_t def,
    size_t lck,
    size_t spd,
    size_t lvl,
    Primary* primary,
    Secondary* secondary,
    std::string battleMusic,
    size_t experienceGiven,
    std::string deathSound
) : Creature(
    pgame,
    mapRep,
    coord,
    name,
    true,
    rawoutput,
    true,
    typeId,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    primary,
    secondary,
    dngutil::P_ENEMY
)
{
    this->battleMusic = battleMusic;
    this->deathSound = deathSound;
    this->experienceGiven = experienceGiven;
}

Enemy::Enemy(const Enemy& other, Game* game)
    :Creature(other, game)
{
    this->battleMusic = other.battleMusic;
    this->deathSound = other.deathSound;
    this->experienceGiven = other.experienceGiven;
}

const std::string& Enemy::getBattleMusic() const
{
    return battleMusic;
}

const std::string& Enemy::getDeathSound() const
{
    return deathSound;
}

const size_t& Enemy::getExpGiven() const
{
    return experienceGiven;
}

void Enemy::printStats(int LONGEST_LINE_LENGTH, int startingCursorY)
{
    int STATS_LENGTH = 13;

    VirtualWindow* vwin = getPGame()->getVWin();

    Coordinate vcursor(LONGEST_LINE_LENGTH + 1, startingCursorY);

    vwin->put(ColorString("*************", dngutil::YELLOW), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Level: " + std::to_string(getLvl()), dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Health: " + std::to_string(getHp()) + "    ", dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Attack: " + std::to_string(getAtt()) + "    ", dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Defense: " + std::to_string(getDef()) + "    ", dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Luck: " + std::to_string(getLck()) + "    ", dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Speed: " + std::to_string(getSpd()) + "    ", dngutil::RED), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString(getPrimary().getName(), dngutil::BLUE), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString(getSecondary().getName(), dngutil::BLUE), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("*************", dngutil::YELLOW), vcursor);
}

void Enemy::deathSequence()
{
    Player* player = getPGame()->getPlayer();

    int realExpGiven = experienceGiven + (6 * (getLvl() - (player->getLvl() ) ) );
    if (realExpGiven < 5)
    {
        realExpGiven = 5;
    }

    player->addExperience(realExpGiven);

    std::string dropType = drop();
    if (dropType != "NULL")
    {
        getPGame()->getVWin()->txtmacs.clearDivider("bottom");

        // TODO: item found sound effect musicPlayer->soundEffect("FindItem.wav", false, false);

        getPGame()->getVWin()->putcen(
            ColorString("The " + getName() + " drops a " + dropType, dngutil::CYAN),
            getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
        pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin()); 
    }
}

Collision Enemy::mapAction(MapObject* collider, std::list<MapObject*>::iterator it)
{
    if (collider == getPGame()->getPlayer() && isAggressive())
    {
        // TODO: game_pointer->getMusicPlayer()->stopMp3();

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        // TODO: game_pointer->getMusicPlayer()->soundEffect("EnterBattle.wav", false, true);
        if (battle(this))
        {
            getPGame()->getVWin()->txtmacs.displayOverworldInfo(getPGame());
            it++;
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
            removeFromMap(true);
            return Collision(false, false);
        }
        else
        {
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(collider);
            collider->removeFromMap(false);
            return Collision(true, true, true);
        }

    }
    return Collision(false, true);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// REnemy Functions
std::string REnemy::drop()
{
    Item* itemToAdd;
    if (random(10) == 0)
    {
        if (random(1) == 0)
        {
            itemToAdd = getPrimaryMemory();
            setPrimary(nullptr);
        }
        else
        {
            itemToAdd = getSecondaryMemory();
            setSecondary(nullptr);
        }

        getPGame()->getPlayer()->addToInventory(itemToAdd);
        return itemToAdd->getName();
    }
    else if (random(20) == 0)
    {
        itemToAdd = new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL);
        getPGame()->getPlayer()->addToInventory(itemToAdd);
        return itemToAdd->getName();
    }

    return "NULL";
}
bool REnemy::movement()
{
    if (!(getLastMoveTime() + ((dngutil::MAX_SPD * 3) - getSpd()) > GetTickCount()))
    {
        switch (random(50000))
        {
        case 0:
            return adjustPosition(dngutil::Movement::UP);
            break;
        case 1:
            return adjustPosition(dngutil::Movement::DOWN);
            break;
        case 2:
            return adjustPosition(dngutil::Movement::LEFT);
            break;
        case 3:
            return adjustPosition(dngutil::Movement::RIGHT);
            break;
        }
    }
    return false;
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// BEnemy functions
std::string BEnemy::drop()
{
    Item* itemToAdd;
    int numberOfPotions = random(2);
    if (numberOfPotions > 0)
    {
        for (int i = 0; i < numberOfPotions; i++)
        {
            itemToAdd = new Potion(getPGame(), Coordinate(-1, -1), static_cast<int>(dngutil::POTION_HEAL * 1.2));
            getPGame()->getPlayer()->addToInventory(itemToAdd);
        }
        return "Potion (" + std::to_string(numberOfPotions) + ")";
    }


    return "NULL";
}
bool BEnemy::movement()
{
    return false;
}

void BEnemy::deathSequence()
{
    // TODO: musicPlayer->startMp3("DefeatBoss.mp3");

    getPGame()->getVWin()->txtmacs.clearMapArea(true, 100);
    getPGame()->getVWin()->txtmacs.clearDivider("bottom");

    // Length of the sound effect adjusted for the screen clearing
    Sleep(5000);

    // TODO: musicPlayer->stopMp3();

    Enemy::deathSequence();
}
//----------------------------------------------------------------