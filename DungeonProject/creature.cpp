#include "creature.h"
#include "mapobject.h"
#include "utilities.h"
#include "item.h"

#include <string>

//------------------------------------------------------------
// Creature Functions
Creature::Creature(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId,
    int hp,
    size_t att,
    size_t def,
    size_t lck,
    size_t spd,
    size_t lvl,
    Primary* primary,
    Secondary* secondary
)
    :MapObject(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId)
{
    this->hp = hp;
    this->maxhp = hp;
    this->att = att;
    this->def = def;
    this->lck = lck;
    this->spd = spd;
    this->lvl = lvl;

    this->primary = primary;
    this->secondary = secondary;
}

// Save Constructor
Creature::Creature(const Creature& other, Game* game)
    :MapObject(other, game)
{
    this->primary = new Primary(*primary, game);
    this->secondary = new Secondary(*secondary, game);

    this->maxhp = other.hp;
    this->att = other.att;
    this->def = other.def;
    this->lck = other.lck;
    this->spd = other.spd;
    this->lvl = other.lvl;
}
Creature::~Creature()
{
    if (primary != nullptr)
    {
        delete primary;
    }
    if (secondary != nullptr)
    {
        delete secondary;
    }
}

const size_t& Creature::getMaxhp() const
{
    return maxhp;
}

const int& Creature::getHp() const
{
    return hp;
}

const size_t& Creature::getAtt() const
{
    return att;
}

const size_t& Creature::getDef() const
{
    return def;
}

const size_t& Creature::getLck() const
{
    return lck;
}

const size_t& Creature::getSpd() const
{
    return spd;
}

const size_t& Creature::getLvl() const
{
    return lvl;
}

const Primary& Creature::getPrimary() const
{
    return *primary;
}

const Secondary& Creature::getSecondary() const
{
    return *secondary;
}

const size_t& Creature::increaseHealth(size_t amount)
{
    size_t tempHealth = hp;
    hp += amount;
    if (hp > maxhp)
    {
        hp = maxhp;
    }
    return hp - tempHealth;
}

const size_t& Creature::decreaseHealth(size_t amount)
{
    hp -= amount;
    return amount;
}

const ColorString& Creature::getHealthBar() const
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / (double)maxhp);

    int numOfCircles = (int)(scaleFactor * hp);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    std::string healthbar = temp + (std::string((size_t)numOfCircles, '='));

    int color;
    if (hp > static_cast<int>(maxhp * .66))
    {
        color = dngutil::GREEN;
    }
    else if (hp > static_cast<int>(maxhp * .33))
    {
        color = dngutil::YELLOW;
    }
    else
    {
        color = dngutil::RED;
    }

    return ColorString(healthbar, color);
}

bool Creature::battle(Enemy* enemy)
{
    // TODO: include enemy
    // TODO: implement this
}

//------------------------------------------------------------