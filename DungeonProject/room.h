#ifndef ROOM_H
#define ROOM_H

#include "coordinate.h"
#include "utilities.h"
#include "soundfile.h"

#include <list>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <array>

class MapObject;
class Game;
class Creature;
class Item;

struct RoomInfo
{
   RoomInfo(
        std::vector<std::string> roomTemplate,
        std::map<Coordinate, MapObject*> specificObjects,
        std::string name,
        int difficulty,
        int backColor,
        std::vector<dngutil::TID> possibleCreatures,
        int floor,
        Coordinate mapCoord
    )
    {
        this->roomTemplate = roomTemplate;
        this->specificObjects = specificObjects;
        this->name = name;
        this->difficulty = difficulty;
        this->backColor = backColor;
        this->possibleCreatures = possibleCreatures;
        this->floor = floor;
        this->mapCoord = mapCoord;
    }

    RoomInfo(const RoomInfo& other)
    {
        for (auto it = other.specificObjects.begin(); it != other.specificObjects.end(); it++)
        {
            this->specificObjects.emplace(it->first, it->second);
        }

        this->roomTemplate = other.roomTemplate;
        this->name = other.name;
        this->difficulty = other.difficulty;
        this->backColor = other.backColor;
        this->possibleCreatures = other.possibleCreatures;
        this->floor = other.floor;
        this->mapCoord = other.mapCoord;
    }

    // each index in the vector holds a string of the starting map
    std::vector<std::string> roomTemplate;

    // specific objects at a coordinate can be placed in this map so they
    // are constructed exactly as wanted
    std::map<Coordinate, MapObject*> specificObjects;

    // when a undefined creature 'e' is found on the map, a random enemy
    // type is picked from here
    std::vector<dngutil::TID> possibleCreatures;

    // General information
    std::string name;
    int difficulty;
    int backColor;
    int floor;
    Coordinate mapCoord;
};

typedef std::vector<std::vector<std::list<MapObject*>>> GAMEMAP;

struct Puzzle
{
    Puzzle(
        std::function<bool(const std::list<Creature*>&, const GAMEMAP&)> solved,
        std::function<void(std::list<Creature*>, GAMEMAP&)> action,
        std::string hint
    )
    {
        isSolved = solved;
        puzzleAction = action;
        this->hint = hint;
    }

    std::function<bool(const std::list<Creature*>&, const GAMEMAP&)> isSolved;
    std::function<void(std::list<Creature*>, GAMEMAP&)> puzzleAction;

    std::string hint;
};


class Room
{
public:
    // if there is no puzzle pass through a nullptr for the puzzle
    Room(Game* game_pointer, RoomInfo mapToGenerate, Puzzle* puzzle, Mp3File music, bool displayOnMap = true);
    ~Room();

    int getRoomX() const;
    int getRoomY() const;

    // Returns if the position of a creature is valid, invalid or in a new room
    dngutil::MovementTypes checkMovement(Coordinate coord, Creature* creature) const;
    
    // Draws all of the changed positions in the room
    void drawRoom();

    // Adds a coordinate that has been changed so it knows to redraw that coord
    void addCoordToList(Coordinate coord);

    // Sets every position as changed
    void setAll();

    // Returns the list of objects at a specific coordinate
    std::list<MapObject*>& Room::getObjects(Coordinate coord);

    // Erases an item at a specific coordinate
    bool eraseItem(Coordinate coord);

    // Returns the list of creatures in the room
    std::list<Creature*>& getCreatureList();

    // Adds a creature to the list of creatures in the room
    void addCreature(Creature* type, Coordinate coord);

    // Reference to the room info - the blueprint of the room
    const RoomInfo& getRoomInfo() const;

    // Returns a const ref to the list of positions that have been adjusted
    const std::list<Coordinate>& getAdjustedPositions() const;

    // checks if the puzzle is solved and does the action
    void checkPuzzle();

    // Returns true if there is a puzzle set
    bool hasPuzzle();

    std::string getPuzzleHint();

    Mp3File getMusic() const;

    // Sets background color of the map
    void setBackgroundColor(int color);

    bool shouldDisplayOnMap() const;
private:
    GAMEMAP gameMap;

    Mp3File music;

    std::list<Coordinate> adjustedPositions;
    std::map<Coordinate, MapObject*> objects;

    int roomX;
    int roomY;
    bool displayOnMap;

    Game* game_pointer;
    std::list<Creature*> creatureList;

    RoomInfo roomInfo;

    Puzzle* puzzle;
};
#endif