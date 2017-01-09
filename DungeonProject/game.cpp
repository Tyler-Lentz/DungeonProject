#include "game.h"
#include "virtualwindow.h"
#include "player.h"
#include "utilities.h"
#include "enemy.h"

#include <string>

//-------------------------------------------------------
// Game Functions

dngutil::ReturnVal Game::run()
{
    // TODO: this entire function
}

Game::Game(const Game& other)
{
    this->vwin = other.vwin;
    player = new Player(*other.player, this);

    Game* lastSave = other.lastSave;

    // TODO: implement this
    std::map<Coordinate, Room*> gamespace[dngutil::NUMFLOORS];
    Room* activeRoom; // initialized after gamespace

    this->exit = other.exit;
    this->returnVal = other.returnVal;
    this->floor = other.floor;
}

Game::~Game()
{
    delete vwin;
}

Room* Game::getActiveRoom()
{
    return activeRoom;
}

Player* Game::getPlayer()
{
    return player;
}

VirtualWindow* Game::getVWin()
{
    return vwin;
}

void Game::makeRooms()
{
    // TODO: add all of the new rooms
}

void Game::setActiveFloor(size_t floor)
{
    this->floor = floor;
}

void Game::setActiveRoom(Room* room)
{
    /* TODO: uncomment this when it can be implemented
    if (activeRoom != nullptr)
    {
        activeRoom->getCreatureList().remove(player);
    }
    activeRoom = room;
    room->getCreatureList().push_back(player);
    */
}

std::map<Coordinate, Room*>& Game::getActiveFloor()
{
    return gamespace[floor];
}

std::list<MapObject*>& Game::getDeletionList()
{
    return deletionList;
}

void Game::clearDeletionList()
{
    for (auto it = deletionList.begin(); it != deletionList.end(); it++)
    {
        delete (*it);
    }
    deletionList.clear();
}

Game*& Game::getLastSave()
{
    return lastSave;
}

Creature* Game::generateCreature(int difficulty, dngutil::TID tid)
{
    int level = random(-1, 1) + difficulty;
    if (level < 1)
    {
        level = 1;
    }

    int health = dngutil::STARTING_ENEMY_HP + random(level - 5, level + 5); // 0
    int attack = 5;
    int defense = 2;
    int luck = 5;
    int speed = 10;
    for (int i = 0; i < level; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            switch (j)
            {
            case 0:
                health += (random(5, 10));
                if (health > dngutil::MAX_HP)
                {
                    health = dngutil::MAX_HP;
                }
                break;
            case 1:
                attack += (random(2, 4));
                if (attack > dngutil::MAX_ATT)
                {
                    attack = dngutil::MAX_ATT;
                }
                break;
            case 2:
                defense += (random(2, 4));
                if (defense > dngutil::MAX_DEF)
                {
                    defense = dngutil::MAX_DEF;
                }
                break;
            case 3:
                luck += (random(5, 15));
                if (luck > dngutil::MAX_LCK)
                {
                    luck = dngutil::MAX_LCK;
                }
                break;
            case 4:
                speed += (random(4, 8));
                if (speed > dngutil::MAX_SPD)
                {
                    speed = dngutil::MAX_SPD;
                }
                break;
            }
        }
    }

    Enemy* enemy = nullptr;
    switch (tid)
    {
        //TODO: put enemies constructors here
    }

    if (enemy == nullptr)
    {
        errorMessage("Enemy is a nullptr. TID is " + std::to_string(static_cast<int>(tid)), __LINE__, __FILE__);
    }

    return enemy;
}
//-------------------------------------------------------