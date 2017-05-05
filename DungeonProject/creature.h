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

struct Damage
{
    int damage;
    int damageDeflected;

    Damage(int damage, int damageDeflected)
    {
        this->damage = damage;
        this->damageDeflected = damageDeflected;
    }
};

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
        unsigned int att,
        unsigned int def,
        unsigned int lck,
        unsigned int spd,
        unsigned int lvl,
        Primary* primary,
        Secondary* secondary,
        int priority,
        dngutil::ClassType classType,
        bool prematurecheck = false
    );

    // Deletes primary and secondary
    ~Creature();

    // Returns the healthbar of the creature as a ColorString, color
    // depends on percentage of health compared to max health.
    ColorString getHealthBar() const;

    virtual bool movement() = 0;
    virtual void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) = 0;
    virtual bool battle(MapObject* enemy);

    virtual void levelUpStats();

    bool isDead() const;
    const unsigned int& getMaxhp() const;
    const int& getHp() const;
    const unsigned int& getAtt() const;
    const unsigned int& getDef() const;
    const unsigned int& getLck() const;
    const unsigned int& getSpd() const;
    const unsigned int& getLvl() const;
    const unsigned long& getLastMoveTime() const;

    void increaseMaxhp(unsigned int amount);
    void increaseAtt(unsigned int amount);
    void increaseDef(unsigned int amount);
    void increaseSpd(unsigned int amount);
    void increaseLvl(unsigned int amount);
    void increaseLck(unsigned int amount);

    void setMaxhp(unsigned int amount);
    void setHp(int amount);
    void setAtt(unsigned int amount);
    void setDef(unsigned int amount);
    void setSpd(unsigned int amount);
    void setLvl(unsigned int amount);
    void setLck(unsigned int amount);

    unsigned int increaseHealth(unsigned int amount);
    unsigned int decreaseHealth(unsigned int amount);

    void increaseLastMoveTime(unsigned long amount);

    const Primary& getPrimary() const;
    void setPrimary(Primary* primary);
    const Secondary& getSecondary() const;
    void setSecondary(Secondary* secondary);

    bool adjustPosition(dngutil::Movement movement);

    Damage getDamageDealt(Creature* defender);

    dngutil::Movement getLastMovement();

    std::string getClassName();
    dngutil::ClassType getClass();
    void setClass(dngutil::ClassType classType);

    void adjustSecondaryDeflectTime(double modifier);

    Primary*& getPrimaryMemory();
    Secondary*& getSecondaryMemory();

    void setLastMoveTime(unsigned long amount);
private:
    dngutil::ClassType classType;

    bool canMiss;

    dngutil::Movement lastMovement;

    unsigned long lastMoveTime;

    // Primary is the weapon that deals damage
    Primary* primary;

    // Secondary is the shield that deflects and/or reduces damage taken
    Secondary* secondary;

    // Stats all creatures have
    unsigned int maxhp;
    int hp;
    unsigned int att;
    unsigned int def;
    unsigned int lck;
    unsigned int spd;
    unsigned int lvl;
};
#endif