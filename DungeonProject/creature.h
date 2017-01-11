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
    ColorString getHealthBar() const;

    virtual bool movement() = 0;
    virtual void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) = 0;
    virtual bool battle(MapObject* enemy);


    bool isDead() const;
    const size_t& getMaxhp() const;
    const int& getHp() const;
    const size_t& getAtt() const;
    const size_t& getDef() const;
    const size_t& getLck() const;
    const size_t& getSpd() const;
    const size_t& getLvl() const;
    const unsigned long& getLastMoveTime() const;

    void increaseMaxhp(size_t amount);
    void increaseAtt(size_t amount);
    void increaseDef(size_t amount);
    void increaseSpd(size_t amount);
    void increaseLvl(size_t amount);
    void increaseLck(size_t amount);

    void setMaxhp(size_t amount);
    void setHp(int amount);
    void setAtt(size_t amount);
    void setDef(size_t amount);
    void setSpd(size_t amount);
    void setLvl(size_t amount);
    void setLck(size_t amount);

    size_t increaseHealth(size_t amount);
    size_t decreaseHealth(size_t amount);

    const Primary& getPrimary() const;
    Primary*& getPrimaryMemory();
    void setPrimary(Primary* primary);
    const Secondary& getSecondary() const;
    Secondary*& getSecondaryMemory();
    void setSecondary(Secondary* secondary);

    bool adjustPosition(dngutil::Movement movement);

    int getDamageDealt(Creature* defender);
private:
    unsigned long lastMoveTime;

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