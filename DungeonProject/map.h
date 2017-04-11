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

    // overworld music
    Mp3File overworldMusic;
};

#endif