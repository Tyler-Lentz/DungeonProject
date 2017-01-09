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

std::string Enemy::drop()
{
    // TODO:
    // random chance to drop something, returns string of name of item dropped
    // this will be the default function and may be overloaded for bosses
    // remember to return "NULL" is there is no drop
}

bool Enemy::movement()
{
    // TODO:
    // standard enemy movement
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

void Enemy::printStats(int LONGEST_LINE_LENGTH, int startingCursorY) const
{
    // TODO: this
}

void Enemy::deathSequence()
{
    /* TODO: make boss be a derived class of enemy, then put this in their deathsequence
             call this first, then the regular enemies if it should
    if (enemy->isBoss())
    {
        musicPlayer->startMp3("DefeatBoss.mp3");

        game_pointer->getTextOutput()->clearMapArea(true, 100);
        game_pointer->getTextOutput()->clearDivider("bottom");

        // Length of the sound effect adjusted for the screen clearing
        Sleep(5000);

        musicPlayer->stopMp3();
    }*/

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

//----------------------------------------------------------------