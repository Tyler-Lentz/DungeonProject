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
    functions.emplace_back([&]() {makeForestTemple(mut); });
    for (auto& i : functions)
    {
        i.join();
    }
}

void Map::makeOverworld(std::mutex& mut)
{
    unsigned int tfloor = 2;
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

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));

        mut.lock();
        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets starting active room
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
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    # ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##  e              e  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("############ ###########");
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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[6][22].back());

            if (wall == nullptr)
            {
                errorMessage("error in puzzle", __LINE__, __FILE__);
            }

            gameMap[6][22].remove(wall);
            wall->removeFromMap(true);
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
        roomTemplate.push_back("##        ####        ##");
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
            WallObject* wall = dynamic_cast<WallObject*>(gameMap[6][0].back());

            if (wall == nullptr)
            {
                errorMessage("error in puzzle", __LINE__, __FILE__);
            }

            gameMap[6][0].remove(wall);
            wall->removeFromMap(true);

            wall = dynamic_cast<WallObject*>(gameMap[6][1].back());

            if (wall == nullptr)
            {
                errorMessage("error in puzzle", __LINE__, __FILE__);
            }

            gameMap[6][1].remove(wall);
            wall->removeFromMap(true);
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