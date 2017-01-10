#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "item.h"
#include "utilities.h"

#include <list>
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
    virtual std::string drop() = 0;
    virtual bool movement() = 0;
    virtual Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator it);
    virtual void printStats(int LONGEST_LINE_LENGTH, int startingCursorY);

    const std::string& getDeathSound() const;
    const std::string& getBattleMusic() const;
    const size_t& getExpGiven() const;

    virtual void deathSequence();

private:
    std::string battleMusic;
    std::string deathSound;
    size_t experienceGiven;
};

class REnemy : public Enemy
{
    REnemy(
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
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battleMusic, experienceGiven, deathSound) {}

    std::string drop();
    
    virtual bool movement();
};

class BEnemy : public Enemy
{
    BEnemy(
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
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battleMusic, experienceGiven, deathSound) {}

    std::string drop();
    
    bool movement();

};

#endif