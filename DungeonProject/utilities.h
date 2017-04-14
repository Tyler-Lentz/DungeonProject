#ifndef UTILITIES_H
#define UTILITIES_H

#include "coordinate.h"

#include <string>
#include <list>

class VirtualWindow;
class MapObject;
class Game;

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
        SegbossTrigger,
        MegaBeastPhase1,
        MegaBeastPhase2,
        Flute,
        HerosClaim,
        Bowman,
        OutsideObject,
        FlameHorse,
        GryphonPhase1,
        GryphonPhase2,
        GryphonPhase3,
        ReaperPhase1,
        ReaperPhase2,
        ReaperPhase3,
        MaskPhase1,
        MaskPhase2,
        MaskPhase3,
        ForestDragon,
        WaterHorse,
        BloodJawPhase1,
        BloodJawPhase2,
        BloodJawPhase3,
        EvilBeastPhase1,
        EvilBeastPhase2,
        EvilBeastPhase3,
        EvilBeastPhase4,
        GhostHorse
    };

    enum class BTID
    {
        Creature,
        Item,
        None
    };

    enum class DungeonType
    {
        DRAGONS_LAIR,
        GRYPHONS_TOWER,
        FIFTYTRIALS,
        UNDERWATER_DUNGEON,
        NEITHER
    };

    enum class ClassType
    {
        ADVENTURER,
        WIZARD,
        RANGER,
        KNIGHT
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

    enum class CreditType
    {
        TITLESCREEN,
        VICTORY,
        SECRET_VICTORY
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

    const int BACKGROUND_COLOR = -1;

    const int STARTING_PLAYER_HP = 50;
    const int STARTING_ENEMY_HP = 20;

    const int MAX_HP = 200;
    const unsigned int MAX_ATT = 75;
    const unsigned int MAX_DEF = 60;
    const unsigned int MAX_LCK = 100;
    const unsigned int MAX_SPD = 220;

    const int SCROLL_TIME = 60;

    const int POTION_HEAL = 50;

    const int MENU_DELAY = 100;

    const int BASE_SCORE_INCREASE_BATTLE = 10;
    const int BASE_SCORE_INCREASE_LEVEL = 5;
    const int BASE_SCORE_VICTORY = 250;
    const int BASE_SCORE_BOSS_BOOST = 50;

    const int CLASS_CHOOSING_LEVEL = 6;
    const int PROMOTION_LEVEL = 14;

    const int MOVEMENT_RANDOM_CHANCE = 100;
}

void errorMessage(std::string error, int line, std::string file);

int getColor(int letterColor, int backgroundColor, int numberOfColors = 16);

bool keypress(int key);

bool keyrelease(int key);

int random(int max);

int random(int min, int max);

void sortPriority(std::list<MapObject*>& list, MapObject* objectToAdd);

void pressEnter(Coordinate cursorpos, VirtualWindow* vwin, int color = dngutil::CYAN);

int getExpToLevel(unsigned int level);

int getHealthbarSleepTime(int damageDealt);

void credits(dngutil::CreditType c, Game* pgame, dngutil::DungeonType dungeon);

void intro(VirtualWindow* vwin);

void statIncreaseDisplay(int change, int prevValue, int color, std::string text, VirtualWindow* vwin, Coordinate& vcursor);
#endif