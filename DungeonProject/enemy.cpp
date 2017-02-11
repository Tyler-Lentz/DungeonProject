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
    getPGame()->adjustScore(dngutil::BASE_SCORE_BOSS_BOOST);

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
    ColorChar('T', dngutil::DARKGRAY),
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
        "A corpsed head, cursed by the skeleton king."
    ),
    "SkeletonKingTheme.mp3",
    50,
    "Screech.wav",
    dngutil::EvType::ATTACK
) {}

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
    "BeastTheme.mp3",
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
        2,
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
        2,
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

//----------------------------------------------------------------
// Seg Enemy

void SegEnemy::deathSequence()
{
    int levelDif = getLvl() - getPGame()->getPlayer()->getLvl();
    getPGame()->adjustScore(dngutil::BASE_SCORE_INCREASE_BATTLE + levelDif);
}

bool SegEnemy::battle(MapObject* t_enemy)
{
    Enemy* enemy = dynamic_cast<Enemy*>(t_enemy);
    Player* player = getPGame()->getPlayer();
    VirtualWindow* vwin = getPGame()->getVWin();

    if (enemy->getLvl() + 1 < player->getLvl())
    {
        enemy->increaseLvl(1);
        enemy->levelUpStats();
    }

    int enemyWeaponSpeed = enemy->getPrimary().getAttSpeed();
    int enemyTimer = 0;

    int playerWeaponSpeed = player->getPrimary().getAttSpeed();
    int playerTimer = 0;
    if (player->getPrimary().getStartReady())
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
                    Sleep(dngutil::HEALTHBAR_ADJUST_TIME);
                }

                vwin->putcen(
                    ColorString("-" + std::to_string(damage.damage), dngutil::GREEN) +
                    ((damage.damageDeflected > 0) ? ColorString(", " + std::to_string(damage.damageDeflected) + " DEFLECTED", dngutil::GREEN) : ColorString("", dngutil::BLACK))
                    , vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);  
                Sleep(300);

                if (enemy->isDead())
                {

                    soundEffect(enemy->getDeathSound(), false, true);

                    enemy->deathSequence();

                    getPGame()->getVWin()->txtmacs.clearMapArea(false, NULL);
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");

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
                    Sleep(dngutil::HEALTHBAR_ADJUST_TIME);
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
                        stopMp3();
                        getPGame()->cleanup(getPGame()->getVWin()->txtmacs.deathScreen(getPGame()));
                        return false;
                    }
                    else
                    {
                        player->setHp(0);
                        vwin->txtmacs.displayHealthBars(enemy, player);
                        vwin->txtmacs.outputBattleInfo(playerTimer, playerWeaponSpeed, enemyTimer, enemyWeaponSpeed);

                        soundEffect("MagicalPotion.wav", false, false);
                        soundEffect("RefillHealth.wav", true, true);
                        for (int i = 0; i < (player->getMaxhp() * .75); i++)
                        {
                            player->increaseHealth(1);
                            vwin->txtmacs.displayHealthBars(enemy, player);
                            Sleep(40);
                        }
                        stopSound();
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
        dngutil::TID::Primary,
        2.2,
        6,
        70,
        false,
        "Dragons Tail.",
        "Attack4.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        .25,
        "Dragons's armor."
    ),
    "FinalBoss.mp3",
    85,
    "SegDeath.wav",
    dngutil::EvType::DEFENSE
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.5));
    setHp(getMaxhp());
    increaseDef(static_cast<unsigned int>(getDef() * .75));
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
        dngutil::TID::Primary,
        1.4,
        3,
        100,
        false,
        "Dragons Wings.",
        "Attack1.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Wing Armor",
        false,
        dngutil::TID::Secondary,
        0,
        .7,
        "Dragons's armor."
    ),
    "FinalBoss.mp3",
    85,
    "SegDeath.wav",
    dngutil::EvType::DEFENSE
)
{
    setMaxhp(static_cast<unsigned int>(getMaxhp() * 1.2));
    setHp(getMaxhp());
    increaseDef(static_cast<unsigned int>(getDef() * .5));
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
        dngutil::TID::Primary,
        1.8,
        4,
        100,
        false,
        "Dragons flames.",
        "FireAttack1.wav"
    ),
    new Secondary(
        pgame,
        ColorChar('?', dngutil::GREEN),
        coord,
        "Scale Armor",
        false,
        dngutil::TID::Secondary,
        0,
        .4,
        "Dragons's armor."
    ),
    "FinalBoss.mp3",
    85,
    "FinalDeath.wav",
    dngutil::EvType::DEFENSE
)
{
    setMaxhp(dngutil::MAX_HP);
    setHp(getMaxhp());
    increaseDef(static_cast<unsigned int>(getDef() * .6));
    increaseAtt(static_cast<unsigned int>(getAtt() * .1));
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
    startMp3(segments.front()->getBattleMusic());

    for (auto& i : segments)
    {
        if (!i->battle(i))
        {
            stopMp3();
            return false;
        }

        t.clearMapArea(false, NULL);
        t.clearDivider("bottom");
    }
    stopMp3();
    // If you are here you won
    // if this was going to be a bigger game this would not be part of this function
    // but it is because this will not be used for anything but the final boss

    ColorString line(std::string(dngutil::CONSOLEX, ' '), getColor(dngutil::WHITE, dngutil::WHITE));

    for (int i = 0; i < dngutil::CONSOLEY; i++)
    {
        v->putcen(line, i);
    }
    Sleep(4000);
    startMp3("Win.mp3");
    Sleep(5000);
    stopMp3();

    Sleep(100);
    startMp3("Ending.mp3");

    pgame->adjustScore(dngutil::BASE_SCORE_VICTORY);

    int color = getColor(dngutil::BLACK, dngutil::WHITE);
    Coordinate vcursor(0, 5);
    v->putcen(ColorString("DUNGEON RPG - DRAGON'S LAIR", color), vcursor.y++);
    vcursor.y += 4;

    v->putcen(ColorString("Programming: Tyler Lentz", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Story: Tyler Lentz and Thomas Westenhofer", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Play Testing: Tyler Lentz, Thomas Westenhoffer, Kristian Rascon,", color), vcursor.y++);
    v->putcen(ColorString("Joshua Chan, Daniel Hernandez, Zach Fineberg, Chris Mosely and others", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Special Thanks to: Evan \"Little Fella\" Maich,", color), vcursor.y++);
    v->putcen(ColorString("Niko \"Mile Stretch Mile\" Fernandez and", color), vcursor.y++);
    v->putcen(ColorString("Anthony \"Duganator 3000\" Dugan", color), vcursor.y++);
    vcursor.y += 5;
    v->putcen(
        ColorString("Your score was: ", color) + 
        ColorString(std::to_string(pgame->getScore()), getColor(dngutil::YELLOW, dngutil::WHITE)),
        vcursor.y++
    );
    vcursor.y += 2;
    v->putcen(ColorString("Your adventure is over", color), vcursor.y++);
    vcursor.y += 3;
    pressEnter(vcursor, v, color);

    v->clearScreen();
    stopMp3();

    pgame->cleanup(dngutil::ReturnVal::RESTART);
    return true;
}
//----------------------------------------------------------------