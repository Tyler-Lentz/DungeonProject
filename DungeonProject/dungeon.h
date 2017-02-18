#ifndef DUNGEON_H
#define DUNGEON_H

#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>

#include "utilities.h"
#include "colorstring.h"

class Room;
class MapObject;

typedef std::array<std::map<Coordinate, Room*>, dngutil::NUMFLOORS> GAMESPACE;

class Dungeon
{
public:
    Dungeon(Game* game);
    ~Dungeon();

    // Game pointer
    Game* pgame;

    // Holds all of the rooms.
    GAMESPACE gamespace;

    // List of items that need to be deleted.
    std::list<MapObject*> deletionList;

    // overworld music
    std::string overworldMusic;

    // story, int is number of lines after normal newline
    std::vector<std::pair<ColorString, int>> story;
};

class DragonsLair :  public Dungeon
{
public:
    DragonsLair(Game* game);

private:

    void makeRooms();
    void makeFloor0(std::mutex& roomMut);
    void makeFloor1(std::mutex& roomMut);
    void makeFloor2(std::mutex& roomMut);
    void makeFloor3(std::mutex& roomMut);
    void makeFloor4(std::mutex& roomMut);
};

class GryphonsTower : public Dungeon
{
public:
    GryphonsTower(Game* game);

private:

    void makeRooms();
    void makeFloor0(std::mutex& roomMut);
    void makeFloor1(std::mutex& roomMut);
    void makeFloor2(std::mutex& roomMut);
    void makeFloor3(std::mutex& roomMut);
    void makeFloor4(std::mutex& roomMut);
    void makeFloor5(std::mutex& roomMut);
    void makeFloor6(std::mutex& roomMut);
};

#endif