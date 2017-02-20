#include "game.h"
#include "virtualwindow.h"
#include "player.h"
#include "utilities.h"
#include "enemy.h"
#include "room.h"
#include "item.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>

//-------------------------------------------------------
// Game Functions

dngutil::ReturnVal Game::run()
{
    if (!exit)
    {
        vwin->txtmacs.displayGame(this);
        startMp3(dungeon->overworldMusic);
    }

    while (!exit)
    {
        activeRoom->drawRoom();

        for (auto& i : activeRoom->getCreatureList())
        {
            // Returns true if player goes to new room/floor
            if (i->movement())
            {
                break;
            }
        }

        activeRoom->checkPuzzle();
    }

    return returnVal;
}

Game::Game(VirtualWindow* vwin)
    :difficulty()
{
    this->vwin = vwin;
    player = new Player(this, Coordinate(-1, -1));

    activeRoom = nullptr;
    floor = 0;

    spawnBeast = false;
    score = 0;

    titleScreen();
}

Game::~Game()
{
    delete dungeon;

    delete player;
}

Room* Game::getActiveRoom()
{
    return activeRoom;
}

Player* Game::getPlayer()
{
    return player;
}

VirtualWindow* Game::getVWin()
{
    return vwin;
}

void Game::setExitToFalse()
{
    this->exit = false;
}

bool Game::shouldExit()
{
    return exit;
}

std::string Game::getOverworldMusic() const
{
    return dungeon->overworldMusic;
}

bool Game::shouldSpawnBeast()
{
    return spawnBeast;
}

void Game::setActiveFloor(unsigned int floor)
{
    this->floor = floor;
}

void Game::setActiveRoom(Room* room)
{
    if (activeRoom != nullptr)
    {
        activeRoom->getCreatureList().remove(player);
    }
    activeRoom = room;
    room->getCreatureList().push_back(player);
}

std::map<Coordinate, Room*>& Game::getActiveFloor()
{
    return dungeon->gamespace[floor];
}

std::list<MapObject*>& Game::getDeletionList()
{
    return dungeon->deletionList;
}

void Game::clearDeletionList()
{
    for (auto& i : dungeon->deletionList)
    {
        delete i;
    }

    dungeon->deletionList.clear();
}

Creature* Game::generateCreature(int difficulty, dngutil::TID tid)
{
    int level = random(-1, 1) + difficulty;
    if (level < 1)
    {
        level = 1;
    }

    int health = dngutil::STARTING_ENEMY_HP + random(level - 5, level + 5); // 0
    int attack = 5;
    int defense = 2;
    int luck = 5;
    int speed = 10;

    Enemy* enemy = nullptr;
    switch (tid)
    {
    case dngutil::TID::Skeleton:
        enemy = new Skeleton(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::SkeletonKing:
        enemy = new SkeletonKing(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::BloodSkeleton:
        enemy = new BloodSkeleton(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::LSKnight:
        enemy = new LSKnight(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::SSKnight:
        enemy = new SSKnight(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::Mage:
        enemy = new Mage(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::Bowman:
        enemy = new Bowman(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::DungeonBeast:
        enemy = new DungeonBeast(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::DragonHead:
        enemy = new DragonHead(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::DragonTail:
        enemy = new DragonTail(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::DragonWings:
        enemy = new DragonWings(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::MegaBeastPhase1:
        enemy = new MegaBeastPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::MegaBeastPhase2:
        enemy = new MegaBeastPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::FlameHorse:
        enemy = new FlameHorse(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::GryphonPhase1:
        enemy = new GryphonPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::GryphonPhase2:
        enemy = new GryphonPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::GryphonPhase3:
        enemy = new GryphonPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::ReaperPhase1:
        enemy = new ReaperPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::ReaperPhase2:
        enemy = new ReaperPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::ReaperPhase3:
        enemy = new ReaperPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::MaskPhase1:
        enemy = new MaskPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::MaskPhase2:
        enemy = new MaskPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::MaskPhase3:
        enemy = new MaskPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;
    }

    if (enemy == nullptr)
    {
        errorMessage("Enemy is a nullptr. TID is " + std::to_string(static_cast<int>(tid)), __LINE__, __FILE__);
    }

    for (unsigned int i = 0; i < enemy->getLvl(); i++)
    {
        enemy->levelUpStats();
    }

    return enemy;
}

void Game::cleanup(dngutil::ReturnVal returnval)
{
    exit = true;
    this->returnVal = returnval;
}

void Game::titleScreen()
{
    startMp3("TitleTheme.mp3");

    vwin->txtmacs.drawDividers();
    vwin->txtmacs.clearDivider("bottom");
    vwin->putcen(ColorString("Dungeon RPG 1.0 (INDEV VERSION)", dngutil::RED), vwin->txtmacs.DIVIDER_LINES[0] + 1);
    vwin->putcen(ColorString("Enter - Continue, C - credits, Esc - exit", dngutil::LIGHTGRAY), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    int r = random(3);
    if (r == 0)
    {
        Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 5);
        VirtualWindow* t = vwin;
        int skeletoncolor = dngutil::WHITE;
        int weaponcolor = dngutil::BROWN;
        t->putcen(ColorString("                              _.--\"\"-._                     \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("  .", weaponcolor) + ColorString("                        .\"         \".                   \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString(" / \\    ,^.", weaponcolor) + ColorString("         /(     Y             |      )\\          \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("/   `---. |--\'\\", weaponcolor) + ColorString("    (  \\__..\'--   -   -- -\'\"\"-.-\'  )          \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("|        :|    `> ", weaponcolor) + ColorString("  \'.     l_..-------.._l      .\'          \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("|      __l;__ .\'", weaponcolor) + ColorString("      \"-.__.||_.-\'v\'-._||`\"----\"            \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString(" \\  .-\' | |  `", weaponcolor) + ColorString("              l._       _.\'                   \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("  \\/    | |   ", weaponcolor) + ColorString("                l`^^\'^^\'j                     \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        | |  ", weaponcolor) + ColorString("              _   \\_____/     _                \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        j |  ", weaponcolor) + ColorString("             l `--__)-\'(__.--\' |               \n", skeletoncolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        | |    ", weaponcolor) + ColorString("           | /`---``-----\'\"1 |", skeletoncolor) + ColorString("  ,-----.      \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        | |    ", weaponcolor) + ColorString("           )/  `--\' \'---\' ", skeletoncolor) + ColorString("   /'-\'  ___  `-.   \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        | |     ", weaponcolor) + ColorString("         //  `-\'  \'`----\'", skeletoncolor) + ColorString("  /  ,-\'   I`.  \\  \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("      _ L |_     ", weaponcolor) + ColorString("       //  `-.-.\'`-----\' ", skeletoncolor) + ColorString("/  /  |   |  `. \\ \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("     \'._\' / \\         _/(   '-  -'- ---\' ", skeletoncolor) + ColorString(";  /__.J   L.__.\\ :\n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("      `._;/7(-.......\'  /        ) (  ", skeletoncolor) + ColorString("   |  |            | |\n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("      `._;l _\'--------_/        )-\'/  ", skeletoncolor) + ColorString("   :  |___.    _._./ ;\n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        | |        ", weaponcolor) + ColorString("         .__ )-\'\\  __ ", skeletoncolor) + ColorString(" \\  \\  I   1   / / \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("        `-\'    ", weaponcolor) + ColorString("            /   `-\\-(-\'   \\", skeletoncolor) + ColorString(" \\  `.|   | ,\' /  \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("                           \\__  `-\'    __/ ", skeletoncolor) + ColorString("  `-. `---\'\',-\'   \n", weaponcolor), vcursor.y); vcursor.y++;
        t->putcen(ColorString("                              )-._.-- (  ", skeletoncolor) + ColorString("      `-----\'      \n", weaponcolor), vcursor.y); vcursor.y++;

    }
    else if (r == 1)
    {
        Coordinate vcursor(15, vwin->txtmacs.DIVIDER_LINES[1] + 1);
        VirtualWindow* t = vwin;
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
    }
    else if (r == 2)
    {
        Coordinate vcursor(15, vwin->txtmacs.DIVIDER_LINES[1] + 1);
        VirtualWindow* t = vwin;
        int color = dngutil::MAGENTA;
        t->put(ColorString(R"(               )", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                )", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                   ,'`.)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(                  ;\  /:)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(              .  /  \/  \)", color), vcursor); vcursor.y++;
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
    }
    else
    {
        Coordinate vcursor(15, vwin->txtmacs.DIVIDER_LINES[1] + 1);
        VirtualWindow* t = vwin;
        int color = dngutil::MAGENTA;
        t->put(ColorString(R"(              .)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             /.\)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.|)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.|   ,'`.)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.|  ;\  /:)", color), vcursor); vcursor.y++;
        t->put(ColorString(R"(             |.| /  \/  \)", color), vcursor); vcursor.y++;
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
    }

    while (true)
    {
        if (keypress(VK_RETURN))
        {

            vwin->txtmacs.clearMapArea(false, NULL);
            vwin->txtmacs.clearDivider("bottom");


            Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 10);
            vwin->putcen(ColorString("Dungeon Selection", dngutil::WHITE), vcursor.y++);
            vcursor.y += 5;
            vwin->putcen(ColorString("(1) - Dragon's Lair", dngutil::LIGHTRED), vcursor.y++);
            vcursor.y++;
            vwin->putcen(ColorString("(2) - Gryphon's Tower", dngutil::MAGENTA), vcursor.y++);
            vcursor.y++;
            vwin->putcen(ColorString("(3) - Pit of 50 Trials", dngutil::WHITE), vcursor.y++);

            while (true)
            {
                if (keypress('1'))
                {
                    dungeon = new DragonsLair(this);
                    break;
                }
                else if (keypress('2'))
                {
                    dungeon = new GryphonsTower(this);
                    break;
                }
                else if (keypress('3'))
                {
                    dungeon = new PitOf50Trials(this);
                    break;
                }
            }

            vwin->txtmacs.clearMapArea(false, NULL);
            vcursor = Coordinate(0, vwin->txtmacs.DIVIDER_LINES[1] + 10);
            vwin->putcen(ColorString("Difficulty Selection", dngutil::WHITE), vcursor.y++);
            vcursor.y += 5;
            vwin->putcen(ColorString("(1) - Easy", dngutil::LIGHTMAGENTA), vcursor.y++);
            vcursor.y++;
            vwin->putcen(ColorString("(2) - Classic", dngutil::GREEN), vcursor.y++);
            vcursor.y++;
            vwin->putcen(ColorString("(3) - Hardcore", dngutil::LIGHTRED), vcursor.y);

            Sleep(200);

            while (true)
            {
                if (keypress('1'))
                {
                    // needs to adjust the difficulty values
                    difficulty.beastSteps = 275;
                    difficulty.canFightMegabeast = false;
                    difficulty.color = dngutil::LIGHTMAGENTA;
                    difficulty.deflectModifier = 1.75;
                    difficulty.damageMultiplier = 1.1;
                    difficulty.healthIncreaseBoost = 1;
                    break;
                }
                else if (keypress('2'))
                {
                    // difficulty is already set to what it needs to be
                    break;
                }
                else if (keypress('3'))
                {
                    // needs to adjust the difficulty values
                    difficulty.beastSteps = 150;
                    difficulty.canFightMegabeast = true;
                    difficulty.color = dngutil::LIGHTRED;
                    difficulty.deflectModifier = 0.65;
                    difficulty.damageMultiplier = .9;
                    difficulty.healthIncreaseBoost = 0;
                    break;
                }
            }
            getPlayer()->changeMapChar(ColorChar(getPlayer()->getMapRep().character, difficulty.color));
            getPlayer()->adjustSecondaryDeflectTime(difficulty.deflectModifier);

            vwin->txtmacs.clearMapArea(false, NULL);
            vcursor.y = vwin->txtmacs.DIVIDER_LINES[1] + 5;
            for (auto& i : dungeon->story)
            {
                vwin->putcen(i.first, vcursor.y);
                vcursor.y += i.second + 1;
            }
            pressEnter(vcursor, vwin);
            break;
        }
        else if (keypress(VK_ESCAPE))
        {
            cleanup(dngutil::ReturnVal::EXIT);
            break;
        }
        else if (keypress('C'))
        {
            vwin->txtmacs.clearDivider("bottom");
            credits(dngutil::CreditType::TITLESCREEN, this, dngutil::DungeonType::NEITHER);
            break;
        }
    }

    
    stopMp3();
}

const Difficulty& Game::getDifficulty()
{
    return difficulty;
}

void Game::setBeastSpawn(bool spawn)
{
    spawnBeast = spawn;
}

int Game::getBeastSteps()
{
    return stepsToBeast;
}

void Game::adjustScore(int adjust)
{
    score += adjust;
}

int Game::getScore()
{
    return score;
}
//-------------------------------------------------------