#include "game.h"
#include "virtualwindow.h"
#include "player.h"
#include "utilities.h"
#include "enemy.h"
#include "room.h"
#include "item.h"
#include "soundfile.h"
#include "map.h"

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
        map->overworldMusic.play();
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

    activeRoom = nullptr;
    floor = 0;

    spawnBeast = false;
    score = 0;

    map = new Map(this);

    titleScreen();
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
    return map->overworldMusic;
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

    case dngutil::TID::PitDragon:
        enemy = new PitDragon(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::WaterHorse:
        enemy = new WaterHorse(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::BloodJawPhase1:
        enemy = new BloodjawPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::BloodJawPhase2:
        enemy = new BloodjawPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::BloodJawPhase3:
        enemy = new BloodjawPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::EvilBeastPhase1:
        enemy = new EvilBeastPhase1(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::EvilBeastPhase2:
        enemy = new EvilBeastPhase2(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::EvilBeastPhase3:
        enemy = new EvilBeastPhase3(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
        break;

    case dngutil::TID::EvilBeastPhase4:
        enemy = new EvilBeastPhase4(this, Coordinate(-1, -1), health, attack, defense, luck, speed, difficulty);
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
    vwin->putcen(ColorString("Harp of the Gods", dngutil::GREEN), vwin->txtmacs.DIVIDER_LINES[0] + 1);
    vwin->putcen(ColorString("Enter - Continue, C - credits, Esc - exit", dngutil::LIGHTGRAY), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

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
        if (keypress(VK_RETURN))
        {
            vwin->txtmacs.clearMapArea(false, NULL);
            vwin->txtmacs.clearDivider("bottom");
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
//-------------------------------------------------------