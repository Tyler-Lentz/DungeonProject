#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <cmath>
#include <functional>

#include "npc.h"
#include "game.h"
#include "map.h"
#include "utilities.h"
#include "colorstring.h"
#include "room.h"
#include "mapobject.h"
#include "item.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "enemy.h"

Map::Map(Game* game)
{
    pgame = game;

    gamespace.resize(15);

    std::mutex mut;
    std::vector<std::thread> functions;
    functions.emplace_back([&]() {makeOverworld(mut); });
    functions.emplace_back([&]() {makeForestTemple(mut); });
    functions.emplace_back([&]() {makeHouses(mut); });
    for (auto& i : functions)
    {
        i.join();
    }
}

void Map::makeOverworld(std::mutex& mut)
{
    unsigned int tfloor = 2; // if overworld floor changes need to change dungeon checks assuming overworld is floor 2 also
    pgame->setActiveFloor(tfloor); // sets starting floor

    // Connecting areas
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################      #");
        roomTemplate.push_back("###########            #");
        roomTemplate.push_back("###########            #");
        roomTemplate.push_back("#####v#####            #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("#    A                 #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Begining";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets starting active room
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#################      #");
        roomTemplate.push_back("  ###############      #");
        roomTemplate.push_back("    #############      #");
        roomTemplate.push_back("           ######      #");
        roomTemplate.push_back("  0           ###      #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("  e                    #");
        roomTemplate.push_back("             ####      #");
        roomTemplate.push_back("            #####      #");
        roomTemplate.push_back("   0      #######      #");
        roomTemplate.push_back("        #########      #");
        roomTemplate.push_back("#################      #");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####           ####  ###");
        roomTemplate.push_back("               ####     ");
        roomTemplate.push_back("  #####    e   ####     ");
        roomTemplate.push_back("  #####                 ");
        roomTemplate.push_back("  #####                 ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("             ####       ");
        roomTemplate.push_back("             ####       ");
        roomTemplate.push_back("  ##### e    ####       ");
        roomTemplate.push_back("  #####                 ");
        roomTemplate.push_back("  #####           ####  ");
        roomTemplate.push_back("                  ####  ");
        roomTemplate.push_back("##   ###################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##   ###################");
        roomTemplate.push_back("#       e              #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("             e          ");
        roomTemplate.push_back("             ####       ");
        roomTemplate.push_back("#    #####     ###     #");
        roomTemplate.push_back("#    #####       #######");
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back("   e            ########");
        roomTemplate.push_back("###########   ##########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##   ###################");
        roomTemplate.push_back("#       e              #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#            #  #      #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("             e          ");
        roomTemplate.push_back("             ####       ");
        roomTemplate.push_back("#    #####     ###     #");
        roomTemplate.push_back("#    #####       #######");
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back("   e            ########");
        roomTemplate.push_back("###########   ##########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("##########      ########");
        roomTemplate.push_back("##########       #######");
        roomTemplate.push_back("#########           ####");
        roomTemplate.push_back("#####          e      ##");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("#                       ");
        roomTemplate.push_back("#       e            ###");
        roomTemplate.push_back("                   #####");
        roomTemplate.push_back("                ########");
        roomTemplate.push_back("               #########");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("         ###############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    
    // Lullin Village
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("wwwwww   wwwwwwwwwwwwwww");
        roomTemplate.push_back("wwwwww   wwwwwwwwwwwwwww");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("   #######      ####### ");
        roomTemplate.push_back("   #######      ####### ");
        roomTemplate.push_back("   #######      ####### ");
        roomTemplate.push_back("   #######      ####### ");
        roomTemplate.push_back("   ###o###      ###o### ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("#################      #");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAA
        specificObjects.emplace(Coordinate(6, 9), new HouseDoorObject(pgame, Coordinate(6, 9), Coordinate(-100, -100), Coordinate(3, 4), dngutil::HOUSE_FLOOR));
        // ID: 0XAB    
        specificObjects.emplace(Coordinate(19, 9), new HouseDoorObject(pgame, Coordinate(19, 9), Coordinate(-99, -99), Coordinate(3, 4), dngutil::HOUSE_FLOOR));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(0, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }

    // Korloma Forest
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#########     0     ####");
        roomTemplate.push_back("#####                 ##");
        roomTemplate.push_back("           0            ");
        roomTemplate.push_back("      0  e       0      ");
        roomTemplate.push_back("#            0       ###");
        roomTemplate.push_back("#         0        #####");
        roomTemplate.push_back("##    0         ########");
        roomTemplate.push_back("####         0 #########");
        roomTemplate.push_back("#########v##############");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Forest Entrance";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########0    0       0 #");
        roomTemplate.push_back("#####           e0      ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("        0     0     0   ");
        roomTemplate.push_back("#   0                   ");
        roomTemplate.push_back("#         0    0   0  0 ");
        roomTemplate.push_back("##          e           ");
        roomTemplate.push_back("####   0     0      0   ");
        roomTemplate.push_back("#######        0  0     ");
        roomTemplate.push_back("#########             0 ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########               ");
        roomTemplate.push_back("###########      0      ");
        roomTemplate.push_back("#############           ");
        roomTemplate.push_back("########       0    0   ");
        roomTemplate.push_back("#####     0       e     ");
        roomTemplate.push_back("####          0      0  ");
        roomTemplate.push_back("####   0          0     ");
        roomTemplate.push_back("###         0           ");
        roomTemplate.push_back("#   0     0         0   ");
        roomTemplate.push_back("##     e       0        ");
        roomTemplate.push_back("####      0             ");
        roomTemplate.push_back("#######          0   0  ");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest";
        Coordinate mapCoord(1, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("        0      0        ");
        roomTemplate.push_back("  0                0    ");
        roomTemplate.push_back("        0      0     0  ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    0   0      0        ");
        roomTemplate.push_back("                    0   ");
        roomTemplate.push_back(" 0      0      0        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("         ##  ##       0 ");
        roomTemplate.push_back("    0   ###  ###  0     ");
        roomTemplate.push_back("     ######  ######     ");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest";
        Coordinate mapCoord(2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back(" 0        0    0     ###");
        roomTemplate.push_back("    0               ####");
        roomTemplate.push_back("         0    0   ######");
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back("    0      e     #######");
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back("          0   0   ######");
        roomTemplate.push_back(" 0                 #####");
        roomTemplate.push_back("        e           ####");
        roomTemplate.push_back("      0     0    0  ####");
        roomTemplate.push_back("                     ###");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest";
        Coordinate mapCoord(3, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########0 ##############");
        roomTemplate.push_back(" ####       ############");
        roomTemplate.push_back(" 0      0       ########");
        roomTemplate.push_back("    0               ####");
        roomTemplate.push_back(" 0     0   0 0     0    ");
        roomTemplate.push_back("    e                 0 ");
        roomTemplate.push_back("    0   0      0    0   ");
        roomTemplate.push_back("  0        0      e     ");
        roomTemplate.push_back("      0         0   0   ");
        roomTemplate.push_back("  0          0          ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("   0  0     0    #######");
        roomTemplate.push_back("                  ######");
        roomTemplate.push_back("  0     00    0     ####");
        roomTemplate.push_back("      0              ###");
        roomTemplate.push_back("  0         00    0   ##");

        std::map<Coordinate, MapObject*> specificObjects;

std::vector<dngutil::TID> possibleCreatures;
possibleCreatures.push_back(dngutil::TID::Bowman);

int difficulty = 1;
int backColor = dngutil::GREEN;
std::string name = "The Korloma Forest";
Coordinate mapCoord(3, 1);
RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
mut.lock();
gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
mut.unlock();
    }

    tfloor = 1;
    // Random Altar caves
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#### ^ #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("#      #################");
        roomTemplate.push_back("# +    #################");
        roomTemplate.push_back("#      #################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Altar Room";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######           ######");
        roomTemplate.push_back("#####     +        #####");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("###   e             ####");
        roomTemplate.push_back("##           e     #####");
        roomTemplate.push_back("##                ######");
        roomTemplate.push_back("###              #######");
        roomTemplate.push_back("#####          #########");
        roomTemplate.push_back("######## ^   ###########");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Altar Cave";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();
    }
}

void Map::makeHouses(std::mutex& mut)
{
    unsigned int tfloor = dngutil::HOUSE_FLOOR;
    {
        // ID: 0XAA
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("###o###");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 4), new HouseDoorObject(pgame, Coordinate(3, 4), Coordinate(0, -2), Coordinate(6, 9), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-100, -100);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAA
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#E    #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("###o###");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 4), new HouseDoorObject(pgame, Coordinate(3, 4), Coordinate(0, -2), Coordinate(19, 9), 2));

        std::vector<ColorString> possibleDialogue;
        possibleDialogue.push_back(ColorString("Strange purple men in armor have begun gathering around the Tower of the Gods", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(1, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(1, 1),
                "Thomas Westenhoffer",
                possibleDialogue
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-99, -99);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
}

void Map::makeForestTemple(std::mutex& mut)
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###### + ##  ###########");
        roomTemplate.push_back("######   ##  ###########");
        roomTemplate.push_back("######       ###########");
        roomTemplate.push_back("######       ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  1  v#######");
        roomTemplate.push_back("########################");
        // activation zone = (15, 11)
        // wall spot = 14, 11
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[11][15].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[11][14].push_back(new WallObject(pgame, Coordinate(14, 11)));
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 1), new Waterboots(pgame, Coordinate(1, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Forest Temple Entrance";
        Coordinate mapCoord(2, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
        mut.unlock();
    }

    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######v        ^#######");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Stairwell";
        Coordinate mapCoord(2, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }

    tfloor = 0;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############-###########");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   # o#  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   #  #  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("     #  #  # #  #  #    ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   #  #  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("######  #  # #  #  #  ##");
        roomTemplate.push_back("###### ^#             ##");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 2), new Potion(pgame, Coordinate(7, 2), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(2, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("  ######################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##  e              e  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("############ ###########");
        // 2 to 21
        // 1 to 7

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
            std::vector<Coordinate> deletionList;
            for (int x = 2; x <= 21; x++)
            {
                for (int y = 1; y <= 7; y++)
                {
                    deletionList.push_back(Coordinate(x, y));
                }
            }
            playSound(WavFile("UnlockDoor", true, true));
            for (auto i : deletionList)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
                Sleep(50);
                pgame->getVWin()->txtmacs.displayGame(pgame);
            }
            stopSound(SoundType::WAV);
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("############      w   ##");
        roomTemplate.push_back("############      we  ##");
        roomTemplate.push_back("############      wwww##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("############      wwww##");
        roomTemplate.push_back("############      we  ##");
        roomTemplate.push_back("############      w   ##");
        roomTemplate.push_back("############ ###########");
        // 2 to 21
        // 1 to 7

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
            gameMap[6][20].push_back(new Secondary(
                pgame,
                ColorChar('o', dngutil::LIGHTGREEN),
                Coordinate(20, 6),
                "Leaf Shield",
                false,
                dngutil::TID::Secondary,
                random(150, 175),
                .8,
                "A giant leaf used for protection"
            ));
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -2;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##o       ####        ##");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("          ####          ");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("##        ####        ##");
        roomTemplate.push_back("##        wwww        ##");
        roomTemplate.push_back("##        wwww        ##");
        roomTemplate.push_back("##        wwww        ##");
        roomTemplate.push_back("########################");
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new MagicalPotion(pgame, Coordinate(2, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(1, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##    o                 ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[3][6].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[3][3].push_back(new HarpPiece(pgame, Coordinate(3, 3), 1));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::ForestDragonPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::ForestDragonPhase2)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 3), new SegbossTrigger(
            pgame, Coordinate(6, 3),
            new Segboss(bossparts, pgame),
            ColorChar('S', dngutil::GREEN)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -4;
        int backColor = dngutil::BLACK;
        std::string name = "The Forest Temple - Boss's Lair";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("BossRoomTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##         wwwwwwwwwww##");
        roomTemplate.push_back("## wwwwww    wwwwwwwww##");
        roomTemplate.push_back("##      w  e    wwwwww##");
        roomTemplate.push_back("##wwwww wwww       www##");
        roomTemplate.push_back("##wwwww wwwwwwwww  www##");
        roomTemplate.push_back("           wwwww   w    ");
        roomTemplate.push_back("##wwwwww w w     www  ##");
        roomTemplate.push_back("##     e w w wwwww   w##");
        roomTemplate.push_back("##  wwwwww w    e  www##");
        roomTemplate.push_back("##       www wwwwwwwww##");
        roomTemplate.push_back("##wwwwww     wwwwwwwww##");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(3, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######  #########  #####");
        roomTemplate.push_back("##         ###        ##");
        roomTemplate.push_back("##         ###        ##");
        roomTemplate.push_back("##         ###        ##");
        roomTemplate.push_back("##         ###        ##");
        roomTemplate.push_back("##    e    ###        ##");
        roomTemplate.push_back("           ###        ##");
        roomTemplate.push_back("##############        ##");
        roomTemplate.push_back("##############        ##");
        roomTemplate.push_back("##############        ##");
        roomTemplate.push_back("############## o    o ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(15, 10), new Potion(pgame, Coordinate(15, 10), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(20, 10), new Primary(
            pgame,
            ColorChar('i', dngutil::WHITE),
            Coordinate(20, 10),
            "Rapier",
            false,
            1.1,
            3,
            90,
            false,
            "A very sharp sword",
            WavFile("Attack1", false, false),
            dngutil::ClassType::KNIGHT
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(4, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("            #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("##          #         ##");
        roomTemplate.push_back("######  #########  #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(4, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##        e             ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("########################");
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
            gameMap[6][2].push_back(new Waterboots(pgame, Coordinate(2, 6)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SkeletonKing);

        int difficulty = -3;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(3, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("          wwwww         ");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("##        wwwww       ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(1, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   e                ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##             e        ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   e                ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        // key spawn location = (5,6)
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
            gameMap[6][5].push_back(new Key(pgame, Coordinate(5, 6)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = -1;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(0, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
        mut.unlock();
    }
}

Map::~Map()
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