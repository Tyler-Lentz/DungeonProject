#include "game.h"
#include "virtualwindow.h"
#include "player.h"

//-------------------------------------------------------
// Game Functions

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
//-------------------------------------------------------