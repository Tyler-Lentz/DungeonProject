#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"

#include <map>
#include <list>
#include <array>
#include <mutex>

class MapObject;
class Room;
class Player;
class VirtualWindow;
class MapObject;
class Creature;

struct Difficulty
{
    Difficulty()
    {
        deflectModifier = 1;
        beastSteps = 200;
        damageMultiplier = 1;
        healthIncreaseBoost = 0;
        canFightMegabeast = true;
        color = dngutil::GREEN;
    }

    double deflectModifier;
    int beastSteps;
    double damageMultiplier;
    int healthIncreaseBoost;
    bool canFightMegabeast;
    int color;
};

class Game
{
public:
    dngutil::ReturnVal run();

    Game(VirtualWindow* vwin);
    ~Game();

    Room* getActiveRoom();
    Player* getPlayer();
    VirtualWindow* getVWin();

    void setActiveFloor(unsigned int floor);
    void setActiveRoom(Room*);
    std::map<Coordinate, Room*>& getActiveFloor();

    std::list<MapObject*>& getDeletionList();
    void clearDeletionList();

    Creature* generateCreature(int difficulty, dngutil::TID tid);

    void cleanup(dngutil::ReturnVal returnval);

    dngutil::DungeonType titleScreen();

    bool shouldSpawnBeast();
    void setBeastSpawn(bool spawn);

    void adjustScore(int adjust);
    int getScore();

    int getBeastSteps();

    const Difficulty& getDifficulty();

    std::string getOverworldMusic();
private:
    VirtualWindow* vwin;
    Room* activeRoom;
    Player* player;

    int floor;
    bool exit;
    dngutil::ReturnVal returnVal;

    // Holds all of the rooms.
    std::array<std::map<Coordinate, Room*>, dngutil::NUMFLOORS> gamespace;

    // List of items that need to be deleted.
    std::list<MapObject*> deletionList;

    // overworld music
    std::string overworldMusic;

    int stepsToBeast;
    Difficulty difficulty;

    std::mutex roomMut;
    void makeRooms();
    void makeFloor0();
    void makeFloor1();
    void makeFloor2();
    void makeFloor3();
    void makeFloor4();

    void makeAltRooms();
    void makeAltFloor0();
    void makeAltFloor1();
    void makeAltFloor2();
    void makeAltFloor3();
    void makeAltFloor4();
    void makeAltFloor5();
    void makeAltFloor6();

    // is false after beast has been summoned
    bool spawnBeast;

    int score;
};

#endif