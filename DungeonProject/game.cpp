#include "game.h"
#include "virtualwindow.h"
#include "player.h"
#include "utilities.h"
#include "enemy.h"
#include "room.h"
#include "item.h"
#include "soundfile.h"
#include "map.h"
#include "savefile.h"

#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <vector>


#undef max

//-------------------------------------------------------
// Game Functions

dngutil::ReturnVal Game::run()
{
    if (!exit)
    {
        vwin->txtmacs.displayGame(this);
        overworldMusic.play();
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
    player = new Player(this, Coordinate(-1, -1), "Link");

    overworldMusic = Mp3File("OverworldTheme");

    activeRoom = nullptr;
    floor = 0;

    spawnBeast = false;
    score = 0;

    map = new Map(this);

    dungeonLevel = 1;

    //must happen after making new map & player
    titleScreen();
    overworldMusic = activeRoom->getMusic();
}

Game::~Game()
{
    delete map;

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

Mp3File Game::getOverworldMusic() const
{
    return overworldMusic;
}

void Game::setOverworldMusic(Mp3File mp3)
{
    overworldMusic = mp3;
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

void Game::setActiveRoom(Coordinate roomCoord)
{
    activeRoom->getCreatureList().remove(player);
    activeRoom = getActiveFloor()[roomCoord];
    activeRoom->getCreatureList().push_back(player);
}

void Game::setActiveRoomFromLoadSave(Coordinate roomCoord)
{
    activeRoom->getCreatureList().remove(player);
    auto it = activeRoom->getObjects(player->getCoord()).back();// this is guarenteed to be where player will be if game has just started
    activeRoom->getObjects(player->getCoord()).remove(it);
    activeRoom = getActiveFloor()[roomCoord];
    activeRoom->getCreatureList().push_back(player);
}

std::map<Coordinate, Room*>& Game::getActiveFloor()
{
    return map->gamespace[floor];
}

int Game::getRawFloor() const
{
    return floor;
}

std::list<MapObject*>& Game::getDeletionList()
{
    return map->deletionList;
}

void Game::clearDeletionList()
{
    for (auto& i : map->deletionList)
    {
        delete i;
    }

    map->deletionList.clear();
}

void Game::setDungeonLevel(int number)
{
    dungeonLevel = number;
}

int Game::getDungeonLevel() const
{
    return dungeonLevel;
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
        enemy = new SkeletonKing(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
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

    case dngutil::TID::DragonHead:
        enemy = new DragonHead(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::DragonTail:
        enemy = new DragonTail(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::DragonWings:
        enemy = new DragonWings(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::MegaBeastPhase1:
        enemy = new MegaBeastPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::MegaBeastPhase2:
        enemy = new MegaBeastPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::FlameHorse:
        enemy = new FlameHorse(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::GhostHorse:
        enemy = new GhostHorse(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::ForestDragon:
        enemy = new ForestDragon(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::WaterHorse:
        enemy = new WaterHorse(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::BloodJawPhase1:
        enemy = new BloodjawPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::BloodJawPhase2:
        enemy = new BloodjawPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::BloodJawPhase3:
        enemy = new BloodjawPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::EvilBeastPhase1:
        enemy = new EvilBeastPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::EvilBeastPhase2:
        enemy = new EvilBeastPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::EvilBeastPhase3:
        enemy = new EvilBeastPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::EvilBeastPhase4:
        enemy = new EvilBeastPhase4(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
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
    playSound(Mp3File("TitleTheme"));

    vwin->txtmacs.drawDividers();
    vwin->txtmacs.clearDivider("bottom");
    vwin->putcen(ColorString("The Harp of the Gods", dngutil::GREEN), vwin->txtmacs.DIVIDER_LINES[0] + 1);
    vwin->putcen(ColorString("1 - New Game, 2 - Load \"save.dat\", C - credits, Esc - exit", dngutil::LIGHTGRAY), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

    Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 5);
    VirtualWindow* t = vwin;
    t->putcen(ColorString(R"(         ____                   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         SSSS____.              )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         (WW);;;;;\             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         `WW'____ |     ,_____  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||\ \___/,---. ) )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||\____/||| //  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||||||||" //   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||||||' //    )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||||"  //     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||||'  //      )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||"  //       )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||'  //        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||"   //         )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||"   //          )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||'   //           )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |"   //            )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         ,UU,'   ||             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(       (~~~~~~~~~~~~]""'        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        

    while (true)
    {
        if (keypress('1'))
        {
            vwin->txtmacs.clearMapArea(false, NULL);
            vwin->txtmacs.clearDivider("bottom");

            vwin->putcen(ColorString("Enter - select    Shift+Enter - select and skip story", dngutil::WHITE), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

            vwin->put(ColorString("Enter your name: ", dngutil::WHITE), Coordinate(15, 20));
            std::string name;
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            std::cin >> name;
            std::cin.ignore();
            if (name.size() > 8)
            {
                name.resize(8);
            }

            vwin->getConsole().setTitle(name);

            player->setName(name);

            if (keypress(VK_SHIFT))
            {
                vwin->clearScreen();
            }
            else
            {
                backgroundStory();
            }

            break;
        }
        else if (keypress('2'))
        {
            if (loadGame(this))
            {
                vwin->txtmacs.clearMapArea(false, NULL);
                vwin->txtmacs.clearDivider("bottom");
                break;
            }
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

    
    stopSound(SoundType::MP3);
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

void Game::backgroundStory()
{
    VirtualWindow* t = vwin;
    TextMacros* tm = &vwin->txtmacs;

    t->clearScreen();

    stopSound(SoundType::MP3);
    playSound(Mp3File("StoryMusic"));

    const int onelineTime = 2000;
    const int tlx = 3;
    const int tly = 1;
    int color = dngutil::WHITE;
    Coordinate vcursor(tlx, tly);
    t->put(ColorString(R"(                  [\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  |\)                                ____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  |                               __(_   )__)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  Y\          ___               _(          ))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 T  \       __)  )--.          (     )-----`)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(                J    \   ,-(         )_         `---')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               Y/T`-._\ (     (       _)                 __)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               /[|   ]|  `-(__  ___)-`  |\          ,-(  __))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(               | |    |      (__)       J'         (     ))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   _           | |  ] |    _           /;\          `-  ')", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  (,,)        [| |    |    L'         /;  \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(             /||.| /\ |   /\         /.,-._\        ___ _)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(            /_|||| || |  /  \        | |{  |       (._.'_))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  L/\       | \| | '` |_ _ {|        | | U |   /\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( /v^v\/\   `|  Y | [  '-' '--''-''-"-'`'   | ,`^v\ /\,`\)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(/ ,'./  \.` |[   |       [     __   L    ] |      /^v\  \)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(,'     `    |    |           ,`##Y.   ]    |___Y Y____,_,,_,,_)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(--   -----.-(] [ |   ]     o/####U|o      ]|| /`-, Y   _   Y  Y)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   Y Y  --;`~T   |      }   \####U|[\ _,.-(^) ,-'  _  (^)__  _)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(  Y  YY   ;'~~l  |   L     [|\###U'E'\  \ \Y-` _  (^) _Y  _)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"( Y  Y Y   ;\~~/\{| [      _,'-\`= = '.\_ ,`   (^)(^) (^) (^))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(     --   ;\~~~/\|  _,.-'`_  `.\_..-'"  _ . ,_ Y_ Y_ _Y  _Y__)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    _    _; \~~( Y``   Y (^) / `,      (^)      _   (^) (^))", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(   (^)  (^)`._~ /  L \  _.Y'`  _  ` --  Y - - -(^) - Y - Y -)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(    Y    Y    `'--..,-'`      (^)   _  -    _   Y ____)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      --           _    _ --   Y   (^)   _ (^)  ===   ----)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(          __   -  (^)  (^)      --- Y   (^) Y)", color), vcursor); vcursor.y++;
    t->put(ColorString(R"(      _            Y    Y                Y             )", color), vcursor); vcursor.y++;
    int topOfText = vcursor.y + 5;

    t->putcen(ColorString("A long time ago in the Kingdom of Bora the people lived among the gods.", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("They erected shrines all over the land to appease them,", dngutil::WHITE), topOfText, true);
    t->putcen(ColorString("and in return the gods gave their people a gift-",dngutil::WHITE), topOfText + 1, true);
    Sleep(onelineTime * 2);
    tm->clearLine(topOfText);
    tm->clearLine(topOfText + 1);

    t->clearScreen();
    vcursor.x = tlx;
    vcursor.y = tly;
    t->putcen(ColorString(R"(         ____                   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         SSSS____.              )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         (WW);;;;;\             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         `WW'____ |     ,_____  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||\ \___/,---. ) )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||\____/||| //  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||||||||" //   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||||||' //    )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||||"  //     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||||'  //      )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||||||"  //       )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||||'  //        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||||"   //         )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |||"   //          )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU ||'   //           )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(          UU |"   //            )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(         ,UU,'   ||             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(       (~~~~~~~~~~~~]""'        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
    t->putcen(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", dngutil::YELLOW), vcursor.y); vcursor.y++;

    t->putcen(ColorString("The Harp of the Gods", dngutil::YELLOW), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("Legends say it could grant what the user wanted the most by playing it", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime * 1.5);
    tm->clearLine(topOfText);

    t->clearScreen();

    t->putcen(ColorString("Of all the shrines in the land, the most important was the Tower of the Gods", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("Sacred rituals were performed there, all using the Harp", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("Bora was peaceful and prosperous.", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("One day, however, this all changed...", dngutil::MAGENTA), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    vcursor.x = tlx;
    vcursor.y = tly;

    t->put(ColorString(R"(           _/          ,          .                                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /,'    /  /      '    /     , - --- .                                 )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       \|    /  (          ,'   '           `.                               )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       ('  ,'    `.    "  / ,'                \                              )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(         ,'        \    ,/,'        '`)   (_   )                             )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(        /           \ , /'          ,      /  /                              )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       .             )  ,       ,         '  /                               )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(                      )      ,              /                                )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(       .            ' `|   ,'              /                                 )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(                    '  |  /              ,'                                  )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(        |\             | <    ______,---'                                    )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(        ` \            ','   (                                               )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(         \ '          /(____ ,`-._,-.                                        )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(          `.         /      `._, )---)                                       )", dngutil::MAGENTA), vcursor); vcursor.y++;
    t->put(ColorString(R"(            `-------'\         `/ \    )", dngutil::MAGENTA), vcursor); vcursor.y++;

    t->putcen(ColorString("The Demon Lord Zorlock rose from the underworld and attacked Bora", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("He created monsters throughout the land which terrorized citizens", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("He attacked the Tower of the Gods and broke the Harp into 4 pieces", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->clearScreen();
    vcursor.x = tlx;
    vcursor.y = tly;
    t->put(ColorString(R"(                         /\     /\)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        (_ \   (  \)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                        (_ `\iRBNMMbn.)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                         \_  \VAPRBNMMb)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                           \o_)lI<= (=\)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          / `   ) , _\ ))", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                          )    /   (_(/)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                         /_ __( \ __ _|)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                            _|     |_)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                   ,edMMNBRB)\ \_  /(PRBNMMbn.)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  dMMNBRPAV/o(_____)o\VAPRBNMMb)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 fMMWBRYIli\ o  o  o /rlIPRBWMMj)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 MMBRRSZIlti+._.-._.+ilIZSRRBMMM)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 /_o__o_/KZYI0lLRBNWMMMN\_o___o_\)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                /_____/flZJlDYTPRKBBNWMM \ ___( \)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(          NNBR0PRRBNMMMMMMMlIPZSKRNWMMMV  \__._\ \)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(          NMBRR00PRBNNMMMMM_o___o___o__/   )_ __)o))", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(          NMMBRR00PRRBNNMMM___o___o___/    /___/ /)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(          NMMBBRR00PRRBNNMM_/\_/\_/\_(  __/___x)/)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(          NMMBBRR00PRRBNNMM \/ \/ \/ __ )    /(`)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(        __NMMBBRR00PRRBNNMM_________|_ /   _'))", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(       /__        ________        ___)(<  (====.)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(       \_____________________________)_\ __\---')", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(            VMBBR00RBNMNV           |__|\/)", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(             lMBRR00RNMV  )", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(              VMB00BMNV   )", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(               VMB0BMV   )", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                b'ger  )", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                 VMV     )", dngutil::YELLOW), vcursor); vcursor.y++;
    t->put(ColorString(R"(                  V     )", dngutil::YELLOW), vcursor); vcursor.y++;

    t->putcen(ColorString("In a final confrontation he was attacked by a brave hero", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("The hero fell to the demon...", dngutil::WHITE), topOfText, true);
    Sleep(500);
    t->putcen(ColorString("but before he died he cast a spell that would contain Zorlock to the tower...", dngutil::WHITE), topOfText + 1, true);
    Sleep(onelineTime * 2);
    tm->clearLine(topOfText);
    tm->clearLine(topOfText + 1);

    t->putcen(ColorString("Now, the only way to regain entry to the tower is to use the power of the harp.", dngutil::WHITE), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->clearScreen();
    t->putcen(ColorString("Centries passed...", color), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("Zorlock's seal has begun to weaken...", color), topOfText, true);
    Sleep(onelineTime);
    tm->clearLine(topOfText);

    t->putcen(ColorString("Oh, " + player->getName() + ", descendant of the brave hero,", color), topOfText, true);
    t->putcen(ColorString("can you unite the Harp and destroy Zorlock for good before it's too late?", color), topOfText + 1, true);
    Sleep(onelineTime * 2.2);
    tm->clearLine(topOfText);
    tm->clearLine(topOfText + 1);

    t->clearScreen();

    playSound(WavFile("ExitToMap", false, true));
}
//-------------------------------------------------------