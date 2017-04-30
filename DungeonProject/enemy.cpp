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
            getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
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
    ColorChar('T', dngutil::RED),
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
        140,
        1.12,
        "A shield from a blood skeleton"
    ),
    Mp3File("BattleTheme", "BattleThemeAlt"),
    random(31, 40),
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
    ColorChar('A', dngutil::DARKGRAY),
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
    return ColorString("\"This is where your miserable little life ends\"", dngutil::MAGENTA);
}

void TrueZorlock::printSelf()
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

void TrueZorlock::beginingCutscene()
{
    Game* g = getPGame();
    VirtualWindow* v = g->getVWin();
    TextMacros& t = v->txtmacs;
    Coordinate vcursor(10, t.DIVIDER_LINES[1] + 1);

    t.clearMapArea(true, 10);
    t.clearDivider("bottom");

    v->put(ColorString(R"(                         /\     /\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                        (_ \   (  \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                        (_ `\iRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                         \_  \VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                           \o_)lI<= (=\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                          / `   ) , _\ ))", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                          )    /   (_(/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                         /_ __( \ __ _|)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                            _|     |_)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                   ,edMMNBRB)\ \_  /(PRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                  dMMNBRPAV/o(_____)o\VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                 fMMWBRYIli\ o  o  o /rlIPRBWMMj)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                 MMBRRSZIlti+._.-._.+ilIZSRRBMMM)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                 /_o__o_/KZYI0lLRBNWMMMN\_o___o_\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                /_____/flZJlDYTPRKBBNWMM \ ___( \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(          NNBR0PRRBNMMMMMMMlIPZSKRNWMMMV  \__._\ \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(          NMBRR00PRBNNMMMMM_o___o___o__/   )_ __)o))", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(          NMMBRR00PRRBNNMMM___o___o___/    /___/ /)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(          NMMBBRR00PRRBNNMM_/\_/\_/\_(  __/___x)/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(          NMMBBRR00PRRBNNMM \/ \/ \/ __ )    /(`)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(        __NMMBBRR00PRRBNNMM_________|_ /   _'))", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(       /__        ________        ___)(<  (====.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(       \_____________________________)_\ __\---')", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(            VMBBR00RBNMNV           |__|\/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(             lMBRR00RNMV  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(              VMB00BMNV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(               VMB0BMV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                b'ger  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                 VMV     )", dngutil::DARKGRAY), vcursor); vcursor.y++;
    v->put(ColorString(R"(                  V     )", dngutil::DARKGRAY), vcursor); vcursor.y++;

    int l = t.BOTTOM_DIVIDER_TEXT_LINE;
    v->putcen(ColorString("You slayed Zorlock", dngutil::WHITE), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcen(ColorString("...", dngutil::WHITE), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcen(ColorString("You fool...", dngutil::MAGENTA), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    playSound(WavFile("ZorlockTransform", true, true));
    for (int i = 35; i > 0; i--)
    {
        t.clearMapArea(false, NULL);
        vcursor.y = t.DIVIDER_LINES[1] + 1;
        if (i % 2 == 0)
        {
            vcursor.x = 10;
            v->put(ColorString(R"(                         /\     /\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                        (_ \   (  \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                        (_ `\iRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                         \_  \VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                           \o_)lI<= (=\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                          / `   ) , _\ ))", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                          )    /   (_(/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                         /_ __( \ __ _|)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                            _|     |_)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                   ,edMMNBRB)\ \_  /(PRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                  dMMNBRPAV/o(_____)o\VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                 fMMWBRYIli\ o  o  o /rlIPRBWMMj)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                 MMBRRSZIlti+._.-._.+ilIZSRRBMMM)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                 /_o__o_/KZYI0lLRBNWMMMN\_o___o_\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                /_____/flZJlDYTPRKBBNWMM \ ___( \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(          NNBR0PRRBNMMMMMMMlIPZSKRNWMMMV  \__._\ \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(          NMBRR00PRBNNMMMMM_o___o___o__/   )_ __)o))", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(          NMMBRR00PRRBNNMMM___o___o___/    /___/ /)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(          NMMBBRR00PRRBNNMM_/\_/\_/\_(  __/___x)/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(          NMMBBRR00PRRBNNMM \/ \/ \/ __ )    /(`)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(        __NMMBBRR00PRRBNNMM_________|_ /   _'))", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       /__        ________        ___)(<  (====.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(       \_____________________________)_\ __\---')", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(            VMBBR00RBNMNV           |__|\/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(             lMBRR00RNMV  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(              VMB00BMNV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(               VMB0BMV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                b'ger  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                 VMV     )", dngutil::DARKGRAY), vcursor); vcursor.y++;
            v->put(ColorString(R"(                  V     )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        }
        else
        {
            vcursor.x = 3;
            v->put(ColorString(R"(           _/          ,          .                                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(       /,'    /  /      '    /                                               )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(       \|    /  (          ,'                                                )", dngutil::MAGENTA), vcursor); vcursor.y++;
            v->put(ColorString(R"(       ('  ,'    `.    "  /                                                  )", dngutil::MAGENTA), vcursor); vcursor.y++;
        }
        Sleep(35);
    }
    stopSound(SoundType::WAV);

    v->putcen(ColorString("- True Zorlock - ", dngutil::MAGENTA), l, true);
    playSound(WavFile("ZorlockAppears", false, false));
    t.clearLine(l);

    v->putcen(ColorString("I was Zorlock", dngutil::MAGENTA), l, true);
    pressEnter(Coordinate(0, l + 1), v);
    t.clearLine(l);
    t.clearLine(l + 1);

    v->putcen(ColorString("When Aegeus, your \"so-called hero,\" sealed me away.", dngutil::MAGENTA), l, true);
    v->putcen(ColorString("his soul was transfered to my body, and mine was transfered to his.", dngutil::MAGENTA), l + 1, true);
    pressEnter(Coordinate(0, l + 2), v);
    t.clearLine(l);
    t.clearLine(l + 1);
    t.clearLine(l + 2);

    v->putcen(ColorString("Now that you have killed Aegeus I am free to regain my true form and", dngutil::MAGENTA), l, true);
    v->putcen(ColorString("destroy the entire world", dngutil::RED), l + 1, true);
    pressEnter(Coordinate(0, l + 2), v);
    t.clearLine(l);
    t.clearLine(l + 1);
    t.clearLine(l + 2);

    playSound(WavFile("EnterBattle", false, true));
    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");
}

void TrueZorlock::deathSequence()
{
    VirtualWindow* v = getPGame()->getVWin();
    TextMacros& t = v->txtmacs;
    v->clearScreen();
    Sleep(2500);
    int textLine = 22;

    std::string name = getPGame()->getPlayer()->getName();
    v->putcen(ColorString("Curse you " + name + "!", dngutil::MAGENTA), textLine, true);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcen(ColorString("Curse you Aegeus!", dngutil::MAGENTA), textLine, true);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcen(ColorString("Do not think this ends here!", dngutil::MAGENTA), textLine, true);
    Sleep(2500);
    t.clearLine(textLine);

    v->putcen(ColorString("When I break free of this seal the Kingom of Bora", dngutil::MAGENTA), textLine, true);
    v->putcen(ColorString("will be destroyed again - Mark my words!", dngutil::MAGENTA), textLine + 1, true);
    Sleep(3500);
    t.clearLine(textLine);
    t.clearLine(textLine + 1);

    v->putcen(ColorString("And I'll reserve a special place in hell for both of you and your descendents!", dngutil::MAGENTA), textLine, true);
    Sleep(2500);
    t.clearLine(textLine);

    playSound(WavFile("ZorlockDeath", false, false));

    v->putcen(ColorString("Peace has been restored to Bora", dngutil::WHITE), textLine, true);
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
        2,
        65,
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
    t->put(ColorString(R"(      \\\\  `-------'         jgs /________.')", color), vcursor); vcursor.y++;
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
    ColorChar('A', dngutil::BROWN),
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

            if (playerTimer >= playerWeaponSpeed && keypress(VK_RETURN))
            {
                playerTimer = 0;

                Damage damage = player->getDamageDealt(enemy);
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
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.4));
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
    setMaxhp(dngutil::MAX_HP - 50);
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