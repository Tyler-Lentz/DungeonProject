#include "enemy.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include "item.h"
#include "game.h"
#include "mapobject.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "room.h"
#include "soundfile.h"

#include <string>

class Game;

//----------------------------------------------------------------
// Enemy Functions
Enemy::Enemy(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool rawoutput,
    dngutil::TID typeId,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl,
    Primary* primary,
    Secondary* secondary,
    Mp3File battleMusic,
    unsigned int experienceGiven,
    WavFile deathSound,
    dngutil::EvType ev,
    dngutil::ClassType classType
) : Creature(
    pgame,
    mapRep,
    coord,
    name,
    true,
    rawoutput,
    true,
    typeId,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    primary,
    secondary,
    dngutil::P_ENEMY,
    classType
)
{
    this->battleMusic = battleMusic;
    this->deathSound = deathSound;
    this->experienceGiven = experienceGiven;
    this->ev = ev;
}

const Mp3File& Enemy::getBattleMusic() const
{
    return battleMusic;
}

const WavFile& Enemy::getDeathSound() const
{
    return deathSound;
}

const unsigned int& Enemy::getExpGiven() const
{
    return experienceGiven;
}

void Enemy::dodgeAnimation()
{
    VirtualWindow* vwin = getPGame()->getVWin();
    int topLine = vwin->txtmacs.DIVIDER_LINES[1] + 1;
    int bottomLine = vwin->txtmacs.DIVIDER_LINES[2] - 1;
    VWIN battleScene;

    for (int i = 0; i < ((bottomLine - topLine) - 1); i++)
    {
        battleScene.push_back(vwin->getLine(i + topLine));
    }

    for (int x = 0; x < (dngutil::CONSOLEX / 3); x++)
    {
        for (int y = topLine; y < (battleScene.size()); y++)
        {
            battleScene[y].popFront();
            vwin->put(battleScene[y - topLine], Coordinate(0, y));
        }
    }
}

void Enemy::printStats(int LONGEST_LINE_LENGTH, int startingCursorY)
{
    int STATS_LENGTH = 13;

    VirtualWindow* vwin = getPGame()->getVWin();

    Coordinate vcursor(LONGEST_LINE_LENGTH + 1, startingCursorY);

    vwin->put(ColorString("*************", dngutil::MAGENTA), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Level: " + std::to_string(getLvl()), dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Health: " + std::to_string(getHp()) + "    ", dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Attack: " + std::to_string(getAtt()) + "    ", dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Defense: " + std::to_string(getDef()) + "    ", dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Luck: " + std::to_string(getLck()) + "    ", dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("Speed: " + std::to_string(getSpd()) + "    ", dngutil::GREEN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString(getPrimary().getName(), dngutil::LIGHTCYAN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString(getSecondary().getName(), dngutil::LIGHTCYAN), vcursor);

    vcursor.x = LONGEST_LINE_LENGTH + 1; vcursor.y++;
    vwin->put(ColorString("*************", dngutil::MAGENTA), vcursor);
}

void Enemy::deathSequence()
{
    Player* player = getPGame()->getPlayer();

    int levelDif = getLvl() - getPGame()->getPlayer()->getLvl();
    getPGame()->adjustScore(dngutil::BASE_SCORE_INCREASE_BATTLE + levelDif);

    int realExpGiven = experienceGiven + (6 * (getLvl() - (player->getLvl() ) ) );
    if (realExpGiven < 5)
    {
        realExpGiven = 5;
    }

    player->addExperience(realExpGiven, ev);

    std::string dropType = drop();
    if (dropType != "NULL")
    {
        getPGame()->getVWin()->txtmacs.clearDivider("bottom");

        playSound(WavFile("FindItem", false, false));

        getPGame()->getVWin()->putcen(
            ColorString("The " + getName() + " drops a " + dropType, dngutil::CYAN),
            getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE, true);
        pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin()); 
    }

    getPGame()->getVWin()->txtmacs.clearLine(getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
    getPGame()->getVWin()->txtmacs.clearLine(getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);

    int goldAmount = random(0, 1);
    if (goldAmount != 0)
    {
        goldAmount += random(0, 9);
    }
    if (goldAmount > 0)
    {
        getPGame()->getPlayer()->changeGold(goldAmount);
        getPGame()->getVWin()->putcen(
            ColorString("The " + getName() + " drops " + std::to_string(goldAmount) + " gold", dngutil::CYAN),
            getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE, true);
        pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin());
    }
}

Collision Enemy::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer() && isAggressive())
    {
        stopSound(SoundType::MP3);

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        if (getTypeId() != dngutil::TID::TrueZorlock)
        {
            playSound(WavFile("EnterBattle", false, true));
        }
        if (battle(this))
        {
            getPGame()->getVWin()->txtmacs.displayOverworldInfo(getPGame());
            it++;
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
            removeFromMap(true);
            return Collision(false, false);
        }
        else
        {
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(collider);
            collider->removeFromMap(false);
            return Collision(true, true, true);
        }

    }
    return Collision(false, true);
}

ColorString Enemy::getBattleInfo() const
{
    return ColorString("You have entered a battle against a " + getName() + "!", dngutil::WHITE);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// REnemy Functions
std::string REnemy::drop()
{
    Item* itemToAdd;
    if (random(10) == 0)
    {
        if (random(1) == 0)
        {
            itemToAdd = getPrimaryMemory();
setPrimary(nullptr);
        }
        else
        {
            itemToAdd = getSecondaryMemory();
            setSecondary(nullptr);
        }

        getPGame()->getPlayer()->addToInventory(itemToAdd);
        return itemToAdd->getName();
    }
    else if (random(20) == 0)
    {
        itemToAdd = new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL);
        getPGame()->getPlayer()->addToInventory(itemToAdd);
        return itemToAdd->getName();
    }

    return "NULL";
}
bool REnemy::movement()
{
    // smart enemies movement may change with this one
    // same as npc
    if (!(getLastMoveTime() + ((dngutil::MAX_SPD * 3) - getSpd()) > GetTickCount()))
    {
        switch (random(dngutil::MOVEMENT_RANDOM_CHANCE))
        {
        case 0:
            return adjustPosition(dngutil::Movement::UP);
            break;
        case 1:
            return adjustPosition(dngutil::Movement::DOWN);
            break;
        case 2:
            return adjustPosition(dngutil::Movement::LEFT);
            break;
        case 3:
            return adjustPosition(dngutil::Movement::RIGHT);
            break;
        }
    }
    return false;
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// SmartEnemy functions

bool SmartEnemy::movement()
{
    // regular enemy's movement may change with this one
    if (!(getLastMoveTime() + ((dngutil::MAX_SPD * 3) - getSpd()) > GetTickCount()))
    {
        Player* p = getPGame()->getPlayer();

        int xdiff = this->getCoord().x - p->getCoord().x;
        int ydiff = this->getCoord().y - p->getCoord().y;

        if (abs(xdiff) < 3 && abs(ydiff) < 3)
        {
            if (random(dngutil::MOVEMENT_RANDOM_CHANCE))
            {
                std::vector<dngutil::Movement> movements;

                if (xdiff < 0)
                {
                    movements.push_back(dngutil::Movement::RIGHT);
                }
                else if (xdiff > 0)
                {
                    movements.push_back(dngutil::Movement::LEFT);
                }

                if (ydiff > 0)
                {
                    movements.push_back(dngutil::Movement::UP);
                }
                else if (ydiff < 0)
                {
                    movements.push_back(dngutil::Movement::DOWN);
                }

                if (!movements.empty())
                {
                    dngutil::Movement move = movements[random(movements.size() - 1)];
                    return adjustPosition(move);
                }
            }
        }
        else 
        {
            return REnemy::movement();
        }
    }

    return false;
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// BEnemy functions
std::string BEnemy::drop()
{
    Item* itemToAdd;
    int numberOfPotions = random(2);
    if (numberOfPotions > 0)
    {
        for (int i = 0; i < numberOfPotions; i++)
        {
            itemToAdd = new Potion(getPGame(), Coordinate(-1, -1), static_cast<int>(dngutil::POTION_HEAL * 1.2));
            getPGame()->getPlayer()->addToInventory(itemToAdd);
        }
        return "Potion (" + std::to_string(numberOfPotions) + ")";
    }


    return "NULL";
}
bool BEnemy::movement()
{
    return false;
}

void BEnemy::deathSequence()
{
    getPGame()->adjustScore(dngutil::BASE_SCORE_BOSS_BOOST);

    playSound(Mp3File("DefeatBoss"));

    getPGame()->getVWin()->txtmacs.clearMapArea(true, 100);
    getPGame()->getVWin()->txtmacs.clearDivider("bottom");

    // Length of the sound effect adjusted for the screen clearing
    Sleep(5000);

    stopSound(SoundType::MP3);

    Enemy::deathSequence();
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Skeleton Functions
Skeleton::Skeleton(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : REnemy(
    pgame,
    ColorChar('T', dngutil::WHITE),
    coord,
    "Skeleton",
    false,
    dngutil::TID::Skeleton,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('\\', dngutil::BROWN),
        coord,
        "Wooden Axe",
        false,
        1.5,
        5,
        70,
        false,
        "A heavy wooden axe that was dropped from a Skeleton",
        WavFile("Attack4", false, false),
        dngutil::ClassType::ADVENTURER
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::BROWN),
        coord,
        "Wooden Shield",
        false,
        dngutil::TID::Secondary,
        100,
        1.1,
        "A heavy wooden shield that was dropped from a Skeleton"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(23, 29),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::HEALTH,
    dngutil::ClassType::ADVENTURER
)
{

}


void Skeleton::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int skeletoncolor = dngutil::WHITE;
    int weaponcolor = dngutil::BROWN;
    t->put(ColorString("                              _.--\"\"-._                     \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  .", weaponcolor) + ColorString("                        .\"         \".                   \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" / \\    ,^.", weaponcolor) + ColorString("         /(     Y             |      )\\          \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("/   `---. |--\'\\", weaponcolor) + ColorString("    (  \\__..\'--   -   -- -\'\"\"-.-\'  )          \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("|        :|    `> ", weaponcolor) + ColorString("  \'.     l_..-------.._l      .\'          \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("|      __l;__ .\'", weaponcolor) + ColorString("      \"-.__.||_.-\'v\'-._||`\"----\"            \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" \\  .-\' | |  `", weaponcolor) + ColorString("              l._       _.\'                   \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  \\/    | |   ", weaponcolor) + ColorString("                l`^^\'^^\'j                     \n", skeletoncolor), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString("        | |  ", weaponcolor) + ColorString("              _   \\_____/     _                \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("        j |  ", weaponcolor) + ColorString("             l `--__)-\'(__.--\' |               \n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |    ", weaponcolor) + ColorString("           | /`---``-----\'\"1 |", skeletoncolor) + ColorString("  ,-----.      \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |    ", weaponcolor) + ColorString("           )/  `--\' \'---\' ", skeletoncolor) + ColorString("   /'-\'  ___  `-.   \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |     ", weaponcolor) + ColorString("         //  `-\'  \'`----\'", skeletoncolor) + ColorString("  /  ,-\'   I`.  \\  \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      _ L |_     ", weaponcolor) + ColorString("       //  `-.-.\'`-----\' ", skeletoncolor) + ColorString("/  /  |   |  `. \\ \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("     \'._\' / \\         _/(   '-  -'- ---\' ", skeletoncolor) + ColorString(";  /__.J   L.__.\\ :\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      `._;/7(-.......\'  /        ) (  ", skeletoncolor) + ColorString("   |  |            | |\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      `._;l _\'--------_/        )-\'/  ", skeletoncolor) + ColorString("   :  |___.    _._./ ;\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |        ", weaponcolor) + ColorString("         .__ )-\'\\  __ ", skeletoncolor) + ColorString(" \\  \\  I   1   / / \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        `-\'    ", weaponcolor) + ColorString("            /   `-\\-(-\'   \\", skeletoncolor) + ColorString(" \\  `.|   | ,\' /  \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("                           \\__  `-\'    __/ ", skeletoncolor) + ColorString("  `-. `---\'\',-\'   \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("                              )-._.-- (  ", skeletoncolor) + ColorString("      `-----\'      \n", weaponcolor), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 62;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);  
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// BloodSkeleton Functions
BloodSkeleton::BloodSkeleton(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('T', dngutil::LIGHTRED),
    coord,
    "Blood Skeleton",
    false,
    dngutil::TID::BloodSkeleton,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('/', dngutil::WHITE),
        coord,
        "Stained Axe",
        false,
        1.6,
        5,
        70,
        false,
        "An axe from a blood skeleton",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::WHITE),
        coord,
        "Stained Shield",
        false,
        dngutil::TID::Secondary,
        175,
        1,
        "A shield from a blood skeleton"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(31, 60),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::HEALTH,
    dngutil::ClassType::KNIGHT
)
{

}

void BloodSkeleton::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int BloodSkeletoncolor = dngutil::RED;
    int weaponcolor = dngutil::WHITE;
    t->put(ColorString("                              _.--\"\"-._                     \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  .", weaponcolor) + ColorString("                        .\"         \".                   \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" / \\    ,^.", weaponcolor) + ColorString("         /(     Y             |      )\\          \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("/   `---. |--\'\\", weaponcolor) + ColorString("    (  \\__..\'--   -   -- -\'\"\"-.-\'  )          \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("|        :|    `> ", weaponcolor) + ColorString("  \'.     l_..-------.._l      .\'          \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("|      __l;__ .\'", weaponcolor) + ColorString("      \"-.__.||_.-\'v\'-._||`\"----\"            \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" \\  .-\' | |  `", weaponcolor) + ColorString("              l._       _.\'                   \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  \\/    | |   ", weaponcolor) + ColorString("                l`^^\'^^\'j                     \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString("        | |  ", weaponcolor) + ColorString("              _   \\_____/     _                \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("        j |  ", weaponcolor) + ColorString("             l `--__)-\'(__.--\' |               \n", BloodSkeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |    ", weaponcolor) + ColorString("           | /`---``-----\'\"1 |", BloodSkeletoncolor) + ColorString("  ,-----.      \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |    ", weaponcolor) + ColorString("           )/  `--\' \'---\' ", BloodSkeletoncolor) + ColorString("   /'-\'  ___  `-.   \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |     ", weaponcolor) + ColorString("         //  `-\'  \'`----\'", BloodSkeletoncolor) + ColorString("  /  ,-\'   I`.  \\  \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      _ L |_     ", weaponcolor) + ColorString("       //  `-.-.\'`-----\' ", BloodSkeletoncolor) + ColorString("/  /  |   |  `. \\ \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("     \'._\' / \\         _/(   '-  -'- ---\' ", BloodSkeletoncolor) + ColorString(";  /__.J   L.__.\\ :\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      `._;/7(-.......\'  /        ) (  ", BloodSkeletoncolor) + ColorString("   |  |            | |\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("      `._;l _\'--------_/        )-\'/  ", BloodSkeletoncolor) + ColorString("   :  |___.    _._./ ;\n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        | |        ", weaponcolor) + ColorString("         .__ )-\'\\  __ ", BloodSkeletoncolor) + ColorString(" \\  \\  I   1   / / \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("        `-\'    ", weaponcolor) + ColorString("            /   `-\\-(-\'   \\", BloodSkeletoncolor) + ColorString(" \\  `.|   | ,\' /  \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("                           \\__  `-\'    __/ ", BloodSkeletoncolor) + ColorString("  `-. `---\'\',-\'   \n", weaponcolor), vcursor); vcursor.y++;
    t->put(ColorString("                              )-._.-- (  ", BloodSkeletoncolor) + ColorString("      `-----\'      \n", weaponcolor), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 62;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// Skeleton King Functions

SkeletonKing::SkeletonKing(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('T', dngutil::BLACK),
    coord,
    "Skeleton King",
    false,
    dngutil::TID::SkeletonKing,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('|', dngutil::DARKGRAY),
        coord,
        "Long Sword",
        false,
        1.5,
        4,
        95,
        false,
        "A very long sword. It is very accurate.",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('A', dngutil::GREEN),
        coord,
        "Mutilated Head",
        false,
        dngutil::TID::Secondary,
        0,
        .8,
        "A corpses head, cursed by the skeleton king."
    ),
    Mp3File("MinibossTheme"),
    50,
    WavFile("Screech", false, false),
    dngutil::EvType::ATTACK,
    dngutil::ClassType::KNIGHT
) {}

ColorString SkeletonKing::getBattleInfo() const
{
    return ColorString("The king of Skeletons towers over you", dngutil::WHITE);
}

void SkeletonKing::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int headcolor = dngutil::GREEN;
    int skeletoncolor = dngutil::DARKGRAY;
    t->put(ColorString("  _,_\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  /7/Y/^\\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  vuVV|C)|            ", skeletoncolor) + ColorString("            __ _\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("    \\|^ /          ", headcolor) + ColorString("             .'  Y '>,\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("    )| \\)       ", headcolor) + ColorString("               / _   _   \\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("   //)|\\\\     ", headcolor) + ColorString("                 )(_) (_)(|}\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  / ^| \\ \\    ", headcolor) + ColorString("                 {  4A   } /\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" //^| || \\\\   ", headcolor) + ColorString("                  \\uLuJJ/\\l\n", skeletoncolor), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(">//||| |\\\\\\|   ", headcolor) + ColorString("                 |3    p)/\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("| \"\"\"\"\"  7/>l ", headcolor) + ColorString("__ _____ ____      /nnm_n//\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("L>_   _-< 7/|", headcolor) + ColorString("_-__,__-)\\,__)(\".  \\_>-<_/D\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(")D\" Y \"c)  9)  ", headcolor) + ColorString("     //V     \\_\"-._.__G<D     __\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("\\  ||    . _/ ", headcolor) + ColorString("      (       \\ \"-._\"> G<D-_.-\"_=)_\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" ) __   / /  ", headcolor) + ColorString("       \"      _/.__ />_c__.-__<\"/ ( \\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString(" | | |  |(|  ", headcolor) + ColorString("             < \"-._\"> _.G_.___)\\   \\7\\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("  \\\"=\" // |   ", headcolor) + ColorString("           (,\"-.__.|\\ \\<.__.-\" )   \\ \\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("   '---'  |     ", headcolor) + ColorString("         |,\"-.__\"| \\!\"-.__.-\".\\   \\ \\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("     |_;._/    ", headcolor) + ColorString("          (_\"-.__\"'\\ \\\"-.__.-\".|    \\_\\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("     )(\" V     ", headcolor) + ColorString("           \\\"-.__\"'|\\ \\-.__.-\".)     \\ \\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("        (     ", headcolor) + ColorString("             \"-.__'\"\\_\\ \\.__.-\"./      \\ l\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("         )          ", headcolor) + ColorString("        \".__\"\">>G\\ \\__.-\">        V )\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                                \"\"  G<\\ \\_.-\"        ./7\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                                     G<\\ \\          ///\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                                ___  G<D\\ \\        /7^\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                              /\".-_\"-\"G\"<\\ \\__    ///))\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                              ||,-_ \"     '.\\_\"- /// (\\\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                              | (_)  _   \"/ __7 |X/   V\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                             /\\__\"\"-/ \"\\ _\"\"c_\"GBV\n", skeletoncolor), vcursor); vcursor.y++;
    t->put(ColorString("                            _)(  \"\"'    \"--.c__>D'\n", skeletoncolor), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// Flame Horse Functions

FlameHorse::FlameHorse(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('M', dngutil::LIGHTRED),
    coord,
    "Flame Horse",
    false,
    dngutil::TID::FlameHorse,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('|', dngutil::DARKGRAY),
        coord,
        "Flame Charge",
        false,
        2,
        5,
        95,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTRED),
        coord,
        "Flame Shield",
        false,
        dngutil::TID::Secondary,
        0,
        1.15,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("MinibossTheme"),
    80,
    WavFile("Screech", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
) 
{
    setMaxhp(getHp() * 2);
    setHp(getMaxhp());
}

ColorString FlameHorse::getBattleInfo() const
{
    return ColorString("A flare of heat comes upon you", dngutil::RED);
}

void FlameHorse::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    t->put(ColorString("                      . . . .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("                      ,`,`,`,`,\n", color), vcursor); vcursor.y++;
    t->put(ColorString(". . . .               `\\`\\`\\`\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString("`\\`\\`\\`\\`,            ~|;!;!;\\!\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ~\\;\\;\\;\\|\\          (--,!!!~`!       .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("(--,\\\\\\===~\\         (--,|||~`!     ./\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" (--,\\\\\\===~\\         `,-,~,=,:. _,//\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  (--,\\\\\\==~`\\        ~-=~-.---|\\;/J,\n", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString("   (--,\\\\\\((```==.    ~'`~/       a |\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     (-,.\\\\('('(`\\\\.  ~'=~|     \\_.  \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        (,--(,(,(,'\\\\. ~'=|       \\\\_;>\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-( ,(,(,;\\\\ ~=/        \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-/ (.(.(,;\\\\,/          )\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           (,--/,;,;,;,\\\\         ./------.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("             (==,-;-'`;'         /_,----`. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     ,.--_,__.-'                    `--.  ` \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    (='~-_,--/        ,       ,!,___--. \\  \\_)\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (-/~(     |         \\   ,_-         | ) /_|\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (~/((\\    )\\._,      |-'         _,/ /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    \\\\))))  /   ./~.    |           \\_\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ,__/////  /   /    )  /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  '===~'   |  |    (, <.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           / /       \\. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("         _/ /          \\_\\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        /_!/            >_\\\n", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// Ghost Horse Functions

GhostHorse::GhostHorse(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('M', dngutil::WHITE),
    coord,
    "Ghost Horse",
    false,
    dngutil::TID::GhostHorse,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('|', dngutil::DARKGRAY),
        coord,
        "Spirit Charge",
        false,
        2,
        5,
        95,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTRED),
        coord,
        "Spirit Shield",
        false,
        dngutil::TID::Secondary,
        0,
        1.15,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("MinibossTheme"),
    80,
    WavFile("Screech", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getHp() * 2);
    setHp(getMaxhp());
}

ColorString GhostHorse::getBattleInfo() const
{
    return ColorString("You feel pressure in your chest", dngutil::WHITE);
}

void GhostHorse::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::WHITE;
    t->put(ColorString("                      . . . .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("                      ,`,`,`,`,\n", color), vcursor); vcursor.y++;
    t->put(ColorString(". . . .               `\\`\\`\\`\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString("`\\`\\`\\`\\`,            ~|;!;!;\\!\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ~\\;\\;\\;\\|\\          (--,!!!~`!       .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("(--,\\\\\\===~\\         (--,|||~`!     ./\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" (--,\\\\\\===~\\         `,-,~,=,:. _,//\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  (--,\\\\\\==~`\\        ~-=~-.---|\\;/J,\n", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString("   (--,\\\\\\((```==.    ~'`~/       a |\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     (-,.\\\\('('(`\\\\.  ~'=~|     \\_.  \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        (,--(,(,(,'\\\\. ~'=|       \\\\_;>\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-( ,(,(,;\\\\ ~=/        \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-/ (.(.(,;\\\\,/          )\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           (,--/,;,;,;,\\\\         ./------.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("             (==,-;-'`;'         /_,----`. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     ,.--_,__.-'                    `--.  ` \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    (='~-_,--/        ,       ,!,___--. \\  \\_)\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (-/~(     |         \\   ,_-         | ) /_|\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (~/((\\    )\\._,      |-'         _,/ /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    \\\\))))  /   ./~.    |           \\_\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ,__/////  /   /    )  /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  '===~'   |  |    (, <.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           / /       \\. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("         _/ /          \\_\\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        /_!/            >_\\\n", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// Basilisk Functions

Basilisk::Basilisk(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('S', dngutil::GREEN),
    coord,
    "Basilisk",
    false,
    dngutil::TID::Basilisk,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('V', dngutil::DARKGRAY),
        coord,
        "Poisonous Fangs",
        false,
        1.6,
        4,
        80,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Scales",
        false,
        dngutil::TID::Secondary,
        0,
        1.25,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("MinibossTheme"),
    80,
    WavFile("Screech", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getHp() * 2);
    setHp(getMaxhp());
}

ColorString Basilisk::getBattleInfo() const
{
    return ColorString("A terrible monster appears", dngutil::RED);
}

void Basilisk::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::GREEN;
    t->put(ColorString(R"(							                      ,_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              ,;;,  .-"```'-.     \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             ,;;;;,/^;/)^/)-.`.___//)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                         <`\ <`\;;;|  \ / \  `-...')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       <\) <\) |;;' \^ | , |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        |` /|` /;'  |\/ =  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      <\/ <\/ /;'  / | ,  /`'-.,_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       \ /;\ /;'  / / =   \  ^   `'.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      _/_;/ /'   | /  "    '--:-.^  `'.)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                   .-' <g>`"`\   / | ,    _,="/  '.    \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  /\__    \\ Ss\ | /   ,="   |     \ ^  \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ( ( ,)   | \ `S|| | ,"      L_     |  ^ |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  \) `-.  | ||  \| |     _,=.-"`    /    |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        | | || ^ \/   ,="  /__  _.-'  ^ /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        / | //   /  ,"    |   ``"-._^_.')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       /  \_/   /         '--,^     `-.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      |  ^    ^;     _,==""/`     ^   `\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      \ ^      |   ,"  _.-'    ^  .-.^  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       \    ^  ;     (`    ^   .'`\  ) ^|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        '.^    \   ,}` ^__,.-'`\ ^ '' ^ /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          '-.^__\    \'`        `-.^_.-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                            /   {     })", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                            `//"`.___.')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        ,==='`=,__||___)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                         ,=`=``(,=,=.-=;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                 (/(|  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

std::string Basilisk::drop()
{
    Item* itemToAdd;
    itemToAdd = new BasiliskHorn(getPGame(), Coordinate(-1, -1));
    getPGame()->getPlayer()->addToInventory(itemToAdd);
    return "Basilisk Horn";
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// Desert Gryhon Functions

DesertGryphon::DesertGryphon(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('M', dngutil::BLACK),
    coord,
    "Desert Gryphon",
    false,
    dngutil::TID::DesertGryphon,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('=', dngutil::BROWN),
        coord,
        "Gryphon Claws",
        false,
        1.6,
        4,
        80,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Gryphon Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.3,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("MinibossTheme"),
    80,
    WavFile("Screech", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(static_cast<unsigned int>(getHp() * 2.3));
    setHp(getMaxhp());
}

ColorString DesertGryphon::getBattleInfo() const
{
    return ColorString("The Desert Gryphon towers above you.", dngutil::RED);
}

void DesertGryphon::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::BROWN;
    t->put(ColorString(R"(            //           //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           ///          ///)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          ////         ////)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          |////       /////)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          |))//;     /)))//;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         /)))))/;   /)))))/;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     .---`,))))/;  /)))))))/;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( __--\/6-  \`))/; |)))))))/;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"((----/    \\\``;  |))))))/;)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(   ~/-\  \\\\\``   \))))))/;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       \\\\\\\\`    |)))))/;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       |\\\\\\\\___/))))))/;__-------.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       //////|  %%_/))))))/;           \___,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      |||||||\   \%%%%VLK;:              \_. \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      |\\\\\\\\\                        |  | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       \\\\\\\                          |  | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |\\\\               __|        /   / /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        | \\__\     \___----  |       |   / /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |    / |     >     \   \      \  / /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |   /  |    /       \   \      >/ /  ,,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |   |  |   |         |   |    // /  //,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |   |  |   |         |   |   /| |   |\\,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     _--'   _--'   |     _---_---'  |  \ \__/\|/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    (-(-===(-(-(===/    (-(-=(-(-(==/   \____/)", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 65;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

std::string DesertGryphon::drop()
{
    Item* itemToAdd;
    itemToAdd = new SunCharm(getPGame(), Coordinate(-1, -1));
    getPGame()->getPlayer()->addToInventory(itemToAdd);
    return "Sun Charm";
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// True Zorlock Functions

TrueZorlock::TrueZorlock(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('H', dngutil::DARKGRAY),
    coord,
    "True Zorlock",
    false,
    dngutil::TID::TrueZorlock,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Hell-Strike",
        false,
        2,
        6,
        80,
        false,
        "You cant get this so this doesnt matter",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTRED),
        coord,
        "Hell-Shield",
        false,
        dngutil::TID::Secondary,
        0,
        1.4,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("ZorlockTheme"),
    80,
    WavFile("ZorlockDying", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getHp() * 12);
    setHp(getMaxhp());
}

ColorString TrueZorlock::getBattleInfo() const
{
    return ColorString("\"This is where your miserable little life ends\"", dngutil::LIGHTGREEN);
}

void TrueZorlock::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::LIGHTGREEN;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

void TrueZorlock::beginingCutscene()
{
    Game* g = getPGame();
    VirtualWindow* v = g->getVWin();
    TextMacros& t = v->txtmacs;
    Coordinate vcursor(10, t.DIVIDER_LINES[1] + 1);

    t.clearMapArea(true, 10);
    t.clearDivider("bottom");

    playSound(WavFile("ZorlockTransform", true, true));
    for (int i = 35; i > 0; i--)
    {
        t.clearMapArea(false, NULL);
        vcursor.y = t.DIVIDER_LINES[1] + 1;
        if (i % 2 == 0)
        {
            vcursor.x = 0;
            v->put(ColorString(R"(           _/          ,          .                                          )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       /,'    /  /      '    /                                               )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       \|    /  (          ,'                                                )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", dngutil::DARKGRAY), vcursor); vcursor.y++;

        }
        else
        {
            vcursor.x = 0;
            v->put(ColorString(R"(           _/          ,          .                                          )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(       /,'    /  /      '    /                                               )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(       \|    /  (          ,'                                                )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
            v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", dngutil::LIGHTGREEN), vcursor); vcursor.y++;
        }
        Sleep(35);
    }
    stopSound(SoundType::WAV);

    int l = t.BOTTOM_DIVIDER_TEXT_LINE;

    v->putcen(ColorString("- True Zorlock - ", dngutil::LIGHTGREEN), l, true);
    playSound(WavFile("ZorlockAppears", false, false));
    t.clearLine(l);

    v->putcen(ColorString("Ugh, you have made me unleash my final form!", dngutil::LIGHTGREEN), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    playSound(WavFile("ZorlockLaugh", false, false));

    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");

    Sleep(2000);
    std::string name = getPGame()->getPlayer()->getName();
    v->putcenSlowScroll(ColorString(name + "...", dngutil::DARKGRAY), l);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcenSlowScroll(ColorString(name + "!", dngutil::DARKGRAY), l);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcen(ColorString("It is I, The hero from generations past.", dngutil::DARKGRAY), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcen(ColorString("I can provide you one gift to defeat Zorlock...", dngutil::DARKGRAY), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    vcursor.y = t.DIVIDER_LINES[1] + 1;
    v->putcen(ColorString(R"( _________________________ )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|<><><>     |  |    <><><>|)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|<>         |  |        <>|)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|           |  |          |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|  (______ <\-/> ______)  |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|  /_.-=-.\| " |/.-=-._\  |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|   /_    \(o_o)/    _\   |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|    /_  /\/ ^ \/\  _\    |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|      \/ | / \ | \/      |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|_______ /((( )))\ _______|)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|      __\ \___/ /__      |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|--- (((---'   '---))) ---|)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|           |  |          |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(|           |  |          |)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(:           |  |          :)", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"( \<>        |  |       <>/ )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(  \<>       |  |      <>/  )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(   \<>      |  |     <>/   )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(    `\<>    |  |   <>/'    )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(      `\<>  |  |  <>/'     )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(        `\<>|  |<>/'       )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(          `-.  .-`         )", dngutil::YELLOW), vcursor.y++);
    v->putcen(ColorString(R"(            '--'           )", dngutil::YELLOW), vcursor.y++);

    v->putcenSlowScroll(ColorString("- The Hero's Shield -", dngutil::YELLOW), l);

    getPGame()->getPlayer()->addToInventory(new Secondary(
        getPGame(),
        ColorChar('(', dngutil::YELLOW),
        Coordinate(-1, -1),
        "Hero's Shield",
        false,
        dngutil::TID::Secondary,
        255,
        1.3,
        "A shield forged by the gods. Given in a time of need."
    ));

    playSound(Mp3File("FindVeryImportantItem"));
    Sleep(10000);
    stopSound(SoundType::MP3);

    getPGame()->getPlayer()->swapSecondary(getPGame()->getPlayer()->getInventoryNotConst().back());

    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcenSlowScroll(ColorString("Go....", dngutil::DARKGRAY), l);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");

    playSound(WavFile("EnterBattle", false, false));
}

void TrueZorlock::deathSequence()
{
    VirtualWindow* v = getPGame()->getVWin();
    TextMacros& t = v->txtmacs;
    v->clearScreen();
    Sleep(2500);
    int textLine = 22;

    playSound(Mp3File("DeathToTheDemon"));

    std::string name = getPGame()->getPlayer()->getName();
    v->putcenSlowScroll(ColorString("Curse you " + name + "!", dngutil::MAGENTA), textLine);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcenSlowScroll(ColorString("Curse you " + name + "!", dngutil::MAGENTA), textLine);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcenSlowScroll(ColorString("Do not think this ends here!", dngutil::MAGENTA), textLine);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcenSlowScroll(ColorString("When I break free of this seal the Kingom of Bora", dngutil::MAGENTA), textLine);
    v->putcenSlowScroll(ColorString("will be destroyed again. Mark my words!", dngutil::MAGENTA), textLine + 1);
    Sleep(3500);
    t.clearLine(textLine);
    t.clearLine(textLine + 1);

    v->putcenSlowScroll(ColorString("And I'll reserve a special place in hell", dngutil::RED), textLine);
    v->putcenSlowScroll(ColorString("for all that oppose me!!", dngutil::RED), textLine + 1);
    Sleep(3500);
    t.clearLine(textLine);
    t.clearLine(textLine + 1);

    stopSound(SoundType::MP3);

    playSound(WavFile("ZorlockDeath", false, false));

    Sleep(3500);

    v->putcenSlowScroll(ColorString("Peace has been restored.", dngutil::YELLOW), textLine);
    playSound(Mp3File("Credits"));
    Sleep(10000);
    t.clearLine(textLine);

    credits(dngutil::CreditType::VICTORY, getPGame());
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// WaterHorse Functions

WaterHorse::WaterHorse(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : BEnemy(
    pgame,
    ColorChar('M', dngutil::BLUE),
    coord,
    "Water Horse",
    false,
    dngutil::TID::WaterHorse,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('o', dngutil::DARKGRAY),
        coord,
        "Bubble Blast",
        false,
        1.68,
        4,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTBLUE),
        coord,
        "Water Shield",
        false,
        dngutil::TID::Secondary,
        0,
        1.213,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("MinibossTheme"),
    68,
    WavFile("Screech", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::WIZARD
)
{
    setMaxhp(getHp() * 2);
    setHp(getMaxhp());
}


ColorString WaterHorse::getBattleInfo() const
{
    return ColorString("A vortex of water surrounds you", dngutil::BLUE);
}

void WaterHorse::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::BLUE;
    t->put(ColorString("                      . . . .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("                      ,`,`,`,`,\n", color), vcursor); vcursor.y++;
    t->put(ColorString(". . . .               `\\`\\`\\`\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString("`\\`\\`\\`\\`,            ~|;!;!;\\!\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ~\\;\\;\\;\\|\\          (--,!!!~`!       .\n", color), vcursor); vcursor.y++;
    t->put(ColorString("(--,\\\\\\===~\\         (--,|||~`!     ./\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" (--,\\\\\\===~\\         `,-,~,=,:. _,//\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  (--,\\\\\\==~`\\        ~-=~-.---|\\;/J,\n", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString("   (--,\\\\\\((```==.    ~'`~/       a |\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     (-,.\\\\('('(`\\\\.  ~'=~|     \\_.  \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        (,--(,(,(,'\\\\. ~'=|       \\\\_;>\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-( ,(,(,;\\\\ ~=/        \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("          (,-/ (.(.(,;\\\\,/          )\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           (,--/,;,;,;,\\\\         ./------.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("             (==,-;-'`;'         /_,----`. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("     ,.--_,__.-'                    `--.  ` \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    (='~-_,--/        ,       ,!,___--. \\  \\_)\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (-/~(     |         \\   ,_-         | ) /_|\n", color), vcursor); vcursor.y++;
    t->put(ColorString("   (~/((\\    )\\._,      |-'         _,/ /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("    \\\\))))  /   ./~.    |           \\_\\;\n", color), vcursor); vcursor.y++;
    t->put(ColorString(" ,__/////  /   /    )  /\n", color), vcursor); vcursor.y++;
    t->put(ColorString("  '===~'   |  |    (, <.\n", color), vcursor); vcursor.y++;
    t->put(ColorString("           / /       \\. \\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("         _/ /          \\_\\\n", color), vcursor); vcursor.y++;
    t->put(ColorString("        /_!/            >_\\\n", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// Bloodjaw Functions

BloodjawPhase1::BloodjawPhase1(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('0', dngutil::LIGHTBLUE),
    coord,
    "Bloodjaw",
    false,
    dngutil::TID::BloodJawPhase1,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Bite",
        false,
        1.876,
        5,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Shark Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.313,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("SegDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
}


ColorString BloodjawPhase1::getBattleInfo() const
{
    return ColorString("A pair of eyes start glowing in the water", dngutil::RED);
}

void BloodjawPhase1::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::LIGHTBLUE;
    int eyeColor = dngutil::RED;
    v->put(ColorString(R"(                              , )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              \`-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |   `\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |     \ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                           __/.- - -.\,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                      _.-'`              `'"'--..,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                  .-'`                              `'--.,_ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(               .'`   _                         _ ___       `) )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             .'   .'` `'-.                    (_`  _`)  _.-' )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(           .'    '--.     '.                 .-.`"`)", color) + ColorString(R"(@)", eyeColor) + ColorString(R"( .-'""-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  .------~'     ,.---'      '-._      _.'   /   `'--'"""".-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(/`        '   /`  _,..-----.,__ `''''`/    ;__,..--''--'` )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(`'--.,__ '    |-'`             `'---'|     | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(        `\    \                       \   / )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         |     |                       '-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(          \    | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(           `\  | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             \/)", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

BloodjawPhase2::BloodjawPhase2(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('0', dngutil::LIGHTGRAY),
    coord,
    "Reinforced Bloodjaw",
    false,
    dngutil::TID::BloodJawPhase2,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Reinforced Bite",
        false,
        2,
        5,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Shark Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.313,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("SegDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
    increaseDef(static_cast<int>(getDef() * 1.2));
}

void BloodjawPhase2::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::LIGHTBLUE;
    int eyeColor = dngutil::RED;
    v->put(ColorString(R"(                              , )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              \`-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |   `\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |     \ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                           __/.- - -.\,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                      _.-'`              `'"'--..,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                  .-'`                              `'--.,_ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(               .'`   _                         _ ___       `) )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             .'   .'` `'-.                    (_`  _`)  _.-' )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(           .'    '--.     '.                 .-.`"`)", color) + ColorString(R"(@)", eyeColor) + ColorString(R"( .-'""-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  .------~'     ,.---'      '-._      _.'   /   `'--'"""".-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(/`        '   /`  _,..-----.,__ `''''`/    ;__,..--''--'` )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(`'--.,__ '    |-'`             `'---'|     | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(        `\    \                       \   / )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         |     |                       '-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(          \    | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(           `\  | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             \/)", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

BloodjawPhase3::BloodjawPhase3(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('0', dngutil::LIGHTGRAY),
    coord,
    "Enraged Bloodjaw",
    false,
    dngutil::TID::BloodJawPhase3,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Eye Laser",
        false,
        1.5,
        3,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("MagicAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Weakened Shark Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.283,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getHp() * 2);
    setHp(getMaxhp());
}

void BloodjawPhase3::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::RED;
    int eyeColor = dngutil::YELLOW;
    v->put(ColorString(R"(                              , )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              \`-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |   `\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                              |     \ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                           __/.- - -.\,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                      _.-'`              `'"'--..,__ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(                  .-'`                              `'--.,_ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(               .'`   _                         _ ___       `) )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             .'   .'` `'-.                    (_`  _`)  _.-' )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(           .'    '--.     '.                 .-.`"`)", color) + ColorString(R"(@)", eyeColor) + ColorString(R"( .-'""-, )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  .------~'     ,.---'      '-._      _.'   /   `'--'"""".-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(/`        '   /`  _,..-----.,__ `''''`/    ;__,..--''--'` )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(`'--.,__ '    |-'`             `'---'|     | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(        `\    \                       \   / )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         |     |                       '-' )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(          \    | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(           `\  | )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(             \/)", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// Evil Beast Functions

EvilBeastPhase1::EvilBeastPhase1(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::MAGENTA),
    coord,
    "Zorlock",
    false,
    dngutil::TID::EvilBeastPhase1,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Piercing Glare",
        false,
        1.67,
        4,
        65,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack1", false, false),
        dngutil::ClassType::RANGER
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Evil Aura",
        false,
        dngutil::TID::Secondary,
        0,
        1.3,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("Laugh", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::RANGER
)
{
    
}


ColorString EvilBeastPhase1::getBattleInfo() const
{
    return ColorString("You can't breath", dngutil::MAGENTA);
}

void EvilBeastPhase1::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::MAGENTA;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

EvilBeastPhase2::EvilBeastPhase2(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::MAGENTA),
    coord,
    "Zorlock",
    false,
    dngutil::TID::EvilBeastPhase2,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Wing",
        false,
        1.6,
        3,
        70,
        false,
        "You cant get this so this doesnt matter",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Evil Aura",
        false,
        dngutil::TID::Secondary,
        0,
        1.3,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("SegDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
    
}

void EvilBeastPhase2::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::MAGENTA;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

EvilBeastPhase3::EvilBeastPhase3(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('0', dngutil::LIGHTGRAY),
    coord,
    "Flaming Zorlock",
    false,
    dngutil::TID::EvilBeastPhase3,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Fire of the Damned",
        false,
        2,
        5,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Fire Shield",
        false,
        dngutil::TID::Secondary,
        0,
        1.28,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getMaxhp() * 2);
    setHp(getMaxhp());
}

void EvilBeastPhase3::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

void EvilBeastPhase3::deathSequence()
{
    getPGame()->getVWin()->txtmacs.displayGame(getPGame());
}

EvilBeastPhase4::EvilBeastPhase4(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::LIGHTRED),
    coord,
    "Flaming Zorlock II",
    false,
    dngutil::TID::EvilBeastPhase4,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Death Flameburst",
        false,
        5,
        8,
        100,
        false,
        "You cant get this so this doesnt matter",
        WavFile("MagicAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::LIGHTBLUE),
        coord,
        "Aura",
        false,
        dngutil::TID::Secondary,
        0,
        1.45,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    68,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(getMaxhp() * 2);
    setHp(getMaxhp());
}

void EvilBeastPhase4::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();

    int color = dngutil::LIGHTRED;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// Pit Dragon Functions

ForestDragonPhase1::ForestDragonPhase1(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('S', dngutil::GREEN),
    coord,
    "Forest Dragon",
    false,
    dngutil::TID::ForestDragonPhase1,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('*', dngutil::LIGHTGREEN),
        coord,
        "Leaf Storm",
        false,
        1.3,
        3,
        80,
        false,
        "You cant get this so this doesnt matter",
        WavFile("BowAttack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTRED),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.2,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    80,
    WavFile("SegDeath", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
     
}

void ForestDragonPhase1::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::GREEN;
    t->put(ColorString(R"(             __                  __)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            ( _)                ( _))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           / / \\              / /\_\_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          / /   \\            / / | \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         / /     \\          / /  |\ \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        /  /   ,  \ ,       / /   /|  \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /  /    |\_ /|      / /   / \   \_\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      /  /  |\/ _ '_| \   / /   /   \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |  /   |/  0 \0\    / |    |    \    \\)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(     |    |\|      \_\_ /  /    |     \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |  | |/    \.\ o\o)  /      \     |    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     \    |     /\\`v-v  /        |    |     \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      | \/    /_| \\_|  /         |    | \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      | |    /__/_ `-` /   _____  |    |  \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      \|    [__]  \_/  |_________  \   |   \    ())", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /    [___] (    \         \  |\ |   |   //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      |    [___]                  |\| \|   /  |/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     /|    [____]                  \  |/\ / / ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    (  \   [____ /     ) _\      \  \    \| | ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     \  \  [_____|    / /     __/    \   / / //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |   \ [_____/   / /        \    |   \/ //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |   /  '----|   /=\____   _/    |   / //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  __ /  /        |  /   ___/  _/\    \  | ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( (/-(/-\)       /   \  (/\/\)/  |    /  | /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               (/\/\)           /   /   //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      _________/   /    /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     \____________/    ()", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

ColorString ForestDragonPhase1::getBattleInfo() const
{
    return ColorString("A terrible, mean face looks down at you", dngutil::WHITE);
}


ForestDragonPhase2::ForestDragonPhase2(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('S', dngutil::RED),
    coord,
    "Forest Dragon",
    false,
    dngutil::TID::ForestDragonPhase2,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('o', dngutil::LIGHTRED),
        coord,
        "Fireball",
        false,
        1.45,
        4,
        80,
        false,
        "You cant get this so this doesnt matter",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::LIGHTRED),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.2,
        "You cant get this so this doesnt matter"
    ),
    Mp3File("DungeonBoss"),
    80,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    
}

void ForestDragonPhase2::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    t->put(ColorString(R"(             __                  __)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            ( _)                ( _))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           / / \\              / /\_\_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          / /   \\            / / | \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         / /     \\          / /  |\ \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        /  /   ,  \ ,       / /   /|  \ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /  /    |\_ /|      / /   / \   \_\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      /  /  |\/ _ '_| \   / /   /   \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |  /   |/  0 \0\    / |    |    \    \\)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(     |    |\|      \_\_ /  /    |     \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |  | |/    \.\ o\o)  /      \     |    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     \    |     /\\`v-v  /        |    |     \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      | \/    /_| \\_|  /         |    | \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      | |    /__/_ `-` /   _____  |    |  \    \\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      \|    [__]  \_/  |_________  \   |   \    ())", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /    [___] (    \         \  |\ |   |   //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      |    [___]                  |\| \|   /  |/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     /|    [____]                  \  |/\ / / ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    (  \   [____ /     ) _\      \  \    \| | ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     \  \  [_____|    / /     __/    \   / / //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |   \ [_____/   / /        \    |   \/ //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     |   /  '----|   /=\____   _/    |   / //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  __ /  /        |  /   ___/  _/\    \  | ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( (/-(/-\)       /   \  (/\/\)/  |    /  | /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               (/\/\)           /   /   //)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      _________/   /    /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     \____________/    ()", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// mega beasts Functions

MegaBeastPhase1::MegaBeastPhase1(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('S', dngutil::LIGHTMAGENTA),
    coord,
    "Lost Spirit",
    false,
    dngutil::TID::MegaBeastPhase1,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Spirit's Breath",
        false,
        1,
        5,
        100,
        false,
        ".",
        WavFile("Attack1", false, false),
        dngutil::ClassType::ADVENTURER
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Spirits Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1,
        "Spirits's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("revival", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::ADVENTURER
)
{
    increaseDef(static_cast<unsigned int>(getDef() * 0.5));
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.5));
    setHp(getMaxhp());
}

ColorString MegaBeastPhase1::getBattleInfo() const
{
    return ColorString("You get a feeling of impending doom...", dngutil::MAGENTA);
}

void MegaBeastPhase1::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::WHITE;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		        *             )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(		       ***            )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		      *****           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		     *******          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		      *****           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		       ***            )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		        *             )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(		                      )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

void MegaBeastPhase1::deathSequence()
{

}

MegaBeastPhase2::MegaBeastPhase2(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('S', dngutil::LIGHTMAGENTA),
    coord,
    "Mega Beast",
    false,
    dngutil::TID::MegaBeastPhase2,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Mega Beam",
        false,
        2,
        5,
        100,
        false,
        "A magical beam.",
        WavFile("MagicAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Mega Beast Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.2,
        "Beast's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::WIZARD
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.25));
    setHp(getMaxhp());
}

void MegaBeastPhase2::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::RED;
    t->put(ColorString(R"(		             \                  /)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		    _________))                ((__________)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		   /.-------./\\    \    /    //\.--------.\)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		  //#######//##\\   ))  ((   //##\\########\\)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		 //#######//###((  ((    ))  ))###\\########\\)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		((#######((#####\\  \\  //  //#####))########)))", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		 \##' `###\######\\  \)(/  //######/####' `##/)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		  )'    ``#)'  `##\`->oo<-'/##'  `(#''     `()", color), vcursor);  vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(		          (       ``\`..'/''       ))", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                     \""()", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                      `- ))", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                      / /)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                     ( /\)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                     /\| \)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                    (  \)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                        ))", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                       /)", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                      ()", color), vcursor);  vcursor.y++;
    t->put(ColorString(R"(		                      ` )", color), vcursor);

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------


//----------------------------------------------------------------
// Zorlock Image Functions

ZorlockImagePhase1::ZorlockImagePhase1(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::DARKGRAY),
    coord,
    "Zorlock Image",
    false,
    dngutil::TID::ZorlockImagePhase1,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Image Flame",
        false,
        1.5,
        5,
        100,
        false,
        ".",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::ADVENTURER
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Image Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.23,
        "Images's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("revival", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::ADVENTURER
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.5));
    setHp(getMaxhp());
}

void ZorlockImagePhase1::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::DARKGRAY;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

void ZorlockImagePhase1::beginingCutscene()
{
    Game* g = getPGame();
    VirtualWindow* v = g->getVWin();
    TextMacros& t = v->txtmacs;
    Coordinate vcursor(10, t.DIVIDER_LINES[1] + 1);

    t.clearMapArea(true, 10);
    t.clearDivider("bottom");

    int l = t.BOTTOM_DIVIDER_TEXT_LINE;
    v->putcen(ColorString("I present thee with a challenge made by the Gods.", dngutil::WHITE), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);
}

ZorlockImagePhase2::ZorlockImagePhase2(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('S', dngutil::LIGHTMAGENTA),
    coord,
    "Zorlock Image II",
    false,
    dngutil::TID::ZorlockImagePhase2,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Image Flame II",
        false,
        1.7,
        4,
        100,
        false,
        "fire",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::DARKGRAY),
        coord,
        "Image Armor II",
        false,
        dngutil::TID::Secondary,
        0,
        1.35,
        "Image's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::WIZARD
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.8));
    setHp(getMaxhp());
}

void ZorlockImagePhase2::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* v = getPGame()->getVWin();
    int color = dngutil::DARKGRAY;
    v->put(ColorString(R"(           _/          ,          .                                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /,'    /  /      '    /                                               )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \|    /  (          ,'                                                )", color), vcursor); vcursor.y++;
    v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

void ZorlockImagePhase2::deathSequence()
{
    Game* g = getPGame();
    VirtualWindow* v = g->getVWin();
    TextMacros& t = v->txtmacs;
    Coordinate vcursor(10, t.DIVIDER_LINES[1] + 1);

    playSound(Mp3File("DeathToTheDemon"));

    t.clearMapArea(true, 10);
    t.clearDivider("bottom");

    int l = t.BOTTOM_DIVIDER_TEXT_LINE;
    if (getPGame()->getPlayer()->hasItem(dngutil::TID::GodStone))
    {
        v->putcen(ColorString("Thou has done well", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("I present thee with a prize - 150 Gold", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        getPGame()->getPlayer()->changeGold(150);
        playSound(WavFile("FindItem", false, false));
        
        stopSound(SoundType::MP3);
    }
    else
    {
        v->putcen(ColorString("Thou has done well", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("I present thee with a prize - The God's Stone", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        getPGame()->getPlayer()->addToInventory(new GodStone(getPGame(), Coordinate(-1, -1)));
        playSound(WavFile("FindItem", false, false));

        stopSound(SoundType::MP3);

        v->putcen(ColorString("BEWARE THE IMPOSTER", dngutil::WHITE), l);
        playSound(WavFile("Attack4", false, false));
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("BEWARE THE IMPOSTER", dngutil::LIGHTMAGENTA), l);
        playSound(WavFile("Attack4", false, false));
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("BEWARE THE IMPOSTER", dngutil::MAGENTA), l);
        playSound(WavFile("Attack4", false, false));
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("BEWARE THE IMPOSTER", dngutil::RED), l);
        playSound(WavFile("Attack4", false, false));
        Sleep(3500);
        t.clearLine(l);
        t.clearLine(l + 1);
    }
}
//----------------------------------------------------------------



//----------------------------------------------------------------
// LSKnight
LSKnight::LSKnight(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('A', dngutil::MAGENTA),
    coord,
    "Cursed Knight",
    false,
    dngutil::TID::LSKnight,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('T', dngutil::MAGENTA),
        coord,
        "Cursed Broadsword",
        false,
        2,
        6,
        85,
        false,
        "A broadsword from a cursed knight",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::MAGENTA),
        coord,
        "Cursed Shield",
        false,
        dngutil::TID::Secondary,
        110,
        1.15,
        "A shield from a cursed knight"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(26, 33),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{

}

void LSKnight::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::MAGENTA;
    t->put(ColorString(R"(              .)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             /.\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|   ,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|  ;\  /:)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.| /  \/  \)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(             |.|<.<_\/_>,>)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.| \`.::,'/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|,'.'||'/.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          ,-'|.|.`.____,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        ,' .`|.| `.____,;/ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       ,'=-.`|.|\ .   \ |,':)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      /_   :)|.|.`.___:,:,'|.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     (  `-:;\|.|.`.)  |.`-':,\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     /.   /  ;.:--'   |    | ,`.    _____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    / _>-'._.'-'.     |.   |' / )._/     \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   :.'    ((.__;/     |    |._ /__/   |   |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   `.>._.-' |)=(      |.   ;  '--.|  -|-  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            ',--'`-._ | _,:       |   |   |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            /_`-. `..`:'/_.\       \  |  /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           :__``--..\\_/_..:        \___/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           |  ``--..,:;\__.|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           |`--..__/:;  :__|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           `._____:-;_,':__;)", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// SSKnight
SSKnight::SSKnight(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('A', dngutil::MAGENTA),
    coord,
    "Cursed Knight",
    false,
    dngutil::TID::SSKnight,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('T', dngutil::MAGENTA),
        coord,
        "Cursed Shortsword",
        false,
        1,
        2,
        85,
        false,
        "A shortsword from a cursed knight",
        WavFile("Attack2", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::MAGENTA),
        coord,
        "Cursed Shield",
        false,
        dngutil::TID::Secondary,
        110,
        1.15,
        "A shield from a cursed knight"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(26, 33),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{

}

void SSKnight::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::MAGENTA;
    t->put(ColorString(R"(               )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   ,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  ;\  /:)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              .  /  \/  \)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(             /.\<.<_\/_>,>)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.| \`.::,'/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             |.|,'.'||'/.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          ,-'|.|.`.____,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        ,' .`|.| `.____,;/ \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       ,'=-.`|.|\ .   \ |,':)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      /_   :)|.|.`.___:,:,'|.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     (  `-:;\|.|.`.)  |.`-':,\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     /.   /  ;.:--'   |    | ,`.    _____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    / _>-'._.'-'.     |.   |' / )._/     \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   :.'    ((.__;/     |    |._ /__/   |   |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   `.>._.-' |)=(      |.   ;  '--.|  -|-  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            ',--'`-._ | _,:       |   |   |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            /_`-. `..`:'/_.\       \  |  /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           :__``--..\\_/_..:        \___/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           |  ``--..,:;\__.|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           |`--..__/:;  :__|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           `._____:-;_,':__;)", color), vcursor); vcursor.y++;

    const int LONGEST_LINE_LENGTH = 59;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Mage
Mage::Mage(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('I', dngutil::YELLOW),
    coord,
    "Mage",
    false,
    dngutil::TID::Mage,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('i', dngutil::YELLOW),
        coord,
        "Staff",
        false,
        1.5,
        4,
        85,
        false,
        "A Mage's staff - spews electricity",
        WavFile("MagicAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::YELLOW),
        coord,
        "Magic Aura",
        false,
        dngutil::TID::Secondary,
        10,
        1.12,
        "A protecting aura from a mage"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(35, 40),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::ATTACK,
    dngutil::ClassType::WIZARD
)
{

}

void Mage::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::YELLOW;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             _,-'|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          ,-'._  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                .||,      |####\ |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               \.`',/     \####| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               = ,. =      |###| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               / || \    ,-'\#/,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||     ,'   `,,. `.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ,|____,' , ,;' \| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                (3|\    _/|/'   _| |)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                 ||/,-''  | >-'' _,\\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||'      ==\ ,-'  ,')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |  V \ ,|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    |` |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    |   \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    \    \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |     |    \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |      \_,-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |___,,--")_\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||         |_|   ccc/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||        ccc/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||         )", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 48;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------


//----------------------------------------------------------------
// Cultist
Cultist::Cultist(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('I', dngutil::LIGHTMAGENTA),
    coord,
    "Cultist",
    false,
    dngutil::TID::Cultist,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('i', dngutil::LIGHTMAGENTA),
        coord,
        "Cultist's Staff",
        false,
        1.45,
        3,
        70,
        false,
        "A Cultist's staff - spews dark magic",
        WavFile("MagicAttack1", false, false),
        dngutil::ClassType::WIZARD
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::MAGENTA),
        coord,
        "Cultist's Aura",
        false,
        dngutil::TID::Secondary,
        200,
        1,
        "Aura from a cultist"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(60, 80),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::WIZARD
)
{

}

void Cultist::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTMAGENTA;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"()", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             _,-'|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          ,-'._  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                .||,      |####\ |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               \.`',/     \####| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               =\/\/=      |###| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               / || \    ,-'\#/,'`.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||     ,'   `,,. `.)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ,|____,' , ,;' \| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                (3|\    _/|/'   _| |)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                 ||/,-''  | >-'' _,\\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||'      ==\ ,-'  ,')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |  V \ ,|)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    |` |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    |   \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |    \    \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |     |    \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |      \_,-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||       |___,,--")_\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||         |_|   ccc/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||        ccc/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 ||         )", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 48;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Imp
Imp::Imp(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('H', dngutil::LIGHTRED),
    coord,
    "Imp",
    false,
    dngutil::TID::Imp,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('=', dngutil::LIGHTRED),
        coord,
        "Imp's Claw",
        false,
        1.4,
        3,
        70,
        false,
        "Claw from an imp",
        WavFile("Attack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('(', dngutil::RED),
        coord,
        "Invisible Shield",
        false,
        dngutil::TID::Secondary,
        133,
        1.2,
        "Invisible shield from an imp"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(50, 65),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{

}

void Imp::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    t->put(ColorString(R"(                                     ,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       ,  .   (          )          -.\ |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       | / .- |\        /|         _  \'/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        \'/   | \.-""-./ |          \_) ;-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     `'-; (_/ ;   _  _   ;           ) /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         \ (   \ (.\/.) /    .-.    / |)", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(          \ \   \ \/\/ /-._.'   \   | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           \ \   \ .. /_         \  | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            \ \   |  |__)     |\  \ | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             \ `"`|==|_       | \  \| |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              \,-'|==| \      |  \    |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   \/   '.    /   `\ /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          |   |     `   ,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          |   |         )\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                __.....__/    |        /  \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              /`              \        `//`)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              |  \`-....-'\    `-.____.'/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              |  |        /   /`"-----'`)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              |  |        |  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              | /         |  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       .------' \         /  /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      (((--------'        \  |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                           | \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                           | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                           | |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                           | /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          /  ))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                         /   |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        (-(-(')", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 48;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Desert Goblin
DesertGoblin::DesertGoblin(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : REnemy(
    pgame,
    ColorChar('o', dngutil::CYAN),
    coord,
    "Goblin",
    false,
    dngutil::TID::DesertGoblin,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('=', dngutil::WHITE),
        coord,
        "Offensive Claws",
        false,
        1.34,
        3,
        60,
        false,
        "Claws from a Goblin",
        WavFile("Attack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('=', dngutil::WHITE),
        coord,
        "Defensive Claw",
        false,
        dngutil::TID::Secondary,
        0,
        1,
        "Claws used for defense from a Goblin"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(40, 58),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::KNIGHT
)
{

}

void DesertGoblin::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::CYAN;
    vcursor.y += 3;
    t->put(ColorString(R"(               ,     _.._     ,)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               (`._."`    `"._.'))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                '._          _.'               /\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 | /`-.  .-'\ | __           .'.')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 |(_()_\/_()_)|'  `\        ( ()", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                 ;   ,____,   ;     \        ) ))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  \  /VvvV\  /    \  \.__   / /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 /`'._`""`_.'      \  \  `\/ /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                /  .  `--'          \  \    /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               /  / `-,       _.----'   \  ;)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(              /  /     )     /  .--------` \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             /  /.----'     /  /   ___.     \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            /  /|   _    _,|  (---'   \     |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(           /  / |   \`""`  \\\\        \    |)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          / /`  |    |      \\\`        \   \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         / /    ;    |                  /   /)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        / / _    \  /                 /`  /`)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       / _\/(    | |                 /  .'_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      | ( \  '--'  \               .'  (__)`\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      \\\\  `-------'             /________.')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       `\\\)", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 50;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Bowman
Bowman::Bowman(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : REnemy(
    pgame,
    ColorChar('A', dngutil::LIGHTGREEN),
    coord,
    "Bowman",
    false,
    dngutil::TID::Bowman,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar(')', dngutil::BROWN),
        coord,
        "Reinforced Bow",
        false,
        1.5,
        3,
        75,
        false,
        "A very sturdy bow.",
        WavFile("BowAttack1", false, false),
        dngutil::ClassType::RANGER
    ),
    new Secondary(
        pgame,
        ColorChar('=', dngutil::BROWN),
        coord,
        "Quiver",
        false,
        dngutil::TID::Secondary,
        0,
        1,
        "A quiver from a bowman."
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(35, 40),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::RANGER
)
{

}

void Bowman::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::GREEN;
    int bowcolor = dngutil::BROWN;
    int arrowcolor = dngutil::WHITE;
    t->put(ColorString(R"(                                                         /|.)", bowcolor), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                                                       /  `|.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                     /     |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                   /       |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                 /         `|.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                               /            |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                             /              |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                           /                |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(      __                                 /                  `|.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(       -\                              /                     |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(         \\                          /                       |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(           \\                      /                         |.)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(            \|                   /                           |\)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(              \#####\    )", color) + ColorString(R"(      /                             ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(          ==###########>  )", color) + ColorString(R"(   /                               ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(           \##==      \  )", color) + ColorString(R"(  /                                 ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(      ______ =       =|__)", color) + ColorChar('/', bowcolor) + ColorString(R"(___                                ||)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  ,--' ,----`-,__ ___/'  --,-`-)", color) + ColorString(R"(==============================##==========>)", arrowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"( \               '        ##_______ ______   ______,--,____,=##,__)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  `,    __==    ___,-,__,--'#'  ==='      `-'              | ##,-/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    `-,____,---'       \####)", color) + ColorChar('\\', bowcolor) + ColorString(R"(              |        ____,--\_##,/)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(        #_              |##   )", color) + ColorChar('\\', bowcolor) + ColorString(R"(  _____,---==,__,---'         ##)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(         #              ]===--==)", color) + ColorString(R"(\                            ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(         #,             ]         )", color) + ColorString(R"(\                          ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(          #_            |           )", color) + ColorString(R"(\                        ||)", bowcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(           ##_       __/'             )", color) + ColorString(R"(\                      ||)", bowcolor), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 10;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------

//----------------------------------------------------------------
// Robber
Robber::Robber(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SmartEnemy(
    pgame,
    ColorChar('A', dngutil::DARKGRAY),
    coord,
    "Robber",
    false,
    dngutil::TID::Robber,
    hp, att, def, lck, spd, lvl,
    new Primary(
        pgame,
        ColorChar('|', dngutil::WHITE),
        coord,
        "Dagger",
        false,
        1.2,
        2,
        90,
        false,
        "A very light dagger.",
        WavFile("Attack1", false, false),
        dngutil::ClassType::ADVENTURER
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::DARKGRAY),
        coord,
        "Robber's Charm",
        false,
        dngutil::TID::Secondary,
        50,
        1.15,
        "A charm from a robber."
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(35, 47),
    WavFile("EnemyDeath", false, false),
    dngutil::EvType::SPEED,
    dngutil::ClassType::ADVENTURER
)
{

}

void Robber::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::DARKGRAY;
    t->put(ColorString(R"(                       __.------.                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      (__  ___   )                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        .)e  )\ /                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       /_.------                           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                       _/_    _/                           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   __.'  / '   `-.__                       )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  / <.--'           `\                     )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                 /   \   \c           |                    )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                /    /    )  GoT  x    \                   )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                |   /\    |c     / \.-  \                  )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                \__/  )  /(     (   \   <>'\               )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     / _/ _\-    `-. \/_|_ /<>             )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    / /--/,-\     _ \     <>.`.            )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    \/`--\_._) - /   `-/\    `.\           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     /        `.     /   )     `\          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     \      \   \___/----'                 )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     |      /    `(                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     \    ./\_   _ \                       )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                     /     |  )    '|                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    |     /   \     \                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   /     |     |____.)                     )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  /      \      \___/                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 \_      :)                                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                /` `-----'                                 )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                \____|                                     )", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH =55;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}
//----------------------------------------------------------------




//----------------------------------------------------------------
// Seg Enemy

void SegEnemy::deathSequence()
{
    getPGame()->getVWin()->txtmacs.clearMapArea(true, 10);
    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
    playSound(Mp3File("Win"));
    Sleep(10500);
    stopSound(SoundType::MP3);
    Enemy::deathSequence();
}

bool SegEnemy::battle(MapObject* t_enemy)
{
    SegEnemy* enemy = dynamic_cast<SegEnemy*>(t_enemy);
    Player* player = getPGame()->getPlayer();
    VirtualWindow* vwin = getPGame()->getVWin();

    if (getPGame()->getActiveRoom()->getRoomInfo().difficulty == -1)
    {
        while (static_cast<int>(enemy->getLvl()) < getPGame()->getDungeonLevel())
        {
            enemy->increaseLvl(1);
            enemy->levelUpStats();
        }
    }
    else if (getPGame()->getActiveRoom()->getRoomInfo().difficulty < -1)
    {
        int level = getPGame()->getActiveRoom()->getRoomInfo().difficulty;
        level = abs(level);
        level += getPGame()->getDungeonLevel();
        while (static_cast<int>(enemy->getLvl()) < level)
        {
            enemy->increaseLvl(1);
            enemy->levelUpStats();
        }
    }

    int enemyWeaponSpeed = enemy->getPrimary().getAttSpeed();
    int enemyTimer = 0;

    int playerWeaponSpeed = player->getPrimary().getAttSpeed();
    int playerTimer = 0;
    if (player->getPrimary().getStartReady() && enemy->isFirst())
    {
        playerTimer = playerWeaponSpeed;
    }

    if (enemy->getPrimary().getStartReady())
    {
        enemyTimer = enemyWeaponSpeed;
    }

    int prevTime = (int)time(NULL);

    vwin->txtmacs.clearMapArea(false, NULL);
    vwin->txtmacs.clearDivider("bottom");

    while (true)
    {
        enemy->printSelf();
        vwin->txtmacs.displayHealthBars(enemy, player);

        while (true)
        {

            if (prevTime != (int)time(NULL))
            {
                if (playerTimer < playerWeaponSpeed)
                {
                    if (!keypress(VK_RETURN))
                    {
                        playerTimer++;
                    }
                }
                if (enemyTimer < enemyWeaponSpeed)
                {
                    enemyTimer++;
                }
                prevTime = (int)time(NULL);
            }

            vwin->txtmacs.outputBattleInfo(playerTimer, playerWeaponSpeed, enemyTimer, enemyWeaponSpeed);

            if (playerTimer >= 1 && keypress(VK_RETURN) && player->getPrimary().getQuickAttack())
            {
                double damageMultiplier = (static_cast<double>(playerTimer) / playerWeaponSpeed);
                if (playerTimer != playerWeaponSpeed)
                {
                    damageMultiplier *= 0.6; // slight negative for not charging all the way
                }

                playerTimer = 0;

                Damage damage = player->getDamageDealt(enemy);
                damage.damage *= damageMultiplier;
                for (int i = 0; i < damage.damage; i++)
                {
                    enemy->decreaseHealth(1);
                    vwin->txtmacs.displayHealthBars(enemy, player);
                    Sleep(getHealthbarSleepTime(damage.damage));
                }

                vwin->putcen(
                    ColorString("-" + std::to_string(damage.damage), dngutil::GREEN) +
                    ((damage.damageDeflected > 0) ? ColorString(", " + std::to_string(damage.damageDeflected) + " DEFLECTED", dngutil::GREEN) : ColorString("", dngutil::BLACK))
                    , vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);  
                Sleep(300);

                if (enemy->isDead())
                {
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");

                    enemy->getDeathSound().play();

                    getPGame()->getVWin()->txtmacs.clearMapArea(false, NULL);

                    return true;
                }
                break;
            }

            if (enemyTimer >= enemyWeaponSpeed)
            {
                enemyTimer = 0;
                Damage damage = enemy->getDamageDealt(player);
                for (int i = 0; i < damage.damage; i++)
                {
                    player->decreaseHealth(1);
                    vwin->txtmacs.displayHealthBars(enemy, player);
                    Sleep(getHealthbarSleepTime(damage.damage));
                }

                vwin->putcen(
                    ColorString("-" + std::to_string(damage.damage), dngutil::GREEN) +
                    ((damage.damageDeflected > 0) ? ColorString(", " + std::to_string(damage.damageDeflected) + " DEFLECTED", dngutil::GREEN) : ColorString("", dngutil::BLACK))
                    , vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);
                Sleep(300);

                if (player->isDead())
                {
                    bool revived = false;

                    for (auto it = player->getInventoryNotConst().begin(); it != player->getInventoryNotConst().end(); it++)
                    {
                        if ((*it)->getTypeId() == dngutil::TID::MagicalPotion)
                        {
                            Item* potion = *it;
                            player->getInventoryNotConst().erase(it);
                            delete potion;
                            revived = true;
                            break;
                        }
                    }

                    if (!revived)
                    {
                        stopSound(SoundType::MP3);
                        getPGame()->cleanup(getPGame()->getVWin()->txtmacs.deathScreen(getPGame()));
                        return false;
                    }
                    else
                    {
                        player->setHp(0);
                        vwin->txtmacs.displayHealthBars(enemy, player);
                        vwin->txtmacs.outputBattleInfo(playerTimer, playerWeaponSpeed, enemyTimer, enemyWeaponSpeed);

                        playSound(WavFile("MagicalPotion", false, false));
                        playSound(WavFile("RefillHealth", false, true));
                        for (int i = 0; i < (player->getMaxhp() * .75); i++)
                        {
                            player->increaseHealth(1);
                            vwin->txtmacs.displayHealthBars(enemy, player);
                            Sleep(40);
                        }
                        stopSound(SoundType::WAV);
                    }
                }
                break;
            }

            if (keypress('I'))
            {
                player->inventoryMenu();
                getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                if (player->getPrimary().getAttSpeed() != playerTimer)
                {
                    playerWeaponSpeed = player->getPrimary().getAttSpeed();
                    playerTimer = 0;
                }
                break;
            }
        }
    }
}

//----------------------------------------------------------------

//----------------------------------------------------------------
// All Dragon Segments

DragonTail::DragonTail(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::WHITE),
    coord,
    "Dragon's Tail",
    false,
    dngutil::TID::DragonTail,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Tail Whip",
        false,
        2.2,
        6,
        70,
        false,
        "Dragons Tail.",
        WavFile("Attack4", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.25,
        "Dragons's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("SegDeath", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.75));
    setHp(getMaxhp());
}



ColorString DragonTail::getBattleInfo() const
{
    return ColorString("A feeling of dread overcomes your body...", dngutil::MAGENTA);
}

void DragonTail::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    t->put(ColorString(R"(                                                 /===-_---~~~~~~~~~------____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                |===-~___                _,-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 -==\\                         `//~\\   ~~~~`---.___.-~~     )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             ______-==|                         | |  \\           _-~`       )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       __--~~~  ,-/-==\\                        | |   `\        ,'           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    _-~       /'    |  \\                      / /      \      /             )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  .'        /       |   \\                   /' /        \   /'              )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(/-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                 )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  \_|      /        _)   ;  ),   __--~~                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    '~~--_/      _-~/-  / \   '-~ \                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   {\__--_/}    / \\_>- )<__\      \                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   /'   (_/  _-~  | |__>--<__|      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  |0  0 _/) )-~     | |__>--<__|      |                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  / /~ ,_/       / /__>---<__/      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 o o _//        /-~_>---<__-~      /                         )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                 (^(~          /~_>---<__-      _-~                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              /__>--<__/     _-~                             )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|     /                  .----_        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |                 /' _---_~\      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |               /'  /     ~\`\    )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              \__>--<__\    \            /'  //        ||    )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                               ~-__>--<_~-_  ~--____---~' _/'/        /'     )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                  ~-_~>--<_/-__       __-~ _/                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                     ~~-'_/_/ /~~~~~~~__--~                  )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                            ~~~~~~~~~~                       )", color), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 65;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}









DragonWings::DragonWings(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::WHITE),
    coord,
    "Dragon's Wings",
    false,
    dngutil::TID::DragonWings,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Wind Gust",
        false,
        1.4,
        3,
        100,
        false,
        "Dragons Wings.",
        WavFile("Attack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Wing Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1,
        "Dragons's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("SegDeath", false, false),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
}

void DragonWings::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    int deadcolor = dngutil::DARKGRAY;
    t->put(ColorString(R"(                                                 /===-_---~~~~~~~~~------____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                |===-~___                _,-')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 -==\\                         `//~\\   ~~~~`---.___.-~~     )", color), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(             ______-==|                         | |  \\           _-~`       )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(       __--~~~  ,-/-==\\                        | |   `\        ,'           )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    _-~       /'    |  \\                      / /      \      /             )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  .'        /       |   \\                   /' /        \   /'              )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(/-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                 )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  \_|      /        _)   ;  ),   __--~~                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    '~~--_/      _-~/-  / \   '-~ \                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   {\__--_/}    / \\_>- )<__\      \                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   /'   (_/  _-~  | |__>--<__|      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  |0  0 _/) )-~     | |__>--<__|      |                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  / /~ ,_/       / /__>---<__/      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 o o _//        /-~_>---<__-~      /                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 (^(~          /~_>---<__-      _-~                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              /__>--<__/     _-~                             )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|     /                  .----_        )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |                 /' _---_~\      )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |               /'  /     ~\`\    )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              \__>--<__\    \            /'  //        ||    )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                               ~-__>--<_~-_  ~--____---~' _/'/        /'     )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                  ~-_~>--<_/-__       __-~ _/                )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                     ~~-'_/_/ /~~~~~~~__--~                  )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                            ~~~~~~~~~~                       )", deadcolor), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 65;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}







DragonHead::DragonHead(
    Game* pgame,
    Coordinate coord,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl
) : SegEnemy(
    pgame,
    ColorChar('?', dngutil::WHITE),
    coord,
    "Dragon's Head",
    false,
    dngutil::TID::DragonHead,
    hp,
    att,
    def,
    lck,
    spd,
    lvl,
    new Primary(
        pgame,
        ColorChar('?', dngutil::LIGHTRED),
        coord,
        "Flame Blast",
        false,
        1.8,
        4,
        100,
        false,
        "Dragons flames.",
        WavFile("FireAttack1", false, false),
        dngutil::ClassType::KNIGHT
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        1.2,
        "Dragons's armor."
    ),
    Mp3File("DungeonBoss"),
    85,
    WavFile("FinalDeath", false, true),
    dngutil::EvType::DEFENSE,
    dngutil::ClassType::KNIGHT
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 2));
    setHp(getMaxhp());
}

void DragonHead::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::LIGHTRED;
    int deadcolor = dngutil::DARKGRAY;
    t->put(ColorString(R"(                                                 /===-_---~~~~~~~~~------____)", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                                |===-~___                _,-')", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 -==\\                         `//~\\   ~~~~`---.___.-~~     )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(             ______-==|                         | |  \\           _-~`       )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(       __--~~~  ,-/-==\\                        | |   `\        ,'           )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(    _-~       /'    |  \\                      / /      \      /             )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(  .'        /       |   \\                   /' /        \   /'              )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"( /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(/-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                 )", deadcolor), vcursor); vcursor.y++;
    const int TOP_CURSOR_Y = vcursor.y;
    t->put(ColorString(R"(                  \_|      /        _)   ;  ),   __--~~                      )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    '~~--_/      _-~/-  / \   '-~ \                          )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   {\__--_/}    / \\_>- )<__\      \                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   /'   (_/  _-~  | |__>--<__|      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  |0  0 _/) )-~     | |__>--<__|      |                      )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  / /~ ,_/       / /__>---<__/      |                        )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 o o _//        /-~_>---<__-~      /                         )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 (^(~          /~_>---<__-      _-~                          )", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              /__>--<__/     _-~                             )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|     /                  .----_        )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |                 /' _---_~\      )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                             |__>--<__|    |               /'  /     ~\`\    )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                              \__>--<__\    \            /'  //        ||    )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                               ~-__>--<_~-_  ~--____---~' _/'/        /'     )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                  ~-_~>--<_/-__       __-~ _/                )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                     ~~-'_/_/ /~~~~~~~__--~                  )", deadcolor), vcursor); vcursor.y++;
    t->put(ColorString(R"(                                            ~~~~~~~~~~                       )", deadcolor), vcursor); vcursor.y++;
    const int LONGEST_LINE_LENGTH = 65;

    printStats(LONGEST_LINE_LENGTH, TOP_CURSOR_Y);
}

//----------------------------------------------------------------


//----------------------------------------------------------------
// segboss

Segboss::Segboss(std::vector<SegEnemy*> segments, Game* game_pointer)
{
    this->segments = segments;
    this->pgame = game_pointer;
}

Segboss::~Segboss()
{
    for (auto it = segments.begin(); it != segments.end(); it++)
    {
        delete (*it);
    }
}

bool Segboss::segmentedBattle(Player* player)
{
    VirtualWindow* v = pgame->getVWin();
    TextMacros& t = v->txtmacs;

    Sleep(1000);

    v->txtmacs.clearDivider("bottom");

    Coordinate vcursor(0, t.BOTTOM_DIVIDER_TEXT_LINE);
    v->putcen(segments.front()->getBattleInfo(), vcursor.y++);
    pressEnter(vcursor, v);
    v->txtmacs.clearDivider("bottom");

    segments.front()->getBattleMusic().play();

    for (auto& i : segments)
    {
        if (!i->battle(i))
        {
            stopSound(SoundType::MP3);
            return false;
        }

        t.clearMapArea(false, NULL);
        t.clearDivider("bottom");
    }
    stopSound(SoundType::MP3);
    // If you are here you won
    Sleep(3500);

    segments.back()->deathSequence();
    playSound(WavFile("ExitToMap", false, true));
    return true;
}
//----------------------------------------------------------------