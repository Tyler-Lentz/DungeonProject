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
    std::string battleMusic,
    unsigned int experienceGiven,
    std::string deathSound,
    dngutil::EvType ev
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
    dngutil::P_ENEMY
)
{
    this->battleMusic = battleMusic;
    this->deathSound = deathSound;
    this->experienceGiven = experienceGiven;
    this->ev = ev;
}

const std::string& Enemy::getBattleMusic() const
{
    return battleMusic;
}

const std::string& Enemy::getDeathSound() const
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

        soundEffect("FindItem.wav", false, false);

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
        stopMp3();

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        soundEffect("EnterBattle.wav", false, true);
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
    if (!(getLastMoveTime() + ((dngutil::MAX_SPD * 3) - getSpd()) > GetTickCount()))
    {
        switch (random(100))
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
    startMp3("DefeatBoss.mp3");

    getPGame()->getVWin()->txtmacs.clearMapArea(true, 100);
    getPGame()->getVWin()->txtmacs.clearDivider("bottom");

    // Length of the sound effect adjusted for the screen clearing
    Sleep(5000);

    stopMp3();

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
        dngutil::TID::Primary,
        1.5,
        5,
        70,
        false,
        "A heavy wooden axe that was dropped from a Skeleton",
        "Attack4.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::BROWN),
        coord,
        "Wooden Shield",
        false,
        dngutil::TID::Secondary,
        50,
        .85,
        "A heavy wooden shield that was dropped from a Skeleton"
    ),
    "BattleTheme.mp3",
    random(23, 29),
    "EnemyDeath.wav",
    dngutil::EvType::HEALTH
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
) : REnemy(
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
        dngutil::TID::Primary,
        1.7,
        4,
        70,
        false,
        "An axe from a blood skeleton",
        "Attack4.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::WHITE),
        coord,
        "Stained Shield",
        false,
        dngutil::TID::Secondary,
        40,
        .80,
        "A shield from a blood skeleton"
    ),
    "BattleTheme.mp3",
    random(31, 40),
    "EnemyDeath.wav",
    dngutil::EvType::HEALTH
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
// Large Skeleton Functions

LargeSkeleton::LargeSkeleton(
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
    ColorChar('T', dngutil::DARKGRAY),
    coord,
    "Large Skeleton",
    false,
    dngutil::TID::LargeSkeleton,
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
        dngutil::TID::Primary,
        1.5,
        4,
        95,
        false,
        "A very long sword. It is very accurate.",
        "Attack4.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('A', dngutil::GREEN),
        coord,
        "Mutilated Head",
        false,
        dngutil::TID::Secondary,
        0,
        .7,
        "A corpsed head, cursed by a large skeleton."
    ),
    "MinibossTheme.mp3",
    50,
    "MinibossDeath.wav",
    dngutil::EvType::ATTACK
) {}

void LargeSkeleton::printSelf()
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
// Large Skeleton Functions

DungeonBeast::DungeonBeast(
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
    ColorChar('S', dngutil::WHITE),
    coord,
    "Dungeon Beast",
    false,
    dngutil::TID::DungeonBeast,
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
        "Magical Beam",
        false,
        dngutil::TID::Primary,
        2.5,
        6,
        100,
        false,
        "A magical beam.",
        "MagicAttack1.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Beast Armor",
        false,
        dngutil::TID::Secondary,
        0,
        .25,
        "Beast's armor."
    ),
    "MinibossTheme.mp3",
    85,
    "KillDemon.wav",
    dngutil::EvType::DEFENSE
) 
{
    setMaxhp(getMaxhp() * 2);
    setHp(getMaxhp());
}

void DungeonBeast::printSelf()
{
    Coordinate vcursor(0, getPGame()->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
    VirtualWindow* t = getPGame()->getVWin();
    int color = dngutil::WHITE;
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
) : REnemy(
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
        dngutil::TID::Primary,
        2.5,
        7,
        85,
        false,
        "A broadsword from a cursed knight",
        "Attack4.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::MAGENTA),
        coord,
        "Cursed Shield",
        false,
        dngutil::TID::Secondary,
        40,
        .80,
        "A shield from a cursed knight"
    ),
    "BattleTheme.mp3",
    random(26, 33),
    "EnemyDeath.wav",
    dngutil::EvType::DEFENSE
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
) : REnemy(
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
        dngutil::TID::Primary,
        1.5,
        2,
        85,
        false,
        "A shortsword from a cursed knight",
        "Attack2.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('0', dngutil::MAGENTA),
        coord,
        "Cursed Shield",
        false,
        dngutil::TID::Secondary,
        40,
        .80,
        "A shield from a cursed knight"
    ),
    "BattleTheme.mp3",
    random(26, 33),
    "EnemyDeath.wav",
    dngutil::EvType::SPEED
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
) : REnemy(
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
        dngutil::TID::Primary,
        2.3,
        4,
        85,
        false,
        "A Mage's staff - spews electricity",
        "MagicAttack1.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('*', dngutil::YELLOW),
        coord,
        "Magic Aura",
        false,
        dngutil::TID::Secondary,
        0,
        .67,
        "A protecting aura from a mage"
    ),
    "BattleTheme.mp3",
    random(35, 40),
    "EnemyDeath.wav",
    dngutil::EvType::ATTACK
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