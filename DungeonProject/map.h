#ifndef MAP_H
#define MAP_H

#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <functional>

#include "utilities.h"
#include "colorstring.h"
#include "soundfile.h"

class Room;
class MapObject;

typedef std::vector<std::map<Coordinate, Room*>> GAMESPACE;

class Map
{
public:
    Map(Game* game);
    ~Map();

    // Game pointer
    Game* pgame;

    // Holds all of the rooms.
    GAMESPACE gamespace;

    // List of items that need to be deleted.
    std::list<MapObject*> deletionList;

private:
    void makeOverworld(std::mutex& mut);
    void makeForestTemple(std::mutex& mut);
    void makeHouses(std::mutex& mut);
    void makeSpiritTemple(std::mutex& mut);
    void makeWaterTemple(std::mutex& mut);
    void makeTowerOfTheGods(std::mutex& mut);
    void makeFireTemple(std::mutex& mut);
};

#endif