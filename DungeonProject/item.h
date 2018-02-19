#ifndef ITEM_H
#define ITEM_H

#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "soundfile.h"

#include <string>

class Game;
class Player;

class Item : public MapObject
{
public:
    Item(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool moveable,
        bool rawoutput,
        bool aggressive,
        dngutil::TID typeId,
        bool consumable,
        std::string description
    );

    // The action done when the player uses the item in their inventory
    virtual void action(Player* player, unsigned int inventoryIndex) = 0;

    const bool& isConsumable() const;
    const std::string& getDescription() const;
private:
    // consumable tells it to delete if after using it in the inventory, or
    // something more specific for irregular items
    bool consumable;

    std::string description;
};

//-------------------------------------------------------------
// Regular Items
class RItem : public Item
{
public:
    RItem(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool moveable,
        bool rawoutput,
        bool aggressive,
        dngutil::TID typeId,
        bool consumable,
        std::string description
    );

    // MapAction for regular items, adds it to the player's inventory
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;
};

class Primary : public RItem
{
public:
    Primary(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        double dmgMultiplier,
        int attSpeed,
        int accuracy,
        bool startReady,
        std::string description,
        WavFile hitsound,
        dngutil::ClassType classType,
        dngutil::TID tid = dngutil::TID::Primary,
        bool quickAttack = true
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, tid, false, description)
    {
        this->dmgMultiplier = dmgMultiplier;
        this->attSpeed = attSpeed;
        this->accuracy = accuracy;
        this->startReady = startReady;
        this->hitsound = hitsound;
        this->classType = classType;
        this->canQuickAttack = quickAttack;
    }

    void action(Player* player, unsigned int inventoryIndex) override;

    const double& getDmgMultiplier() const;
    const int& getAttSpeed() const;
    const int& getAccuracy() const;
    const bool& getStartReady() const;
    const bool& getQuickAttack() const;
    const WavFile& getHitsound() const;

    bool hit();

    dngutil::ClassType getClass() const;

    void setAccuracy(int amount);
private:
    // What class it can be equipped by
    dngutil::ClassType classType;

    // The damage multiplier that is given to the attackers attack during battle
    double dmgMultiplier;

    // How many seconds it takes to fully charge a weapon
    int attSpeed;

    // A number from 0 - 100 that is the weapons chance as a percentage to hit
    int accuracy;

    // If this is true, when entering a battle the weapon's charge meter
    // will be fully charged.
    bool startReady;

    // wav file that plays when the weapon is used
    WavFile hitsound;

    bool canQuickAttack;
};

class Secondary : public RItem
{
public:
    Secondary(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        dngutil::TID typeId,
        int deflectTime,
        double dmgReductMult,
        std::string description
    );

    void action(Player* player, unsigned int inventoryIndex) override;

    const double& getDefenseBoost() const;
    const int& getDeflectTime() const;

    void adjustDeflectTime(double modifier);
private:
    // The attackers attack is multiplied by this amount
    double defenseBoost;

    // The amount of time in miliseconds you have to deflect
    int deflectTime;
};

class Potion : public RItem
{
public:
    Potion(Game* pgame, Coordinate coord, int healAmount);

    void action(Player* player, unsigned int inventoryIndex) override;
private:
    int healAmount;
};

class HerosClaim : public RItem
{
public:
    HerosClaim(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class MagicalPotion : public RItem
{
public:
    MagicalPotion(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class Key : public RItem
{
public:
    Key(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class Flute : public RItem
{
public:
    Flute(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;

private:
    std::vector<Coordinate> usedRooms;
    bool isUsed(Coordinate coord);

    const int HEALTH_DECREASE = 30;
};

class BasiliskHorn : public RItem
{
public:
    BasiliskHorn(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class GodStone : public RItem
{
public:
    GodStone(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class SunCharm : public RItem
{
public:
    SunCharm(Game* pgame, Coordinate coord);

    void action(Player* player, unsigned int inventoryIndex) override;
};

class HerosBlade : public Primary
{
public:
    HerosBlade(Game* pgame, Coordinate coord) :
        Primary(
            pgame,
            ColorChar('T', dngutil::YELLOW),
            coord,
            "Hero's Blade",
            false,
            1.7,
            5,
            60,
            true,
            "Blade forged by the gods",
            WavFile("HerosBladeAttack", false, false),
            dngutil::ClassType::KNIGHT,
            dngutil::TID::HerosBlade
        )
    {

    }
};

class HerosBlade2 : public Primary
{
public:
    HerosBlade2(Game* pgame, Coordinate coord) :
        Primary(
            pgame,
            ColorChar('T', dngutil::YELLOW),
            coord,
            "Hero's Blade II",
            false,
            1.7,
            4,
            90,
            true,
            "Blade forged by the gods. It is at full strength.",
            WavFile("HerosBladeAttack", false, false),
            dngutil::ClassType::KNIGHT,
            dngutil::TID::HerosBlade2
        )
    {

    }
};

//-------------------------------------------------------------
#endif