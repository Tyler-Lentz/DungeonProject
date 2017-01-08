#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"

#include <map>
#include <list>

class MapObject;
class Room;
class Player;
class VirtualWindow;
class MapObject;

class Game
{
public:
    dngutil::ReturnVal run();

    Game(VirtualWindow* vwin)
    {
        this->vwin = vwin;
        activeRoom = nullptr;
        player = nullptr;
        lastSave = nullptr;
        floor = 0;
    }

    Game(const Game& other);
    ~Game();

    Room* getActiveRoom();
    Player* getPlayer();
    VirtualWindow* getVWin();

    void makeRooms();

    void setActiveFloor(size_t floor);
    void setActiveRoom(Room*);
    std::map<Coordinate, Room*>& getActiveFloor();

    std::list<MapObject*>& getDeletionList();
    void clearDeletionList();

    Game*& getLastSave();
private:
    VirtualWindow* vwin;
    Room* activeRoom;
    Player* player;

    Game* lastSave;

    // Holds all of the rooms.
    std::map<Coordinate, Room*> gamespace[dngutil::NUMFLOORS];

    // List of items that need to be deleted.
    std::list<MapObject*> deletionList;

    int floor;
    bool exit;
    dngutil::ReturnVal returnVal;
};

#endif