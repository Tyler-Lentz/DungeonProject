#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"

#include <map>
#include <list>
#include <array>

class MapObject;
class Room;
class Player;
class VirtualWindow;
class MapObject;
class Creature;

class Game
{
public:
    dngutil::ReturnVal run();

    Game(VirtualWindow* vwin);

    Game(const Game& other);
    ~Game();

    Room* getActiveRoom();
    Player* getPlayer();
    VirtualWindow* getVWin();

    void makeRooms();

    void setActiveFloor(unsigned int floor);
    void setActiveRoom(Room*);
    std::map<Coordinate, Room*>& getActiveFloor();

    std::list<MapObject*>& getDeletionList();
    void clearDeletionList();

    Game*& getLastSave();

    Creature* generateCreature(int difficulty, dngutil::TID tid);

    void cleanup(dngutil::ReturnVal returnval);

    void titleScreen();
private:
    VirtualWindow* vwin;
    Room* activeRoom;
    Player* player;

    Game* lastSave;

    // Holds all of the rooms.
    std::array<std::map<Coordinate, Room*>, dngutil::NUMFLOORS> gamespace;

    // List of items that need to be deleted.
    std::list<MapObject*> deletionList;

    int floor;
    bool exit;
    dngutil::ReturnVal returnVal;
};

#endif