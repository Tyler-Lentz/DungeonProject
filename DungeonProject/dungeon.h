#ifndef DUNGEON_H
#define DUNGEON_H

#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <functional>

#include "utilities.h"
#include "colorstring.h"

class Room;
class MapObject;

typedef std::vector<std::map<Coordinate, Room*>> GAMESPACE;

class Dungeon
{
public:
    Dungeon(Game* game, double beastMultiplier);
    ~Dungeon();

    // the beast steps get multiplied by this amount
    double beastMultiplier;

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
    
    // beast sequence for that dungeon
    std::function<void(Game* game)> beastSequence;
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

class UnderwaterTemple : public Dungeon
{
public:
    UnderwaterTemple(Game* game);

private:

    void makeRooms();
    void makeFloor5(std::mutex& roomMut);
    void makeFloor4(std::mutex& roomMut);
    void makeFloor3(std::mutex& roomMut);
    void makeFloor2(std::mutex& roomMut);
    void makeFloor1(std::mutex& roomMut);
    void makeFloor0(std::mutex& roomMut);
};

class PitOf50Trials : public Dungeon
{
public:
    PitOf50Trials(Game* game);

private:
    void generateDungeon();
};

#endif