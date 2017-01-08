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

    // Goes through every item in the other inventory and makes a new copy
    // of it.
    Inventory(const Inventory& other, Game* game)
    {
        for (auto it = other.begin(); it != other.end(); it++)
        {
            push_back((*it)->makeSaveInv(game));
        }
    }

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

    Player(const Player& other, Game* game);

    virtual bool movement();

    virtual MapObject* makeSave(Game* game);
    virtual Collision mapAction(MapObject* collider);

    void addToInventory(Item* item);
    void removeFromInventory(size_t index);
private:
    size_t exp;
    Inventory inventory;
};

#endif