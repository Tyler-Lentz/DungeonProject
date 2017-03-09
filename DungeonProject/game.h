#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"
#include "dungeon.h"
#include "soundfile.h"

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

    void titleScreen();

    bool shouldSpawnBeast();
    void setBeastSpawn(bool spawn);

    void adjustScore(int adjust);
    int getScore();

    int getBeastSteps();

    const Difficulty& getDifficulty();

    Mp3File getOverworldMusic() const;

    void setExitToFalse();
    bool shouldExit();

    void beastSequence();
private:
    VirtualWindow* vwin;
    Room* activeRoom;
    Player* player;

    int floor;
    bool exit;
    dngutil::ReturnVal returnVal;

    Dungeon* dungeon;

    int stepsToBeast;
    Difficulty difficulty;

    bool spawnBeast;

    int score;
};

#endif