#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "item.h"
#include "utilities.h"

#include <string>

class Game;

class Enemy : public Creature
{
public:
    Enemy(
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
    );

    Enemy(const Enemy& other, Game* game);

    virtual void printSelf() = 0;
    virtual std::string drop();
    virtual bool movement();

    virtual void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) const;

    const std::string& getDeathSound() const;
    const std::string& getBattleMusic() const;
    const size_t& getExpGiven() const;

    virtual void deathSequence();

private:
    std::string battleMusic;
    std::string deathSound;
    size_t experienceGiven;
};

#endif