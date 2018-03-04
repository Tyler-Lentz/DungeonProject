#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "item.h"
#include "utilities.h"
#include "soundfile.h"

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
        Mp3File battleMusic,
        unsigned int experienceGiven,
        WavFile deathSound,
        dngutil::EvType ev,
        dngutil::ClassType classType
    );

    virtual void printSelf() = 0;
    virtual void dodgeAnimation();
    virtual std::string drop() = 0;
    virtual void beginingCutscene() {}
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
    void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) override;

    const WavFile& getDeathSound() const;
    const Mp3File& getBattleMusic() const;
    const unsigned int& getExpGiven() const;

    virtual void deathSequence();

    virtual ColorString getBattleInfo() const;
private:
    Mp3File battleMusic;
    WavFile deathSound;
    unsigned int experienceGiven;
    dngutil::EvType ev;
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
        Mp3File battlemusic,
        unsigned int experienceGiven,
        WavFile deathSound,
        dngutil::EvType ev,
        dngutil::ClassType classType
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battlemusic, experienceGiven, deathSound, ev, classType) {}

    std::string drop();
    
    bool movement() override;
};

class SmartEnemy : public REnemy
{
public:
    SmartEnemy(
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
        Mp3File battlemusic,
        unsigned int experienceGiven,
        WavFile deathSound,
        dngutil::EvType ev,
        dngutil::ClassType classType
    ) : REnemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def,
        lck, spd, lvl, primary, secondary, battlemusic, experienceGiven,
        deathSound, ev, classType
    ) {}

    bool movement() final override;
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

    void printSelf() override;
};

class BloodSkeleton : public SmartEnemy
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

    void printSelf() override;
};

class LSKnight : public SmartEnemy
{
public:
    LSKnight(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class SSKnight : public SmartEnemy
{
public:
    SSKnight(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class Mage : public SmartEnemy
{
public:
    Mage(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class Imp : public SmartEnemy
{
public:
    Imp(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class Cultist : public SmartEnemy
{
public:
    Cultist(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class Bowman : public REnemy
{
public:
    Bowman(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class DesertGoblin : public REnemy
{
public:
    DesertGoblin(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class Robber : public SmartEnemy
{
public:
    Robber(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
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
        Mp3File battlemusic,
        unsigned int experienceGiven,
        WavFile deathSound,
        dngutil::EvType ev,
        dngutil::ClassType classType
    ) : Enemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battlemusic, experienceGiven, deathSound, ev, classType) {}

    std::string drop() override;
    
    bool movement() override;

    void deathSequence() override;
};

class SkeletonKing : public BEnemy 
{
public:
    SkeletonKing(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    ColorString getBattleInfo() const override;

    void printSelf() override;
};

class FlameHorse : public BEnemy
{
public:
    FlameHorse(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    ColorString getBattleInfo() const override;
    void printSelf() override;
};

class GhostHorse : public BEnemy
{
public:
    GhostHorse(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    ColorString getBattleInfo() const override;
    void printSelf() override;
};

class Basilisk : public BEnemy
{
public:
    Basilisk(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    std::string drop() override;
    ColorString getBattleInfo() const override;
    void printSelf() override;
};

class DesertGryphon : public BEnemy
{
public:
    DesertGryphon(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    std::string drop() override;
    ColorString getBattleInfo() const override;
    void printSelf() override;
};

class TrueZorlock : public BEnemy
{
public:
    TrueZorlock(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    ColorString getBattleInfo() const override;
    void printSelf() override;
    void beginingCutscene() override;
    void deathSequence() override;
};

class WaterHorse : public BEnemy
{
public:
    WaterHorse(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
    ColorString getBattleInfo() const override;
};

class SegEnemy : public BEnemy
{
public:
    SegEnemy(
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
        Mp3File battlemusic,
        unsigned int experienceGiven,
        WavFile deathSound,
        dngutil::EvType ev,
        dngutil::ClassType classType
    ) : BEnemy(pgame, mapRep, coord, name, rawoutput, typeId, hp, att, def, lck, spd,
        lvl, primary, secondary, battlemusic, experienceGiven, deathSound, ev, classType)
    {
        first = false;
    }

    void deathSequence() override;
    bool battle(MapObject* enemy);

    bool isFirst() const
    {
        return first;
    }

    void setFirst()
    {
        first = true;
    }
private:
    bool first;
};

class MegaBeastPhase1 : public SegEnemy
{
public:
    MegaBeastPhase1(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    ColorString getBattleInfo() const override;
    void printSelf() override;
    void deathSequence() override;
};


class MegaBeastPhase2 : public SegEnemy
{
public:
    MegaBeastPhase2(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};


class ZorlockImagePhase1 : public SegEnemy
{
public:
    ZorlockImagePhase1(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
    void beginingCutscene() override;
};

class ZorlockImagePhase2 : public SegEnemy
{
public:
    ZorlockImagePhase2(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
    void deathSequence() override;
};

class DragonTail : public SegEnemy
{
public:
    DragonTail(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    ColorString getBattleInfo() const override;
    void printSelf() override;

};

class DragonWings : public SegEnemy
{
public:
    DragonWings(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );    
    void printSelf() override;

};

class DragonHead : public SegEnemy
{
public:
    DragonHead(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    void printSelf() override;
};

class ForestDragonPhase1 : public SegEnemy
{
public:
    ForestDragonPhase1(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    ColorString getBattleInfo() const;
    void printSelf() override;
};

class ForestDragonPhase2 : public SegEnemy
{
public:
    ForestDragonPhase2(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );
    void printSelf() override;
};


class BloodjawPhase1 : public SegEnemy
{
public:
    BloodjawPhase1(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    ColorString getBattleInfo() const override;
    void printSelf() override;

};

class BloodjawPhase2 : public SegEnemy
{
public:
    BloodjawPhase2(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;

};

class BloodjawPhase3 : public SegEnemy
{
public:
    BloodjawPhase3(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};

class EvilBeastPhase1 : public SegEnemy
{
public:
    EvilBeastPhase1(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    ColorString getBattleInfo() const override;
    void printSelf() override;

};

class EvilBeastPhase2 : public SegEnemy
{
public:
    EvilBeastPhase2(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;

};

class EvilBeastPhase3 : public SegEnemy
{
public:
    EvilBeastPhase3(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
    void deathSequence() override;
};

class EvilBeastPhase4 : public SegEnemy
{
public:
    EvilBeastPhase4(
        Game* pgame,
        Coordinate coord,
        int hp,
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl
    );

    void printSelf() override;
};
//----------------------------------------------------------------
// segboss
class Segboss
{
public:
    Segboss(std::vector<SegEnemy*> segments, Game* game_pointer);
    ~Segboss();

    bool segmentedBattle(Player* player);
private:
    std::vector<SegEnemy*> segments;
    Game* pgame;
};
//----------------------------------------------------------------

#endif