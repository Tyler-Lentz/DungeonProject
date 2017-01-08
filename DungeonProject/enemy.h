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
        size_t experienceGiven
    );

    Enemy(const Enemy& other, Game* game);

    virtual bool battle(Enemy* enemy);
    virtual void printSelf() = 0;
    virtual std::string drop();
    virtual bool movement();

    const std::string& getBattleMusic() const;
    const size_t& getExpGiven() const;

private:
    std::string battleMusic;
    size_t experienceGiven;
};

#endif