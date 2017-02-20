#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <cmath>

#include "game.h"
#include "dungeon.h"
#include "utilities.h"
#include "colorstring.h"
#include "room.h"
#include "mapobject.h"
#include "item.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "enemy.h"

Dungeon::Dungeon(Game* game)
{
    pgame = game;
}

Dungeon::~Dungeon()
{
    for (unsigned int i = 0; i < gamespace.size(); i++)
    {
        for (auto& j : gamespace[i])
        {
            delete j.second;
        }
    }

    pgame->clearDeletionList();
}

DragonsLair::DragonsLair(Game* game):
    Dungeon(game)
{
    overworldMusic = "DragonsLair.mp3";

    story.push_back(std::make_pair(ColorString("Your village has been destroyed by a dragon,", dngutil::WHITE), 0));
    story.push_back(std::make_pair(ColorString("and you are the sole survivor...", dngutil::WHITE), 2));
    story.push_back(std::make_pair(ColorString("You have tracked the dragon back to its lair.", dngutil::WHITE), 0));
    story.push_back(std::make_pair(ColorString("With your trusty sword and shield you pass through the entrance.", dngutil::WHITE), 5));
    story.push_back(std::make_pair(ColorString("Avenge your village!", dngutil::YELLOW), 2));
    
    gamespace.resize(5);
    makeRooms();
}

void DragonsLair::makeRooms()
{
    std::vector<std::thread> threads;
    std::mutex roomMut;

    makeFloor0(roomMut);

    makeFloor1(roomMut);

    makeFloor2(roomMut);
    makeFloor3(roomMut);
    makeFloor4(roomMut);

   /* threads.emplace_back([this, &roomMut]() { makeFloor0(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor1(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor2(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor3(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor4(roomMut); });

    for (auto& thread : threads)
    {
        thread.join();
    }*/
}

void DragonsLair::makeFloor0(std::mutex& roomMut)
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
        specificObjects.emplace(Coordinate(1, 18), new Potion(pgame, Coordinate(1, 18), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(7, 18), new Potion(pgame, Coordinate(7, 18), dngutil::POTION_HEAL));

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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Passageway";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(2, 1), new MagicalPotion(pgame, Coordinate(2, 1)));
        specificObjects.emplace(Coordinate(12, 2), new Potion(pgame, Coordinate(12, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(12, 3), new Potion(pgame, Coordinate(12, 3), dngutil::POTION_HEAL));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 4;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Underneath the Bridge";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(1, 13), new Potion(pgame, Coordinate(1, 13), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(5, 8), new Secondary(
            pgame,
            ColorChar('0', dngutil::WHITE),
            Coordinate(5, 8),
            "White Shield",
            false,
            dngutil::TID::Secondary,
            210,
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}

void DragonsLair::makeFloor1(std::mutex& roomMut)
{
    unsigned int tfloor = 1;
    pgame->setActiveFloor(tfloor); // sets starting floor
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets starting active room
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
        specificObjects.emplace(Coordinate(10, 12), new Potion(pgame, Coordinate(10, 12), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SkeletonKing);

        int difficulty = 6;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Main Room";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
            gameMap[8][1].push_back(new Key(pgame, Coordinate(1, 8)));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(7, 5), new Potion(pgame, Coordinate(7, 5), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(9, 4), new Primary(
            pgame,
            ColorChar('/', dngutil::BLACK),
            Coordinate(9, 4),
            "Black Spear",
            false,
            .8,
            2,
            70,
            false,
            "A spear that is colored pitch black",
            "Attack3.wav",
            dngutil::ClassType::KNIGHT
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hallway and Maze";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Dangerous Bridge";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
        specificObjects.emplace(Coordinate(1, 11), new Key(pgame, Coordinate(1, 11)));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Tight Hallways";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(4, 11), new MagicalPotion(pgame, Coordinate(4, 11)));
        specificObjects.emplace(Coordinate(3, 18), new Key(pgame, Coordinate(3, 18)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Passage Entrance";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(15, 5), new Key(pgame, Coordinate(15, 5)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BLACK;
        std::string name = "Darkness";
        Coordinate mapCoord(1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}

void DragonsLair::makeFloor2(std::mutex& roomMut)
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
            pgame,
            ColorChar('.', dngutil::LIGHTRED),
            Coordinate(10, 10),
            "Sparking Gem",
            false,
            dngutil::TID::Secondary,
            10,
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 8;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Pit of Dispair";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ######################");
        roomTemplate.push_back("#   #   #   #   #   ####");
        roomTemplate.push_back("#                   # ^#");
        roomTemplate.push_back("#   #   #   #   #   ####");
        roomTemplate.push_back("########################");

        Player* p = pgame->getPlayer();
        auto puzzleSolved = [p](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (p->getStepCount() < 2);
        };

        Game* game = pgame;
        auto puzzleAction = [game](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {

            WallObject* wall = dynamic_cast<WallObject*>(gameMap[2][20].back());

            if (wall == nullptr)
            {
                errorMessage("at [2][20] there is not a wall? This shouldn't happen, but apparently it did lol.", __LINE__, __FILE__);
            }

            gameMap[2][20].remove(wall);
            wall->removeFromMap(true);

            Creature* beast = game->generateCreature(10, dngutil::TID::DungeonBeast);
            gameMap[2][20].push_back(beast);
            game->getActiveRoom()->addCreature(beast, Coordinate(20, 2));

            game->setBeastSpawn(false);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::RED;
        std::string name = "Room of the Beast";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("###########");
        roomTemplate.push_back("##o     e  ");
        roomTemplate.push_back("## ##    ##");
        roomTemplate.push_back("## #    ###");
        roomTemplate.push_back("##     ####");
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
        specificObjects.emplace(Coordinate(2, 2), new Flute(pgame, Coordinate(2, 2)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = 5;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Double Hallway";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
            pgame,
            ColorChar('t', dngutil::BROWN),
            Coordinate(2, 1),
            "Musket",
            false,
            2,
            7,
            50,
            true,
            "A very innacurate, slow, high-damaging rifle.",
            "GunAttack1.wav",
            dngutil::ClassType::RANGER
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Storeroom";
        Coordinate mapCoord(1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(21, 1), new Key(pgame, Coordinate(21, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);


        int difficulty = 6;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Double Hallways II";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}

void DragonsLair::makeFloor3(std::mutex& roomMut)
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#    ^    #");
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Lost?";
        Coordinate mapCoord(-1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
            pgame,
            ColorChar('|', dngutil::BLACK),
            Coordinate(1, 1),
            "Danny's Longsword",
            false,
            2,
            4,
            100,
            false,
            "A sword found next to a dead body.",
            "Attack1.wav",
            dngutil::ClassType::KNIGHT
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(5, 5), new Key(pgame, Coordinate(5, 5)));
        specificObjects.emplace(Coordinate(2, 4), new Potion(pgame, Coordinate(2, 4), dngutil::POTION_HEAL + 15));
        specificObjects.emplace(Coordinate(2, 5), new Potion(pgame, Coordinate(2, 5), dngutil::POTION_HEAL + 15));
        specificObjects.emplace(Coordinate(8, 4), new MagicalPotion(pgame, Coordinate(8, 4)));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 7;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Supply Room";
        Coordinate mapCoord(0, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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
        specificObjects.emplace(Coordinate(1, 3), new MagicalPotion(pgame, Coordinate(1, 3)));
        specificObjects.emplace(Coordinate(1, 4), new MagicalPotion(pgame, Coordinate(1, 4)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 10;
        int backColor = dngutil::RED;
        std::string name = "Above the Room of the Beast";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
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

        Game* game = pgame;
        auto puzzleSolved = [dragon, game](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (game->getPlayer()->getLvl() >= dngutil::SECRET_BOSS_LEVEL && game->shouldExit() && game->getPlayer()->getHp() > 0 && game->getDifficulty().canFightMegabeast)
            {
                return true;
            }
            return false;
        };
        
        auto puzzleAction = [game, dragon](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            // y needs to go from 0 - 7
            // x needs to go from -1 of dragon.x to +1 of dragon.x
            game->getVWin()->txtmacs.displayGame(game);
            game->getVWin()->txtmacs.drawDividers();
            for (int y = 0; y < 8; y++)
            {
                for (int x = dragon.x - 1; x <= dragon.x + 1; x++)
                {
                    HoleObject* hole = dynamic_cast<HoleObject*>(gameMap[y][x].back());

                    gameMap[y][x].remove(hole);
                    hole->removeFromMap(true);
                }
                game->getActiveRoom()->drawRoom();
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

            game->setBeastSpawn(true);
            game->getPlayer()->resetSteps();
            game->setExitToFalse();
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(11, dngutil::TID::DragonTail)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(11, dngutil::TID::DragonWings)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(12, dngutil::TID::DragonHead)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(dragon, new SegbossTrigger(
            pgame, dragon,
            new Segboss(bossparts, pgame),
            ColorChar('S', dngutil::LIGHTRED)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 10;
        int backColor = dngutil::MAGENTA;
        std::string name = "At World's End";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
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

        Game* game = pgame;
        auto puzzleSolved = [game](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (game->getPlayer()->getStepCount() < 2);
        };

        auto puzzleAction = [game, beastCoord](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<SegEnemy*> bossparts;
            bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(dngutil::SECRET_BOSS_LEVEL, dngutil::TID::MegaBeastPhase1)));
            bossparts[0]->setFirst();
            bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(dngutil::SECRET_BOSS_LEVEL, dngutil::TID::MegaBeastPhase2)));

            gameMap[beastCoord.y][beastCoord.x].push_back(new SegbossTrigger(
                game, beastCoord,
                new Segboss(bossparts, game),
                ColorChar('*', dngutil::WHITE)
            ));

            game->setBeastSpawn(false);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 16;
        int backColor = dngutil::MAGENTA;
        std::string name = "Room of the Mega Beast";
        Coordinate mapCoord(2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
}

void DragonsLair::makeFloor4(std::mutex& roomMut)
{
    int tfloor = 4;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########");
        roomTemplate.push_back("#    v    #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("#    o    #");
        roomTemplate.push_back("#         #");
        roomTemplate.push_back("###########");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 4), new HerosClaim(pgame, Coordinate(5, 4)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Room of the Hero";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}




GryphonsTower::GryphonsTower(Game* game):
    Dungeon(game)
{
    overworldMusic = "GryphonsTower.mp3";

    story.push_back(std::make_pair(ColorString("Your family has been kidnapped by a gryphon.", dngutil::WHITE), 2));

    story.push_back(std::make_pair(ColorString("This gryphon is feared by your town,", dngutil::WHITE), 0));
    story.push_back(std::make_pair(ColorString("once someone is taken they are never to be seen again...", dngutil::WHITE), 2));

    story.push_back(std::make_pair(ColorString("However, you have followed the gryphon back to its tower.", dngutil::WHITE), 0));
    story.push_back(std::make_pair(ColorString("With your trusty sword and shield you pass through the entrance.", dngutil::WHITE), 5));
    story.push_back(std::make_pair(ColorString("Save your family!", dngutil::YELLOW), 2));

    gamespace.resize(7);
    makeRooms();
}

void GryphonsTower::makeRooms()
{
    std::vector<std::thread> threads;
    std::mutex roomMut;

    threads.emplace_back([this, &roomMut]() { makeFloor0(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor1(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor2(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor3(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor4(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor5(roomMut); });
    threads.emplace_back([this, &roomMut]() { makeFloor6(roomMut); });

    for (auto& thread : threads)
    {
        thread.join();
    }
}

void GryphonsTower::makeFloor0(std::mutex& roomMut)
{
    unsigned int tfloor = 0;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#      ");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#  ^  #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Darker";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#               #");
        roomTemplate.push_back("#       e       #");
        roomTemplate.push_back("#               #");
        roomTemplate.push_back("#     ###########");
        roomTemplate.push_back("#     #e  #     #");
        roomTemplate.push_back("      # ^ #  e  #");
        roomTemplate.push_back("#     #  o#     #");
        roomTemplate.push_back("#  e  #####     #");
        roomTemplate.push_back("#     #         #");
        roomTemplate.push_back("#     # e       #");
        roomTemplate.push_back("#     #         #");
        roomTemplate.push_back("#     #          ");
        roomTemplate.push_back("#################");

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
            for (Coordinate i(13, 0); i.x <= 15; i.x++)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(9, 7), new Potion(pgame, Coordinate(9, 7), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Under the Center Room";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#############   #");
        roomTemplate.push_back("############# ^ #");
        roomTemplate.push_back("#############   #");
        roomTemplate.push_back("#############   #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Secret Stairwell";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#      ");
        roomTemplate.push_back("#######");

        // Will be solved when all of the enemies have been killed
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto it = gameMap[3][2].begin(); it != gameMap[3][2].end(); it++)
            {
                if ((*it)->getTypeId() == dngutil::TID::Player)
                {
                    return true;
                }
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[12][0].back());

            gameMap[12][0].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Hidden Switch";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#####");
        roomTemplate.push_back("#####");
        roomTemplate.push_back("#####");
        roomTemplate.push_back("##^##");
        roomTemplate.push_back("## ##");
        roomTemplate.push_back("## ##");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("    #");
        roomTemplate.push_back("#####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor1(std::mutex& roomMut)
{
    unsigned int tfloor = 1;
    pgame->setActiveFloor(tfloor);
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "You shouldnt be here";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("#A#");
        roomTemplate.push_back("# #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Entrance Hall";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets the starting active room
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#      ");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#  v  #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#######");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Locked Away Staircase";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########  ######");
        roomTemplate.push_back("#               #");
        roomTemplate.push_back("#     e         #");
        roomTemplate.push_back("#               #");
        roomTemplate.push_back("#     o###      #");
        roomTemplate.push_back("#     #####     #");
        roomTemplate.push_back("|    ###v###    #");
        roomTemplate.push_back("#     #####     #");
        roomTemplate.push_back("#      ###      #");
        roomTemplate.push_back("#               #");
        roomTemplate.push_back("#           e   #");
        roomTemplate.push_back("#e#             #");
        roomTemplate.push_back("# ##             ");
        roomTemplate.push_back("# ###############");

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
            std::list<Coordinate> deletionList;
            deletionList.push_back(Coordinate(5, 6));
            deletionList.push_back(Coordinate(6, 6));
            deletionList.push_back(Coordinate(7, 6));
            deletionList.push_back(Coordinate(9, 6));
            deletionList.push_back(Coordinate(10, 6));
            deletionList.push_back(Coordinate(11, 6));

            deletionList.push_back(Coordinate(8, 4));
            deletionList.push_back(Coordinate(8, 5));
            deletionList.push_back(Coordinate(8, 7));
            deletionList.push_back(Coordinate(8, 8));

            for (auto i : deletionList)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 4), new Potion(pgame, Coordinate(6, 4), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Center Room";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#          ##   #");
        roomTemplate.push_back("#     e    ## ^ #");
        roomTemplate.push_back("#          ##   #");
        roomTemplate.push_back("#o         ##   #");
        roomTemplate.push_back("#     e    ## v #");
        roomTemplate.push_back("#          ##   #");
        roomTemplate.push_back("#########  ######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 4), new Key(pgame, Coordinate(1, 4)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Key Room";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#o   o#");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("# e e #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("       ");
        roomTemplate.push_back("#######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 2), new Potion(pgame, Coordinate(1, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(5, 2), new Potion(pgame, Coordinate(5, 2), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Guarded Hall";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#####");
        roomTemplate.push_back("#####");
        roomTemplate.push_back("#####");
        roomTemplate.push_back("##v##");
        roomTemplate.push_back("## ##");
        roomTemplate.push_back("## ##");
        roomTemplate.push_back("# e #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("#   #");
        roomTemplate.push_back("    #");
        roomTemplate.push_back("#####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 3;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Downstairs";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor2(std::mutex& roomMut)
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################");
        roomTemplate.push_back("#XXXXXXXXXXX#####");
        roomTemplate.push_back("#XXXXXX   XX##v##");
        roomTemplate.push_back("#XXXX      e   ##");
        roomTemplate.push_back("#   e   X      ##");
        roomTemplate.push_back("#     XXXXXX#####");
        roomTemplate.push_back("#  XXXXXXXXX#####");
        roomTemplate.push_back("#  ##############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 4;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Wavy Bridge";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("# e#");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("|   ");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#e #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 4;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hall";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#############");
        roomTemplate.push_back("####  o  ####");
        roomTemplate.push_back("###       ###");
        roomTemplate.push_back("##         ##");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("            #");
        roomTemplate.push_back("     e      #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("##         ##");
        roomTemplate.push_back("###       ###");
        roomTemplate.push_back("####     ##  ");
        roomTemplate.push_back("#############");
        // 11,13
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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[13][11].back());

            gameMap[13][11].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 1), new MagicalPotion(pgame, Coordinate(6, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SkeletonKing);

        int difficulty = 7;
        int backColor = dngutil::BROWN;
        std::string name = "Skeleton King's Court";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####");
        roomTemplate.push_back("####");
        roomTemplate.push_back("####");
        roomTemplate.push_back("####");
        roomTemplate.push_back("####");
        roomTemplate.push_back("####");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("#^##");
        roomTemplate.push_back("####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Side Hall";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Corner";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########### #");
        roomTemplate.push_back("     e      #");
        roomTemplate.push_back("            #");
        roomTemplate.push_back("#############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 6;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Hall";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#    o #");
        roomTemplate.push_back("# e    #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#e     #");
        roomTemplate.push_back("#  e e #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("# e    #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#    e #");
        roomTemplate.push_back("# e    #");
        roomTemplate.push_back("       #");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 1), new Key(pgame, Coordinate(5, 1)));

        std::vector<dngutil::TID> possibleCreatures;

        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 5;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Mosh Pit";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor3(std::mutex& roomMut)
{
    unsigned int tfloor = 3;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("#@##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("# ##");
        roomTemplate.push_back("#v##");
        roomTemplate.push_back("####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Fate";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("#   ");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#ee#");
        roomTemplate.push_back("#  #");
        roomTemplate.push_back("#  #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 7;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Turn";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###################");
        roomTemplate.push_back("      o ###     ###");
        roomTemplate.push_back("        ###    e ##");
        roomTemplate.push_back("#   e   ### ###   #");
        roomTemplate.push_back("#       ### e #   #");
        roomTemplate.push_back("##      ###   ###  ");
        roomTemplate.push_back("###   e ###        ");
        roomTemplate.push_back("######  ###  ######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 1), new Flute(pgame, Coordinate(6, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 7;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Blocked";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#bbbbb#");
        roomTemplate.push_back("#bbbbb#");
        roomTemplate.push_back("#bbbbb#");
        roomTemplate.push_back("#bbbbb#");
        roomTemplate.push_back("#bbbbb#");
        roomTemplate.push_back("       ");
        roomTemplate.push_back("       ");
        roomTemplate.push_back("# bbbb#");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Outside Bridge";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# b");
        roomTemplate.push_back("# b");
        roomTemplate.push_back("# b");
        roomTemplate.push_back("# b");
        roomTemplate.push_back("# b");
        roomTemplate.push_back("# b");
        roomTemplate.push_back("#eb");
        roomTemplate.push_back("# b");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 8;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Outside Bridge";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# b");
        roomTemplate.push_back("  b");
        roomTemplate.push_back("  b");
        roomTemplate.push_back("bbb");
        roomTemplate.push_back("bbb");
        roomTemplate.push_back("bbb");
        roomTemplate.push_back("bbb");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Outside Edge";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###");
        roomTemplate.push_back("#^#");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("#-#");
        roomTemplate.push_back("  #");
        roomTemplate.push_back("  #");
        roomTemplate.push_back("###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Stairwell";
        Coordinate mapCoord(2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######  ###  ######");
        roomTemplate.push_back("#        #        #");
        roomTemplate.push_back("#        # ###### #");
        roomTemplate.push_back("#   e    | #  e   #");
        roomTemplate.push_back("#        # #      #");
        roomTemplate.push_back("#        #        #");
        roomTemplate.push_back("## ################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 7;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Attatched rooms";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("## ################");
        roomTemplate.push_back("#+      ##o    e   ");
        roomTemplate.push_back("#  o#   ##         ");
        roomTemplate.push_back("#  ###  ##bbbbbbbbb");
        roomTemplate.push_back("#   #o  ##bbbbbbbbb");
        roomTemplate.push_back("#e      ##bbbbbbbbb");
        roomTemplate.push_back("## #######bbbbbbbbb");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 2), new Potion(pgame, Coordinate(3, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(5, 4), new Potion(pgame, Coordinate(5, 4), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(10, 1), new Key(pgame, Coordinate(10, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = 7;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Double";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("bb   o bb");
        roomTemplate.push_back("bbbbbbbbb");
        roomTemplate.push_back("bbbbbbbbb");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 0), new Key(pgame, Coordinate(5, 0)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 7;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Balcony";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor4(std::mutex& roomMut)
{
    unsigned int tfloor = 4;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###");
        roomTemplate.push_back("#v#");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("# #");
        roomTemplate.push_back("  #");
        roomTemplate.push_back("###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Stairwell";
        Coordinate mapCoord(2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("# bbbe#");
        roomTemplate.push_back("# bbb #");
        roomTemplate.push_back("#ebbb #");
        roomTemplate.push_back("# bbb #");
        roomTemplate.push_back("# bbb  ");
        roomTemplate.push_back("# bbbb#");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "On the Edge";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("  bbbbb");
        roomTemplate.push_back("  bbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("# bbbbb");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "On the Edge";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("#ebbbbb");
        roomTemplate.push_back("# bbbbb");
        roomTemplate.push_back("#obbbbb");
        roomTemplate.push_back("#bbbbbb");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 4), new Primary(
            pgame,
            ColorChar('(', dngutil::BROWN),
            Coordinate(1, 4),
            "Crossbow",
            false,
            1.65,
            5,
            95,
            true,
            "A very accurate ranged weapon. Shoots with force.",
            "BowAttack1.wav",
            dngutil::ClassType::RANGER
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "On the Edge";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########");
        roomTemplate.push_back("#e       #");
        roomTemplate.push_back("# #####e  ");
        roomTemplate.push_back("# #o  #   ");
        roomTemplate.push_back("  # o #  #");
        roomTemplate.push_back("# #####  #");
        roomTemplate.push_back("#   e   e#");
        roomTemplate.push_back("##########");

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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[4][6].back());

            gameMap[4][6].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(3, 3), new MagicalPotion(pgame, Coordinate(3, 3)));
        specificObjects.emplace(Coordinate(4, 4), new Key(pgame, Coordinate(4, 4)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::Mage);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Entrance to the Labyrinth";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########");
        roomTemplate.push_back("#    #e #");
        roomTemplate.push_back("## ###  #");
        roomTemplate.push_back("#    #  #");
        roomTemplate.push_back("| #e #   ");
        roomTemplate.push_back("# # ### #");
        roomTemplate.push_back("# #     #");
        roomTemplate.push_back("# ########");

        // Will be solved when all of the enemies have been killed
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto it = gameMap[1][1].begin(); it != gameMap[1][1].end(); it++)
            {
                if ((*it)->getTypeId() == dngutil::TID::Player)
                {
                    return true;
                }
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[0][4].back());

            gameMap[0][4].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Labyrinth : Hidden Switch";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########");
        roomTemplate.push_back("#      o#");
        roomTemplate.push_back("# #######");
        roomTemplate.push_back("# #     #");
        roomTemplate.push_back("| # e e #");
        roomTemplate.push_back("# #     #");
        roomTemplate.push_back("#        ");
        roomTemplate.push_back("#### ####");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 1), new MagicalPotion(pgame, Coordinate(7, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 10;
        int backColor = dngutil::BLACK;
        std::string name = "The Labyrinth : Dark Room";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#    #o#");
        roomTemplate.push_back("# ##   #");
        roomTemplate.push_back("#e#### #");
        roomTemplate.push_back("# ####  ");
        roomTemplate.push_back("#    ###");
        roomTemplate.push_back("# ## ###");
        roomTemplate.push_back("# ## ###");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 1), new Potion(pgame, Coordinate(6, 1), dngutil::POTION_HEAL + 20));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Labyrinth : Narrow Halls";
        Coordinate mapCoord(-2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ## ###");
        roomTemplate.push_back("# ## # #");
        roomTemplate.push_back("# ##e#e#");
        roomTemplate.push_back("# ## # #");
        roomTemplate.push_back("# ## #  ");
        roomTemplate.push_back("#    # #");
        roomTemplate.push_back("#    #e#");
        roomTemplate.push_back("########");

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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[7][1].back());

            gameMap[7][1].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Labyrinth : Puzzle?";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ######");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#   e  #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("########");

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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[5][1].back());

            gameMap[5][1].remove(wall);
            wall->removeFromMap(true);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::FlameHorse);

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Labyrinth : Flame Horse's Den";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# ######");
        roomTemplate.push_back("#     ^#");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 11;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Labyrinth Stairwell";
        Coordinate mapCoord(-2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########");
        roomTemplate.push_back("# e ######");
        roomTemplate.push_back("# #      #");
        roomTemplate.push_back("# ##e### #");
        roomTemplate.push_back("#o#o #   #");
        roomTemplate.push_back("###### ###");
        roomTemplate.push_back("  e      #");
        roomTemplate.push_back("##########");

        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(1, 4), new Key(pgame, Coordinate(1, 4)));
        specificObjects.emplace(Coordinate(3, 4), new Potion(pgame, Coordinate(3, 4), dngutil::POTION_HEAL + 10));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 10;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Labyrinth : Narrow Halls";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("# #######");
        roomTemplate.push_back("#      ##");
        roomTemplate.push_back("#      e ");
        roomTemplate.push_back("#      e ");
        roomTemplate.push_back("#      ##");
        roomTemplate.push_back("#   +   #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#bbbbbbb#");
        roomTemplate.push_back("#bbbbbbb#");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Labyrinth : Balcony";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########");
        roomTemplate.push_back("####     #");
        roomTemplate.push_back("         #");
        roomTemplate.push_back("     o   #");
        roomTemplate.push_back("####     #");
        roomTemplate.push_back("##########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 3), new HerosClaim(pgame, Coordinate(5, 3)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = 10;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Labyrinth : Room of the Hero";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor5(std::mutex& roomMut)
{
    unsigned int tfloor = 5;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###  ###");
        roomTemplate.push_back("#     ^#");
        roomTemplate.push_back("########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 11;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Labyrinth Stairwell";
        Coordinate mapCoord(-2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#  e   #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("###  ###");

        Game* game = pgame;
        auto puzzleSolved = [game](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            for (auto it = creatureList.begin(); it != creatureList.end(); it++)
            {
                if ((*it)->getTypeId() != dngutil::TID::Player)
                {
                    return false;
                }
            }
            game->setBeastSpawn(false);
            return true;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[0][3].back());

            gameMap[0][3].remove(wall);
            wall->removeFromMap(true);

            wall = dynamic_cast<WallObject*>(gameMap[0][4].back());

            gameMap[0][4].remove(wall);
            wall->removeFromMap(true);
        };


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DungeonBeast);

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Beast's Blockade";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("################");
        roomTemplate.push_back("#              #");
        roomTemplate.push_back("#            ^ #");
        roomTemplate.push_back("#              #");
        roomTemplate.push_back("#             o#");
        roomTemplate.push_back("###  ###########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(14, 4), new MagicalPotion(pgame, Coordinate(14, 4)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Final Stairwell";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
}
void GryphonsTower::makeFloor6(std::mutex& roomMut)
{
    unsigned int tfloor = 6;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("################");
        roomTemplate.push_back("###########    #");
        roomTemplate.push_back("###########  v #");
        roomTemplate.push_back("###########     ");
        roomTemplate.push_back("###########    #");
        roomTemplate.push_back("################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Final Stairwell";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("bbbbbb");
        roomTemplate.push_back("bbbbbb");
        roomTemplate.push_back("bbbbbb");
        roomTemplate.push_back("    bb");
        roomTemplate.push_back("bb  bb");
        roomTemplate.push_back("bb  bb");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 13;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        roomMut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("bb  bbbbbbbbbb");
        roomTemplate.push_back("bb  bbbbbbbbbb");
        roomTemplate.push_back("bb        bbbb");
        roomTemplate.push_back("bb         bbb");
        roomTemplate.push_back("bb      o   bb");
        roomTemplate.push_back("bbb        bbb");
        roomTemplate.push_back("bbbb      bbbb");
        roomTemplate.push_back("bbbbbbbbbbbbbb");
        roomTemplate.push_back("bbbbbbbbbbbbbb");

        Coordinate gryphonCoord(8, 4);

        Game* game = pgame;
        auto puzzleSolved = [game](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (game->getPlayer()->getLvl() >= dngutil::SECOND_SECRET_BOSS_LEVEL && game->shouldExit() && game->getPlayer()->getHp() > 0 && game->getDifficulty().canFightMegabeast)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [game, gryphonCoord](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {

            MapObject* drag = nullptr;
            for (auto& i : gameMap[gryphonCoord.y][gryphonCoord.x])
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

            gameMap[gryphonCoord.y][gryphonCoord.x].remove(drag);
            drag->removeFromMap(true);

            std::vector<SegEnemy*> bossparts;
            bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(dngutil::SECOND_SECRET_BOSS_LEVEL, dngutil::TID::ReaperPhase1)));
            bossparts[0]->setFirst();
            bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(dngutil::SECOND_SECRET_BOSS_LEVEL, dngutil::TID::ReaperPhase2)));
            bossparts.push_back(dynamic_cast<SegEnemy*>(game->generateCreature(dngutil::SECOND_SECRET_BOSS_LEVEL, dngutil::TID::ReaperPhase3)));

            gameMap[0][2].push_back(new SegbossTrigger(
                game, Coordinate(2, 0),
                new Segboss(bossparts, game),
                ColorChar('A', dngutil::WHITE)
            ));


            game->getVWin()->txtmacs.displayGame(game);
            game->getVWin()->txtmacs.drawDividers();

            game->setExitToFalse();
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(13, dngutil::TID::GryphonPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(13, dngutil::TID::GryphonPhase2)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(14, dngutil::TID::GryphonPhase3)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(8, 4), new SegbossTrigger(
            pgame, Coordinate(8, 4),
            new Segboss(bossparts, pgame),
            ColorChar('M', dngutil::BROWN)
        ));


        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 13;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Top of the Tower : Gryphon's Nest";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        roomMut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        roomMut.unlock();
    }
}

PitOf50Trials::PitOf50Trials(Game* game) :
    Dungeon(game)
{
    overworldMusic = "PitTheme.mp3";

    story.push_back(std::make_pair(ColorString("Legends tell of a dungeon of fifty trials...", dngutil::WHITE), 0));
    story.push_back(std::make_pair(ColorString("Fifty trials of unspeakable pain.", dngutil::WHITE), 2));

    story.push_back(std::make_pair(ColorString("Today you have found that dungeon", dngutil::WHITE), 2));

    story.push_back(std::make_pair(ColorString("As you enter the dungeon the door closes behind you.", dngutil::WHITE), 5));
    story.push_back(std::make_pair(ColorString("Will this be your deathbed?", dngutil::RED), 2));

    gamespace.resize(51);
    generateDungeon();
}

void PitOf50Trials::generateDungeon()
{
    {
        int floor = 50;
        pgame->setActiveFloor(floor);
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("# e   A #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#       #");
        roomTemplate.push_back("#########");

        Coordinate stairCoord(6, 4);
        Coordinate spObjCoord(4, 2);

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

        Game* game = pgame;
        auto puzzleAction = [stairCoord, game](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[stairCoord.y][stairCoord.x].push_back(new ExitObject(game, stairCoord, false, ColorChar('v', dngutil::WHITE)));
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(spObjCoord, new Potion(pgame, spObjCoord, dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Trial " + std::to_string(51 - floor);
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, floor, mapCoord);
        gamespace[floor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        pgame->setActiveRoom(gamespace[floor][mapCoord]); // sets the starting active room
    }
    for (int floor = 49; floor > 0; floor--)
    {
        {
            std::vector<std::string> roomTemplate;
            roomTemplate.push_back("#########");
            roomTemplate.push_back("#       #");
            roomTemplate.push_back("#   o   #");
            roomTemplate.push_back("#       #");
            roomTemplate.push_back("# e     #");
            roomTemplate.push_back("#       #");
            roomTemplate.push_back("#       #");
            roomTemplate.push_back("#       #");
            roomTemplate.push_back("#########");

            Coordinate stairCoord(6, 4);
            Coordinate spObjCoord(4, 2);
            auto puzzleSolved = [spObjCoord, floor](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
            {
                if (floor == 1)
                {
                    return false;
                }

                for (auto it = creatureList.begin(); it != creatureList.end(); it++)
                {
                    if ((*it)->getTypeId() != dngutil::TID::Player)
                    {
                        return false;
                    }
                }

                return true;
            };

            Game* game = pgame;
            auto puzzleAction = [stairCoord, game](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
            {
                gameMap[stairCoord.y][stairCoord.x].push_back(new ExitObject(game, stairCoord, false, ColorChar('v', dngutil::WHITE)));
            };

            std::map<Coordinate, MapObject*> specificObjects;
            if (floor % 5 == 0)
            {
                specificObjects.emplace(spObjCoord, new Potion(pgame, spObjCoord, dngutil::POTION_HEAL));
            }
            else if (floor % 9 == 0)
            {
                specificObjects.emplace(spObjCoord, new MagicalPotion(pgame, spObjCoord));
            }
            else
            {
                specificObjects.emplace(spObjCoord, new EmptyObject(pgame, spObjCoord));
            }

            std::vector<dngutil::TID> possibleCreatures;
            int backColor;
            if (floor >= 45)
            {
                possibleCreatures.push_back(dngutil::TID::Skeleton);
                backColor = dngutil::LIGHTGRAY;
            }
            else if (floor >= 38)
            {
                possibleCreatures.push_back(dngutil::TID::Skeleton);
                possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
                possibleCreatures.push_back(dngutil::TID::Bowman);
                backColor = dngutil::LIGHTGRAY;
            }
            else if (floor >= 26)
            {
                possibleCreatures.push_back(dngutil::TID::LSKnight);
                possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
                possibleCreatures.push_back(dngutil::TID::SSKnight);
                backColor = dngutil::DARKGRAY;
            }
            else if (floor >= 15)
            {
                possibleCreatures.push_back(dngutil::TID::Mage);
                possibleCreatures.push_back(dngutil::TID::LSKnight);
                backColor = dngutil::LIGHTMAGENTA;
            }
            else if (floor >= 2)
            {
                possibleCreatures.push_back(dngutil::TID::Mage);
                possibleCreatures.push_back(dngutil::TID::LSKnight);
                backColor = dngutil::MAGENTA;
            }
            else  // final trial
            {
                possibleCreatures.push_back(dngutil::TID::Mage);
                backColor = dngutil::MAGENTA;
            }

            int difficulty = static_cast<int>((51 - floor) / 2.5);
            if (floor == 10 || floor == 20 || floor == 30 || floor == 40 || floor == 0)
            {
                possibleCreatures.clear();
                switch (floor)
                {
                case 40:
                    possibleCreatures.push_back(dngutil::TID::SkeletonKing);
                    break;
                case 30:
                    possibleCreatures.push_back(dngutil::TID::DungeonBeast);
                    break;
                case 20:
                    possibleCreatures.push_back(dngutil::TID::FlameHorse);
                    break;
                case 10:
                    possibleCreatures.push_back(dngutil::TID::MegaBeastPhase2);
                    break;
                    
                case 0:
                {
                    possibleCreatures.push_back(dngutil::TID::Skeleton);
                    std::vector<SegEnemy*> bossparts;
                    bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(difficulty, dngutil::TID::MaskPhase1)));
                    bossparts[0]->setFirst();
                    bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(difficulty, dngutil::TID::MaskPhase2)));
                    bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(difficulty, dngutil::TID::MaskPhase3)));

                    std::map<Coordinate, MapObject*> specificObjects;
                    specificObjects.emplace(spObjCoord, new SegbossTrigger(
                        pgame, spObjCoord,
                        new Segboss(bossparts, pgame),
                        ColorChar('O', dngutil::WHITE)
                    ));
                    break;
                }
                }
            }

            std::string name = "Trial " + std::to_string(51 - floor);
            Coordinate mapCoord(0, 0);
            RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, floor, mapCoord);
            gamespace[floor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction)));
        }
    }
}