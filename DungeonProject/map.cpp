#include <array>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <cmath>
#include <functional>

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

    for (auto& i : functions)
    {
        i.join();
    }
}

void Map::makeOverworld(std::mutex& mut)
{
    unsigned int tfloor = 2;
    pgame->setActiveFloor(tfloor); // sets starting floor
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets starting active room
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
    }
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("ForestTheme")));
    }

    tfloor = 1;
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
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