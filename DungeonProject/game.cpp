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

    spawnBeast = true;
    score = 0;

    titleScreen();
    if (!exit)
    {
        makeRooms();
    }
}

Game::~Game()
{
    for (unsigned int i = 0; i < gamespace.size(); i++)
    {
        for (auto& j : gamespace[i])
        {
            delete j.second;
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
    threads.emplace_back(&Game::makeFloor2, this);
    threads.emplace_back(&Game::makeFloor3, this);

    for (auto& thread : threads)
    {
        thread.join();
    }
}

void Game::makeFloor0()
{
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
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::Mage);

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
        possibleCreatures.push_back(dngutil::TID::Mage);

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
        specificObjects.emplace(Coordinate(2, 1), new MagicalPotion(this, Coordinate(2, 1)));
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
            157,
            .9,
            "A very bright shield, good for deflecting."
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
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("# ^  + #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Altar Room";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
}

void Game::makeFloor1()
{
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
        roomTemplate.push_back("##v   #-#  ####");
        roomTemplate.push_back("#### ## ## ####");
        roomTemplate.push_back("#### ##-## ####");
        roomTemplate.push_back("###  ## ##  ###");
        roomTemplate.push_back("##  ###-###  ##");
        roomTemplate.push_back("#  ####e####  #");
        roomTemplate.push_back("  ##### #####  ");
        roomTemplate.push_back("# #####^##### #");
        roomTemplate.push_back("# ##### ##### #");
        roomTemplate.push_back("#  #### ####  #");
        roomTemplate.push_back("#   ######o   #");
        roomTemplate.push_back("##   #####   ##");
        roomTemplate.push_back("###         ###");
        roomTemplate.push_back("####       ####");
        roomTemplate.push_back("#####  @  #####");
        roomTemplate.push_back("####### #######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(10, 12), new Potion(this, Coordinate(10, 12), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SkeletonKing);

        int difficulty = 6;
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
        roomTemplate.push_back("#######     +##");
        roomTemplate.push_back("#######      ##");
        roomTemplate.push_back("####### #######");
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
        roomTemplate.push_back("#### ^##");
        roomTemplate.push_back("#### ###");
        roomTemplate.push_back("### e ##");
        roomTemplate.push_back("### # ##");
        roomTemplate.push_back("### o ##");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("########");
        roomTemplate.push_back("#  v   #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#  o   #");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 11), new MagicalPotion(this, Coordinate(4, 11)));
        specificObjects.emplace(Coordinate(3, 18), new Key(this, Coordinate(3, 18)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Passage Entrance";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##^##############");
        roomTemplate.push_back("#   #     #   # #");
        roomTemplate.push_back("#   # ### # # # #");
        roomTemplate.push_back("#   # #   # # # #");
        roomTemplate.push_back("#     # ### # # #");
        roomTemplate.push_back("#     #     #  o#");
        roomTemplate.push_back("#     # # ### ###");
        roomTemplate.push_back("##### ### ###  ##");
        roomTemplate.push_back("##### # # ####  #");
        roomTemplate.push_back("#####   #       #");
        roomTemplate.push_back("#################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(15, 5), new Key(this, Coordinate(15, 5)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BLACK;
        std::string name = "Darkness";
        Coordinate mapCoord(1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BLACK;
        std::string name = "you shouldnt be here";
        Coordinate mapCoord(1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
}

void Game::makeFloor2()
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############");
        roomTemplate.push_back("#          #");
        roomTemplate.push_back("#          #");
        roomTemplate.push_back("#          #");
        roomTemplate.push_back("#  e       #");
        roomTemplate.push_back("#          #");
        roomTemplate.push_back("#      e   #");
        roomTemplate.push_back("#         ##");
        roomTemplate.push_back("#     ### ##");
        roomTemplate.push_back("# e   #v# ##");
        roomTemplate.push_back("#     #   ##");
        roomTemplate.push_back("############");

        // Will be solved when all of the enemies have been killed
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto i : creatureList)
            {
                if (i->getTypeId() != dngutil::TID::Player)
                {
                    return false;
                }
            }
            return true;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[0][1].back());

            if (wall == nullptr)
            {
                errorMessage("at [0][1] there is not a wall? This shouldn't happen, but apparently it did lol.", __LINE__, __FILE__);
            }

            gameMap[0][1].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = 3;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Trapped?";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###");
        roomTemplate.push_back("###");
        roomTemplate.push_back("   ");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("   ");
        roomTemplate.push_back("# #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Branching Paths";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ################");
        roomTemplate.push_back("# ##########      ");
        roomTemplate.push_back("  ##########      ");
        roomTemplate.push_back("############ #####");
        roomTemplate.push_back("############e#####");
        roomTemplate.push_back("##########   #####");
        roomTemplate.push_back("#####  ###^  #####");
        roomTemplate.push_back("####X   ##   #####");
        roomTemplate.push_back("###   +  #########");
        roomTemplate.push_back("##  X     ########");
        roomTemplate.push_back("#  X      o#######");
        roomTemplate.push_back("     X    ########");
        roomTemplate.push_back("##################");
        // coord of where you step on: 12, 3
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto& i : gameMap[3][12])
            {
                if (i->getTypeId() == dngutil::TID::Player)
                {
                    return true;
                }
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            WallObject* wall1 = dynamic_cast<WallObject*>(gameMap[7][8].back());
            WallObject* wall2 = dynamic_cast<WallObject*>(gameMap[7][9].back());

            gameMap[7][8].remove(wall1);
            wall1->removeFromMap(true);

            gameMap[7][9].remove(wall2);
            wall2->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(10, 10), new Secondary(
            this,
            ColorChar('.', dngutil::LIGHTRED),
            Coordinate(10, 10),
            "Sparking Gem",
            false,
            dngutil::TID::Secondary,
            0,
            1.23,
            "A shining red gem that protects you from damage"
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 8;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Cracked Floor + Altar Room";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##############");
        roomTemplate.push_back("             #");
        roomTemplate.push_back("      e   e  #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#    e     e #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#          e #");
        roomTemplate.push_back("# e     e    #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#    e       #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#         e  #");
        roomTemplate.push_back("#  e         #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#            #");
        roomTemplate.push_back("#         e  #");
        roomTemplate.push_back("#  e         #");
        roomTemplate.push_back("#        e   #");
        roomTemplate.push_back("#    e       #");
        roomTemplate.push_back("# e          #");
        roomTemplate.push_back("#          e #");
        roomTemplate.push_back("# ############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 8;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Pit of Dispair";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ######################");
        roomTemplate.push_back("#   #   #   #   #   ####");
        roomTemplate.push_back("#                   # ^#");
        roomTemplate.push_back("#   #   #   #   #   ####");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (this->getPlayer()->getStepCount() < 2);
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {

            WallObject* wall = dynamic_cast<WallObject*>(gameMap[2][20].back());

            if (wall == nullptr)
            {
                errorMessage("at [2][20] there is not a wall? This shouldn't happen, but apparently it did lol.", __LINE__, __FILE__);
            }

            gameMap[2][20].remove(wall);
            wall->removeFromMap(true);

            Creature* beast = generateCreature(10, dngutil::TID::DungeonBeast);
            gameMap[2][20].push_back(beast);
            this->getActiveRoom()->addCreature(beast, Coordinate(20, 2));
             
            this->setBeastSpawn(false);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::RED;
        std::string name = "Room of the Beast";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("###########");
        roomTemplate.push_back("######  e  ");
        roomTemplate.push_back("#####    ##");
        roomTemplate.push_back("####    ###");
        roomTemplate.push_back("###    ####");
        roomTemplate.push_back("##    #####");
        roomTemplate.push_back("##   ######");
        roomTemplate.push_back("##   ######");
        roomTemplate.push_back("##   ######");
        roomTemplate.push_back("##   ######");
        roomTemplate.push_back("##   ##    ");
        roomTemplate.push_back("##e  # e###");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("## e # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");
        roomTemplate.push_back("##   # ####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = 5;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Double Hallway";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##v##############");
        roomTemplate.push_back("## ##     #   # #");
        roomTemplate.push_back("## ## ### # # # #");
        roomTemplate.push_back("#   # #   # # # #");
        roomTemplate.push_back("#   # # ### # # #");
        roomTemplate.push_back("#   # #     #   #");
        roomTemplate.push_back("#   # # # ### ###");
        roomTemplate.push_back("#   # ### ###  ##");
        roomTemplate.push_back("#   # # # ####  #");
        roomTemplate.push_back("#   #   #       #");
        roomTemplate.push_back("# ###############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "View";
        Coordinate mapCoord(1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");
        roomTemplate.push_back("# o #############");
        roomTemplate.push_back("#   #############");
        roomTemplate.push_back("## ##############");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new Primary(
            this,
            ColorChar('t', dngutil::BROWN),
            Coordinate(2, 1),
            "Musket",
            false,
            dngutil::TID::Primary,
            2,
            7,
            50,
            true,
            "A very innacurate, slow, high-damaging rifle.",
            "GunAttack1.wav"
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Storeroom";
        Coordinate mapCoord(1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##   # ################");
        roomTemplate.push_back("##   #       e       o#");
        roomTemplate.push_back("##   ##              ##");
        roomTemplate.push_back("##    #################");
        roomTemplate.push_back("##       # # # # |   ##");
        roomTemplate.push_back("##       # # e # #   ##");
        roomTemplate.push_back("## e   e | # # # #   ##");
        roomTemplate.push_back("###      # # #   # ^ ##");
        roomTemplate.push_back("####     #   # # #   ##");
        roomTemplate.push_back("################### ###");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(21, 1), new Key(this, Coordinate(21, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);


        int difficulty = 6;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Double Hallways II";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("################### ###");
        roomTemplate.push_back("##################   ##");
        roomTemplate.push_back("################## + ##");
        roomTemplate.push_back("#######################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;


        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Altar Room";
        Coordinate mapCoord(-1, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
}

void Game::makeFloor3()
{
    unsigned int tfloor = 3;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ####################");
        roomTemplate.push_back("#               ######");
        roomTemplate.push_back("#  e    ##### e  #####");
        roomTemplate.push_back("#      #######    ####");
        roomTemplate.push_back("##    #########    ###");
        roomTemplate.push_back("###    #######      ##");
        roomTemplate.push_back("####  e #####        #");
        roomTemplate.push_back("#####              v #");
        roomTemplate.push_back("######               #");
        roomTemplate.push_back("######################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Beginning";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#    e     ");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("# #########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("     e    |");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#    e     ");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::BLACK;
        std::string name = "Lost?";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("     e     ");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("     e    #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#    e     ");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(-1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("     e     ");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::BLACK;
        std::string name = "Lost?";
        Coordinate mapCoord(0, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#o        #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("     e    #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 1), new Primary(
            this,
            ColorChar('|', dngutil::BLACK),
            Coordinate(1, 1),
            "Danny's Longsword",
            false,
            dngutil::TID::Primary,
            2,
            4,
            100,
            false,
            "A sword found next to a dead body.",
            "Attack1.wav"
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::BLACK;
        std::string name = "Lost?";
        Coordinate mapCoord(1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("###########");
        roomTemplate.push_back("####   ####");
        roomTemplate.push_back("###     ###");
        roomTemplate.push_back("##o     o##");
        roomTemplate.push_back("##o  o   ##");
        roomTemplate.push_back("##       ##");
        roomTemplate.push_back("###     ###");
        roomTemplate.push_back("####   ####");
        roomTemplate.push_back("##### #####");
        roomTemplate.push_back("##### #####");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 5), new Key(this, Coordinate(5, 5)));
        specificObjects.emplace(Coordinate(2, 4), new Potion(this, Coordinate(2, 4), dngutil::POTION_HEAL + 15));
        specificObjects.emplace(Coordinate(2, 5), new Potion(this, Coordinate(2, 5), dngutil::POTION_HEAL + 15));
        specificObjects.emplace(Coordinate(8, 4), new MagicalPotion(this, Coordinate(8, 4)));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Supply Room";
        Coordinate mapCoord(0, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############");
        roomTemplate.push_back("#     e    #");
        roomTemplate.push_back("# ######## #");
        roomTemplate.push_back("  ######## #");
        roomTemplate.push_back("# ######## #");
        roomTemplate.push_back("# ######## #");
        roomTemplate.push_back("#     e   v#");
        roomTemplate.push_back("############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 8;
        int backColor = dngutil::DARKGRAY;
        std::string name = "And Found";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########   ##############");
        roomTemplate.push_back("#                       #");
        roomTemplate.push_back("#                     v #");
        roomTemplate.push_back("#o                      #");
        roomTemplate.push_back("#o                      #");
        roomTemplate.push_back("#########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 3), new MagicalPotion(this, Coordinate(1, 3)));
        specificObjects.emplace(Coordinate(1, 4), new MagicalPotion(this, Coordinate(1, 4)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 10;
        int backColor = dngutil::RED;
        std::string name = "Above the Room of the Beast";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXXXXXXXXXXXX#");
        roomTemplate.push_back("#XXXXXXX o XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");
        roomTemplate.push_back("#XXXXXXX   XXXXXXX#");

        Coordinate dragon(9, 8);

        auto puzzleSolved = [dragon, this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (this->getPlayer()->getLvl() >= dngutil::SECRET_BOSS_LEVEL && exit && getPlayer()->getHp() > 0)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this, dragon](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            // y needs to go from 0 - 7
            // x needs to go from -1 of dragon.x to +1 of dragon.x
            this->getVWin()->txtmacs.displayGame(this);
            this->getVWin()->txtmacs.drawDividers();
            for (int y = 0; y < 8; y++)
            {
                for (int x = dragon.x - 1; x <= dragon.x + 1; x++)
                {
                    HoleObject* hole = dynamic_cast<HoleObject*>(gameMap[y][x].back());

                    gameMap[y][x].remove(hole);
                    hole->removeFromMap(true);
                }
                this->getActiveRoom()->drawRoom();
                Sleep(250);
            }

            MapObject* drag = nullptr;
            for (auto& i : gameMap[dragon.y][dragon.x])
            {
                if (i->getTypeId() == dngutil::TID::SegbossTrigger)
                {
                    drag = i;
                }
            }
            if (drag == nullptr)
            {
                errorMessage("shit", __LINE__, __FILE__);
            }

            gameMap[dragon.y][dragon.x].remove(drag);
            drag->removeFromMap(true);

            setBeastSpawn(true);
            getPlayer()->resetSteps();
            exit = false;
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(generateCreature(11, dngutil::TID::DragonTail)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(generateCreature(11, dngutil::TID::DragonWings)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(generateCreature(12, dngutil::TID::DragonHead)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(dragon, new SegbossTrigger(
            this, dragon,
            new Segboss(bossparts, this),
            ColorChar('S', dngutil::LIGHTRED)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 10;
        int backColor = dngutil::MAGENTA;
        std::string name = "At World's End";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }

    {
    std::vector<std::string> roomTemplate;
    roomTemplate.push_back("###################");
    roomTemplate.push_back("#   #         #   #");
    roomTemplate.push_back("### #         # ###");
    roomTemplate.push_back("#                 #");
    roomTemplate.push_back("###             ###");
    roomTemplate.push_back("#                 #");
    roomTemplate.push_back("###             ###");
    roomTemplate.push_back("########   ########");

    Coordinate beastCoord(9, 1);

    auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
    {
        return (this->getPlayer()->getStepCount() < 2);
    };

    auto puzzleAction = [this, beastCoord](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
    {
        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(generateCreature(dngutil::SECRET_BOSS_LEVEL, dngutil::TID::MegaBeastPhase1)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(generateCreature(dngutil::SECRET_BOSS_LEVEL, dngutil::TID::MegaBeastPhase2)));

        gameMap[beastCoord.y][beastCoord.x].push_back(new SegbossTrigger(
            this, beastCoord,
            new Segboss(bossparts, this),
            ColorChar('*', dngutil::WHITE)
        ));

        this->setBeastSpawn(false);
    };

    std::map<Coordinate, MapObject*> specificObjects;

    std::vector<dngutil::TID> possibleCreatures;

    int difficulty = 16;
    int backColor = dngutil::MAGENTA;
    std::string name = "Room of the Mega Beast";
    Coordinate mapCoord(2, -1);
    RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
    roomMut.lock();
    gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
    roomMut.unlock();
    }
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
    vwin->putcen(ColorString("Dungeon RPG - Dragon's Lair (BETA 5)", dngutil::RED), vwin->txtmacs.DIVIDER_LINES[0] + 1);
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

            Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 5);
            vwin->putcen(ColorString("Your village has been destroyed by a dragon,", dngutil::WHITE), vcursor.y++);
            vwin->putcen(ColorString("and you are the sole survivor...", dngutil::WHITE), vcursor.y++);
            vcursor.y += 2;
            vwin->putcen(ColorString("You have tracked the dragon back to its lair.", dngutil::WHITE), vcursor.y++);
            vwin->putcen(ColorString("With your trusty sword and shield you pass through the entrance.", dngutil::WHITE), vcursor.y++);
            vcursor.y += 5;

            vwin->putcen(ColorString("Avenge your village!", dngutil::YELLOW), vcursor.y++);
            vcursor.y += 2;
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
            credits(dngutil::CreditType::TITLESCREEN, this);
            break;
        }
    }

    

    stopMp3();
}

void Game::setBeastSpawn(bool spawn)
{
    spawnBeast = spawn;
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