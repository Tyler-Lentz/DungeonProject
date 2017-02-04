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
        startMp3("Overworld.mp3");
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
{
    this->vwin = vwin;
    activeRoom = nullptr;
    player = new Player(this, Coordinate(-1, -1));
    floor = 0;

    titleScreen();
    if (!exit)
    {
        makeRooms();
    }
}

Game::Game(const Game& other)
{
    this->exit = other.exit;
    this->returnVal = other.returnVal;
    this->floor = other.floor;

    this->vwin = other.vwin;
    player = new Player(*other.player, this);

    int f = dngutil::NUMFLOORS;
    for (int i = 0; i < f; i++)
    {
        for (auto it = other.gamespace[i].begin(); it != other.gamespace[i].end(); it++)
        {
            this->gamespace[i].emplace(it->first, new Room(*it->second, this));
        }
    }

    Coordinate playerMapCoord = other.activeRoom->getRoomInfo().mapCoord;
    this->activeRoom = gamespace[floor][playerMapCoord];
}

Game::~Game()
{
    for (int i = 0; i < dngutil::NUMFLOORS; i++)
    {
        for (auto& i : gamespace[i])
        {
            delete i.second;
        }
    }

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

void Game::makeRooms()
{
    std::vector<std::thread> threads;
    threads.emplace_back(&Game::makeFloor0, this);
    threads.emplace_back(&Game::makeFloor1, this);

    for (auto& thread : threads)
    {
        thread.join();
    }
}

void Game::makeFloor0()
{
    // Basement
    unsigned int tfloor = 0;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#        ^#");
        roomTemplate.push_back("#  ########");
        roomTemplate.push_back("#  ##  e  #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#e ## ### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ##^### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ## ###^#");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("# e##e### #");
        roomTemplate.push_back("#  ## ###e#");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#  ## ### #");
        roomTemplate.push_back("#o ##  o  #");
        roomTemplate.push_back("###########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 18), new Potion(this, Coordinate(1, 18), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(7, 18), new Potion(this, Coordinate(7, 18), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Rectangular Hallway";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("###^ ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###e ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("### e###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###e ###");
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("###^ ###");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Passageway";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###############");
        roomTemplate.push_back("# o           #");
        roomTemplate.push_back("#       e   o #");
        roomTemplate.push_back("# e         o #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#     e       #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#        e    #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("# e           #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#          e  #");
        roomTemplate.push_back("#    e        #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("         ^    #");
        roomTemplate.push_back("###############");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new Potion(this, Coordinate(2, 1), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(12, 2), new Potion(this, Coordinate(12, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(12, 3), new Potion(this, Coordinate(12, 3), dngutil::POTION_HEAL));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 4;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Underneath the Bridge";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#############");
        roomTemplate.push_back("#         ^ #");
        roomTemplate.push_back("# ###########");
        roomTemplate.push_back("# #    #    #");
        roomTemplate.push_back("# # ## # ## #");
        roomTemplate.push_back("# # #  # #  #");
        roomTemplate.push_back("#   #e## ## #");
        roomTemplate.push_back("### #  #  # #");
        roomTemplate.push_back("#   #o ## # #");
        roomTemplate.push_back("# ######### #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("#   ##### ###");
        roomTemplate.push_back("###     # # #");
        roomTemplate.push_back("#o# ##  # # #");
        roomTemplate.push_back("# #  #  # # #");
        roomTemplate.push_back("# #  #    # #");
        roomTemplate.push_back("# ######  # #");
        roomTemplate.push_back("#        ## #");
        roomTemplate.push_back("## ##### #  #");
        roomTemplate.push_back("## #   #   ##");
        roomTemplate.push_back("## # # ## ###");
        roomTemplate.push_back("#    #       ");
        roomTemplate.push_back("#############");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 13), new Potion(this, Coordinate(1, 13), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(5, 8), new Secondary(
            this,
            ColorChar('0', dngutil::WHITE),
            Coordinate(5, 8),
            "White Shield",
            false,
            dngutil::TID::Secondary,
            117,
            .86,
            "A very bright shield."
        )
        );

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Maze";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
}

void Game::makeFloor1()
{
    // Floor 0
    unsigned int tfloor = 1;
    this->floor = tfloor; // sets the starting floor
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "You Shouldn't be here";
        Coordinate mapCoord(0, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("#######e#######");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("#######A#######");
        roomTemplate.push_back("####### #######");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Entrance Hall";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        activeRoom = gamespace[tfloor][mapCoord]; // sets the starting activeRoom
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### ### #####");
        roomTemplate.push_back("#####     #####");
        roomTemplate.push_back("####  #-#  ####");
        roomTemplate.push_back("#### ## ## ####");
        roomTemplate.push_back("#### ##-## ####");
        roomTemplate.push_back("###  ## ##  ###");
        roomTemplate.push_back("##   ##-###  ##");
        roomTemplate.push_back("#   ###e####  #");
        roomTemplate.push_back("  ##### #####  ");
        roomTemplate.push_back("# #####^##### #");
        roomTemplate.push_back("# ##### ##### #");
        roomTemplate.push_back("#  #### ####  #");
        roomTemplate.push_back("#   ######o   #");
        roomTemplate.push_back("##   #####   ##");
        roomTemplate.push_back("###         ###");
        roomTemplate.push_back("####       ####");
        roomTemplate.push_back("#####     #####");
        roomTemplate.push_back("####### #######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(10, 12), new Potion(this, Coordinate(10, 12), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        // TODO: change this to the first boss
        possibleCreatures.push_back(dngutil::TID::LargeSkeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Main Room";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ##########");
        roomTemplate.push_back("# #        #");
        roomTemplate.push_back("# # e    e #");
        roomTemplate.push_back("# #        #");
        roomTemplate.push_back("# #        #");
        roomTemplate.push_back("# #        #");
        roomTemplate.push_back("# # e    e #");
        roomTemplate.push_back("# #        #");
        roomTemplate.push_back("# #         ");
        roomTemplate.push_back("############");

        // Will be solved when all of the enemies have been killed
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto it = creatureList.begin(); it != creatureList.end(); it++)
            {
                if ((*it)->getTypeId() != dngutil::TID::Player)
                {
                    return false;
                }
            }
            return true;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[8][1].push_back(new Key(this, Coordinate(1, 8)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Secret Treasure";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############");
        roomTemplate.push_back("#         v#");
        roomTemplate.push_back("# ##########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hallway";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#        v#");
        roomTemplate.push_back("#  ########");
        roomTemplate.push_back("#  ########");
        roomTemplate.push_back("#  ##    o#");
        roomTemplate.push_back("#  ## #o  #");
        roomTemplate.push_back("#  ## #####");
        roomTemplate.push_back("#  ##    e#");
        roomTemplate.push_back("   ##v#####");
        roomTemplate.push_back("#  ########");
        roomTemplate.push_back("#  ###e# v#");
        roomTemplate.push_back("#  ###    #");
        roomTemplate.push_back("#  ##  ####");
        roomTemplate.push_back("#  ##    ##");
        roomTemplate.push_back("#  ## ## ##");
        roomTemplate.push_back("#  ## #  ##");
        roomTemplate.push_back("#  ##     #");
        roomTemplate.push_back("#  ###### #");
        roomTemplate.push_back("#      e  #");
        roomTemplate.push_back("###########");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 5), new Potion(this, Coordinate(7, 5), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(9, 4), new Primary(
            this,
            ColorChar('/', dngutil::BLACK),
            Coordinate(9, 4),
            "Black Spear",
            false,
            dngutil::TID::Primary,
            .8,
            2,
            70,
            false,
            "A spear that is colored pitch black",
            "Attack3.wav"
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hallway and Maze";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######   ######");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX  e  XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXXe   eXXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("#XXXX     XXXX#");
        roomTemplate.push_back("##### ### #####");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto it = creatureList.begin(); it != creatureList.end(); it++)
            {
                if ((*it)->getTypeId() != dngutil::TID::Player)
                {
                    return false;
                }
            }
            return true;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            HoleObject* hole = dynamic_cast<HoleObject*>(gameMap[6][7].back());

            if (hole == nullptr)
            {
                errorMessage("at [6][7] there is not a hole? This shouldn't happen, but apparently it did lol.", __LINE__, __FILE__);
            }

            gameMap[6][7].remove(hole);
            hole->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Dangerous Bridge";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###############");
        roomTemplate.push_back("               ");
        roomTemplate.push_back("###############");
        roomTemplate.push_back("###############");
        roomTemplate.push_back("###############");
        roomTemplate.push_back("###############");
        roomTemplate.push_back("#######        ");
        roomTemplate.push_back("####### #######");
        roomTemplate.push_back("#######e#######");
        roomTemplate.push_back("  ##### #######");
        roomTemplate.push_back("# ####   ######");
        roomTemplate.push_back("#o####   ######");
        roomTemplate.push_back("######   ######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 11), new Key(this, Coordinate(1, 11)));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Tight Hallways";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###");
        roomTemplate.push_back("#  ");
        roomTemplate.push_back("#e#");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("#e#");
        roomTemplate.push_back("#  ");
        roomTemplate.push_back("###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hallway";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("       #");
        roomTemplate.push_back("###### #");
        roomTemplate.push_back("###### #");
        roomTemplate.push_back("###### #");
        roomTemplate.push_back("###### #");
        roomTemplate.push_back("       #");
        roomTemplate.push_back("###### #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Connector";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###### #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#  v   #");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("#  v   #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Passage Entrance";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
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
    return gamespace[floor];
}

std::list<MapObject*>& Game::getDeletionList()
{
    return deletionList;
}

void Game::clearDeletionList()
{
    for (auto& i : deletionList)
    {
        delete i;
    }
    deletionList.clear();
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

    case dngutil::TID::LargeSkeleton:
        enemy = new LargeSkeleton(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
        break;

    case dngutil::TID::BloodSkeleton:
        enemy = new BloodSkeleton(this, Coordinate(-1, -1), health, attack, defense, luck, speed, level);
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
    vwin->putcen(ColorString("Dungeon RPG - Dragon's Lair", dngutil::RED), vwin->txtmacs.DIVIDER_LINES[0] + 1);
    vwin->putcen(ColorString("Enter - Continue, Esc - exit", dngutil::LIGHTGRAY), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

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
    const int TOP_CURSOR_Y = vcursor.y;
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


    while (true)
    {
        if (keypress(VK_RETURN))
        {
            break;
        }
        else if (keypress(VK_ESCAPE))
        {
            cleanup(dngutil::ReturnVal::EXIT);
            break;
        }
    }

    vwin->txtmacs.clearDivider("bottom");
    stopMp3();
}
//-------------------------------------------------------