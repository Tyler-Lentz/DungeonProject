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
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl,
        Primary* primary,
        Secondary* secondary,
        std::string battleMusic,
        unsigned int experienceGiven,
        std::string deathSound
    );

    Enemy(const Enemy& other, Game* game);

    virtual void printSelf() = 0;
    virtual std::string drop() = 0;
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
    void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) override;

    const std::string& getDeathSound() const;
    const std::string& getBattleMusic() const;
    const unsigned int& getExpGiven() const;

    virtual void deathSequence();

private:
    std::string battleMusic;
    std::string deathSound;
    unsigned int experienceGiven;
};

class REnemy : public Enemy
{
public:
    REnemy(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        dngutil::TID typeId,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl,
        Primary* primary,
        Secondary* secondary,
        std::string battleMusic,
        unsigned int experienceGiven,
        std::string deathSound
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battleMusic, experienceGiven, deathSound) {}

    REnemy(const REnemy& other, Game* game) :Enemy(other, game) {}

    std::string drop();
    
    bool movement() override;
};

class Skeleton : public REnemy
{
public:
    Skeleton(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    Skeleton(const Skeleton& other, Game* game) : REnemy(other, game) {}

    void printSelf() override;

    MapObject* makeSave(Game* game) override
    {
        return new Skeleton(*this, game);
    }
};

class BloodSkeleton : public REnemy
{
public:
    BloodSkeleton(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    BloodSkeleton(const BloodSkeleton& other, Game* game) : REnemy(other, game) {}

    void printSelf() override;

    MapObject* makeSave(Game* game) override
    {
        return new BloodSkeleton(*this, game);
    }
};

class BEnemy : public Enemy
{
public:
    BEnemy(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        dngutil::TID typeId,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl,
        Primary* primary,
        Secondary* secondary,
        std::string battleMusic,
        unsigned int experienceGiven,
        std::string deathSound
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battleMusic, experienceGiven, deathSound) {}

    BEnemy(const BEnemy& other, Game* game) :Enemy(other, game) {}


    std::string drop() override;
    
    bool movement() override;

    void deathSequence() override;
};

class LargeSkeleton : public BEnemy
{
public:
    LargeSkeleton(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    LargeSkeleton(const LargeSkeleton& other, Game* game) : BEnemy(other, game) {}

    void printSelf() override;

    MapObject* makeSave(Game* game) override
    {
        return new LargeSkeleton(*this, game);
    }
};

#endif