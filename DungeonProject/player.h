#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "mapobject.h"
#include "utilities.h"
#include "coordinate.h"
#include "item.h"

#include <vector>

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

    void inventoryMenu();
    void statsMenu();

    const Inventory& getInventory() const;
    Inventory& getInventoryNotConst();

    bool swapPrimary(Item*& itemToSwap);
    bool swapSecondary(Item*& itemToSwap);

    ColorString getStepString();
    void resetSteps();
    int getStepCount();

    void chooseClass();

    int getExp() const;
    int getPlayersExpToLevel() const;

    void setHarp(int number, bool value);
    bool hasFullHarp() const;
    bool hasHarpPiece(int number) const;

    void setExp(int value);
    void setExpToLevel(int value);
    void gotoDungeonStart();
private:

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
};

#endif