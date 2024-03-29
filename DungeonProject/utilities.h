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
        Water,
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
        ForestDragonPhase1,
        WaterHorse,
        BloodJawPhase1,
        BloodJawPhase2,
        BloodJawPhase3,
        EvilBeastPhase1,
        EvilBeastPhase2,
        EvilBeastPhase3,
        EvilBeastPhase4,
        GhostHorse,
        Tree,
        DungeonCheck,
        Waterboots,
        Speedboots,
        BlueTunic,
        StandardBoots,
        ForestDragonPhase2,
        HarpPiece,
        Floor,
        Npc,
        HeroSpirit,
        HerosTunic,
        MovementObject,
        Robber,
        Bubblecharm,
        HerosBlade,
        HerosBladeStone,
        TrueZorlock,
        DesertGoblin,
        Vender,
        ReinforcedBoots,
        PowerBoots,
        SmashableRock,
        Basilisk,
        BasiliskHorn,
        DesertGryphon,
        SunCharm,
        Imp,
        Cultist,
        GodStone,
        ZorlockImagePhase1,
        ZorlockImagePhase2,
        HerosBlade2,
        Spellbook,
        EldestSage,
        Sage,
        MaskVar1,
        MaskVar2,
        DarkKnight,
        ManaPotion,
        PossessedBowman,
        DarkKnight2,
        StrangeStone,
        SuperPotion,
        SuperManaPotion
    };

    enum class SPELLTID
    {
        SealRevealer,
        DragonBlessing,
        SpiritRoller,
        Meditation,
        DragonShield
    };

    enum class BTID
    {
        Creature,
        Item,
        None
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
        VICTORY
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

    const char noArmorPlayerColor = dngutil::WHITE;

    const int CONSOLEX = 80;
    const int CONSOLEY = 45;

    const int BACKGROUND_COLOR = -1;

    const int STARTING_PLAYER_HP = 50;
    const int STARTING_ENEMY_HP = 20;

    const int STARTING_MANA = 25;
    const int MAX_HP = 9999;
    const int MAX_MANA = 999;
    const unsigned int MAX_ATT = 999;
    const unsigned int MAX_DEF = 989;
    const unsigned int MAX_LCK = 100;
    const unsigned int MAX_SPD = 220;

    const int MIN_HP_GAIN = 18;
    const int MAX_HP_GAIN = 20;
    const int MIN_MANA_GAIN = 8;
    const int MAX_MANA_GAIN = 12;
    const int MIN_ATT_GAIN = 3;
    const int MAX_ATT_GAIN = 4;
    const int MIN_DEF_GAIN = 3;
    const int MAX_DEF_GAIN = 4;
    const int MIN_LCK_GAIN = 2;
    const int MAX_LCK_GAIN = 6;
    const int MIN_SPD_GAIN = 7;
    const int MAX_SPD_GAIN = 8;


    const int TEXT_SCROLL_TIME = 40;

    const int SCROLL_TIME = 30;

    const int POTION_HEAL = 75;
    const int MANA_POTION_HEAL = 50;

    const double MAGIC_POTION_PERCENT = .25;

    const int MENU_DELAY = 100;

    const int TEXT_HIGHLIGHT_COLOR = YELLOW;

    const int BASE_SCORE_INCREASE_BATTLE = 10;
    const int BASE_SCORE_INCREASE_LEVEL = 5;
    const int BASE_SCORE_VICTORY = 250;
    const int BASE_SCORE_BOSS_BOOST = 50;

    const int CLASS_CHOOSING_LEVEL = 6;
    const int PROMOTION_LEVEL = 14;

    const int MOVEMENT_RANDOM_CHANCE = 100;

    const int HOUSE_FLOOR = 14;

    const int DEMON_FLOOR = 7;
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

void credits(dngutil::CreditType c, Game* pgame);

void intro(VirtualWindow* vwin);

void statIncreaseDisplay(int change, int prevValue, int color, std::string text, VirtualWindow* vwin, Coordinate& vcursor);

std::string getClassName(dngutil::ClassType cl);

std::string safeInput(unsigned int maxSize);

void printRandomTitlePic(Game* game);
#endif