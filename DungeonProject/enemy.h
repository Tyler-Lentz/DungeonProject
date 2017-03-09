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
    virtual std::string drop() = 0;
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

    void printSelf() override;
};

class LSKnight : public REnemy
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

class SSKnight : public REnemy
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

class Mage : public REnemy
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

    void printSelf() override;
};

class DungeonBeast : public BEnemy
{
public:
    DungeonBeast(
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

    void printSelf() override;
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

class PitDragon : public BEnemy
{
public:
    PitDragon(
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
    void deathSequence() override;
};



class GryphonPhase1 : public SegEnemy
{
public:
    GryphonPhase1(
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

class GryphonPhase2 : public SegEnemy
{
public:
    GryphonPhase2(
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

class GryphonPhase3 : public SegEnemy
{
public:
    GryphonPhase3(
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


class ReaperPhase1 : public SegEnemy
{
public:
    ReaperPhase1(
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

class ReaperPhase2 : public SegEnemy
{
public:
    ReaperPhase2(
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

class ReaperPhase3 : public SegEnemy
{
public:
    ReaperPhase3(
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


class MaskPhase1 : public SegEnemy
{
public:
    MaskPhase1(
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

class MaskPhase2 : public SegEnemy
{
public:
    MaskPhase2(
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

class MaskPhase3 : public SegEnemy
{
public:
    MaskPhase3(
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

    void deathSequence() override;
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

    void deathSequence() override;
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