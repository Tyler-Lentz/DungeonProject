#include "enemy.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "item.h"

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
    size_t experienceGiven
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
    this->experienceGiven = experienceGiven;
}

Enemy::Enemy(const Enemy& other, Game* game)
    :Creature(other, game)
{
    this->battleMusic = other.battleMusic;
    this->experienceGiven = other.experienceGiven;
}

std::string Enemy::drop()
{
    // TODO:
    // random chance to drop something, returns string of name of item dropped
}

bool Enemy::battle(Enemy* enemy)
{
    // TODO:
    // standard battle function
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

const size_t& Enemy::getExpGiven() const
{
    return experienceGiven;
}

//----------------------------------------------------------------