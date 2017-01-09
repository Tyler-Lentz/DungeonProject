#ifndef CREATURE_H
#define CREATURE_H

#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"

#include <string>

class Game;
class Primary;
class Secondary;
class Enemy;

class Creature : public MapObject
{
public:
    Creature(
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
        Secondary* secondary,
        int priority
    );

    // Save Constructor
    Creature(const Creature& other, Game* game);

    // Deletes primary and secondary
    ~Creature();

    // Returns the healthbar of the creature as a ColorString, color
    // depends on percentage of health compared to max health.
    const ColorString& getHealthBar() const;

    virtual bool movement() = 0;
    virtual void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) const = 0;
    virtual bool battle(Enemy* enemy);

    const size_t& getMaxhp() const;
    const int& getHp() const;
    const size_t& getAtt() const;
    const size_t& getDef() const;
    const size_t& getLck() const;
    const size_t& getSpd() const;
    const size_t& getLvl() const;

    const size_t& increaseHealth(size_t amount);
    const size_t& decreaseHealth(size_t amount);

    const Primary& getPrimary() const;
    const Secondary& getSecondary() const;
private:
    // Primary is the weapon that deals damage
    Primary* primary;

    // Secondary is the shield that deflects and/or reduces damage taken
    Secondary* secondary;

    // Stats all creatures have
    size_t maxhp;
    int hp;
    size_t att;
    size_t def;
    size_t lck;
    size_t spd;
    size_t lvl;
};

#endif