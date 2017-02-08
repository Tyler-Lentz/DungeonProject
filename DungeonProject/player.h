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
        Coordinate coord
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
    void dungeonBeastSequence();
private:

    double hpEv;
    double attEv;
    double defEv;
    double lckEv;
    double spdEv;

    unsigned int exp;
    unsigned int expToLevel;
    Inventory inventory;

    int steps;
};

#endif