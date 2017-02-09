#ifndef UTILITIES_H
#define UTILITIES_H

#include "coordinate.h"

#include <string>
#include <list>

class VirtualWindow;
class MapObject;

namespace dngutil
{
    enum colors
    {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE
    };

    enum class TID
    {
        Player,
        Potion,
        Primary,
        Secondary,
        Empty,
        Exit,
        Wall,
        Skeleton,
        Key,
        LockedDoor,
        Hole,
        SkeletonKing,
        BloodSkeleton,
        LSKnight,
        SSKnight,
        Mage,
        MagicalPotion,
        Altar,
        DungeonBeast,
        BeastTrigger,
        DragonTail,
        DragonWings,
        DragonHead,
        SegbossTrigger
    };

    enum class BTID
    {
        Creature,
        Item,
        None
    };

    enum class Movement
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class ReturnVal
    {
        EXIT,
        RESTART
    };

    enum class MovementTypes
    {
        VALID,
        INVALID,
        NEW_ROOM,
        RETURN_TRUE
    };

    enum class EvType
    {
        HEALTH,
        ATTACK,
        DEFENSE,
        LUCK,
        SPEED,
        NONE
    };

    enum Priority
    {
        P_EMPTY,
        P_ITEM,
        P_STAIRCASE,
        P_ENEMY,
        P_PLAYER,
        P_WALL
    };

    const int CONSOLEX = 80;
    const int CONSOLEY = 45;

    // TODO: change this to use gamespaces size
    const int NUMFLOORS = 4;

    const int MAPSIZE = 100;

    const int BACKGROUND_COLOR = -1;

    const int STARTING_PLAYER_HP = 50;
    const int STARTING_ENEMY_HP = 20;

    const int MAX_HP = 255;
    const unsigned int MAX_ATT = 75;
    const unsigned int MAX_DEF = 60;
    const unsigned int MAX_LCK = 100;
    const unsigned int MAX_SPD = 220;

    const int SCROLL_TIME = 60;

    const int POTION_HEAL = 50;

    const int MENU_DELAY = 100;

    const int FULL_STEPS = 200;
}

void errorMessage(std::string error, int line, std::string file);

int getColor(int letterColor, int backgroundColor, int numberOfColors = 16);

bool keypress(int key);

bool keyrelease(int key);

int random(int max);

int random(int min, int max);

void sortPriority(std::list<MapObject*>& list, MapObject* objectToAdd);

void pressEnter(Coordinate cursorpos, VirtualWindow* vwin);

int getExpToLevel(unsigned int level);

void startMp3(std::string fileName);

void stopMp3();

void soundEffect(std::string fileName, bool loop, bool async);

void stopSound();

#endif