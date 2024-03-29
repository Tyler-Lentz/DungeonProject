#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "mapobject.h"
#include "utilities.h"
#include "coordinate.h"
#include "item.h"
#include "spell.h"
#include <vector>
#include <list>

class Equipment;
class Game;

struct Inventory : public std::vector<Item*>
{
    Inventory() {}

    // Deletes all items in the inventory
    ~Inventory()
    {
        for (auto it = begin(); it != end(); it++)
        {
            delete *it;
        }
    }
};

class Player : public Creature
{
public:
    Player(
        Game* pgame,
        Coordinate coord,
        std::string name
    );

    bool movement() override;
    void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) override;
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

    void minusStep();

    void levelUpStats() override;

    void addToInventory(Item* item);
    void removeFromInventory(unsigned int index);

    ColorString getExperienceBar();

    void addExperience(unsigned int experience, dngutil::EvType ev);
    bool takeExperience(double percentage);

    void inventoryMenu();
    void statsMenu();
    void mapMenu();

    const Inventory& getInventory() const;
    Inventory& getInventoryNotConst();

    bool swapPrimary(Item*& itemToSwap);
    bool swapSecondary(Item*& itemToSwap);
    bool swapArmor(Item*& itemToSwap);
    bool swapBoots(Item*& itemToSwap);

    ColorString getStepString();
    void resetSteps();
    int getStepCount();

    int getExp() const;
    int getPlayersExpToLevel() const;

    void setHarp(int number, bool value);
    bool hasFullHarp() const;
    bool hasHarpPiece(int number) const;

    void setExp(int value);
    void setExpToLevel(int value);
    void gotoDungeonStart(int harpPiece);
    void setDungeonStart(Coordinate room, Coordinate map);
    void setDungeonStart();
    Coordinate getStartingDungeonMapCoord() const;
    Coordinate getStartingDungeonRoomCoord() const;

    Equipment*& getBootsMemory();
    Equipment*& getArmorMemory();

    void setArmor(Equipment* equip);
    void setBoots(Equipment* equip);

    bool hasItem(dngutil::TID tid, bool onlyCountHolding = false);

    int getGold() const;
    void changeGold(int amount);
    void setGold(int amount); 

    void setMaxSpeedMultiplier(double amount);

    const double startingMaxSpeedMultiplier;

    void upgradePrimary(dngutil::TID itemToUpgrade);

    void setCertainCrit(bool value);
    bool getCertainCrit();

    int getNumberOfHarpPieces();

    void addSpell(Spell* spell);
    bool hasSpell(dngutil::SPELLTID sp);
private:

    Coordinate startingDungeonMapCoord;
    Coordinate startingDungeonRoomCoord;

    double maxSpeedMultiplier;

    double hpEv;
    double attEv;
    double defEv;
    double lckEv;
    double spdEv;

    bool harp1;
    bool harp2;
    bool harp3;
    bool harp4;

    unsigned int exp;
    unsigned int expToLevel;
    Inventory inventory;

    int steps;

    std::string name;

    Equipment* armor;
    Equipment* boots;

    int gold;

    bool certainCrit = false;
};

#endif