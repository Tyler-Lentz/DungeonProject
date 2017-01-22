#include "game.h"
#include "virtualwindow.h"
#include "player.h"
#include "utilities.h"
#include "enemy.h"
#include "room.h"
#include "item.h"

#include <chrono>
#include <string>

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

        for (auto it = activeRoom->getCreatureList().begin(); it != activeRoom->getCreatureList().end(); it++)
        {
            // Returns true if player goes to new room/floor
            if ((*it)->movement())
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
    lastSave = nullptr;
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

    Game* lastSave = other.lastSave;

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
        for (auto it = gamespace[i].begin(); it != gamespace[i].end(); it++)
        {
            delete it->second;
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
    /*
    auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
    {
    };

    auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
    {
    };

    std::map<Coordinate, MapObject*> specificObjects;
    specificObjects.emplace(Coordinate(4, 1), new Potion(this, Coordinate(4, 1), dngutil::POTION_HEAL));

    std::vector<dngutil::TID> possibleCreatures;
    possibleCreatures.push_back(dngutil::TID::Skeleton);

    int difficulty = 0;
    int backColor = dngutil::LIGHTGRAY;
    std::string name = "F0: Entrance Hall";
    Coordinate mapCoord(0, 0);
    RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, floor, mapCoord);
    gamespace->emplace(mapCoord, new Room(this, rminfo, nullptr));

    activeRoom = gamespace[floor][mapCoord];
    */

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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        roomTemplate.push_back("       ^      #");
        roomTemplate.push_back("###############");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new Potion(this, Coordinate(2, 1), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(12, 2), new Potion(this, Coordinate(12, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(12, 3), new Potion(this, Coordinate(12, 3), dngutil::POTION_HEAL));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 4;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Underneath The Bridge - Dangerous Skeletons!";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#############");
        roomTemplate.push_back("#          ^#");
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
    }

    // Floor 0
    tfloor = 1;
    this->floor = tfloor; // sets the starting floor
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
            gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));

            activeRoom = gamespace[tfloor][mapCoord]; // sets the starting activeRoom
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
            possibleCreatures.push_back(dngutil::TID::Skeleton);

            int difficulty = 3;
            int backColor = dngutil::LIGHTGRAY;
            std::string name = "Main Room";
            Coordinate mapCoord(0, 1);
            RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
            gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
            gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
            gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
            roomTemplate.push_back("   ##^#####");
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
                "A spear that is colored pitch black"
            ));

            std::vector<dngutil::TID> possibleCreatures;
            possibleCreatures.push_back(dngutil::TID::Skeleton);

            int difficulty = 1;
            int backColor = dngutil::LIGHTGRAY;
            std::string name = "Hallway and Maze";
            Coordinate mapCoord(1, 1);
            RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
            gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        roomTemplate.push_back("#XXXX  v  XXXX#");
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Tight Hallways";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(this, rminfo, nullptr));
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
    for (auto it = deletionList.begin(); it != deletionList.end(); it++)
    {
        delete (*it);
    }
    deletionList.clear();
}

Game*& Game::getLastSave()
{
    return lastSave;
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
    }

    if (enemy == nullptr)
    {
        errorMessage("Enemy is a nullptr. TID is " + std::to_string(static_cast<int>(tid)), __LINE__, __FILE__);
    }

    for (int i = 0; i < enemy->getLvl(); i++)
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
    vwin->putcen(ColorString("Dungeon RPG 2", dngutil::RED), vwin->txtmacs.DIVIDER_LINES[0] + 1);
    vwin->putcen(ColorString("Enter - Continue, Esc - exit", dngutil::LIGHTGRAY), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);

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