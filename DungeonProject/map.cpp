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
#include "equipment.h"

Map::Map(Game* game)
{
    pgame = game;

    gamespace.resize(15);

    std::mutex mut;
    std::vector<std::thread> functions;
    functions.emplace_back([&]() {makeOverworld(mut); });
    functions.emplace_back([&]() {makeForestTemple(mut); });
    functions.emplace_back([&]() {makeHouses(mut); });
    functions.emplace_back([&]() {makeSpiritTemple(mut); });
    functions.emplace_back([&]() {makeWaterTemple(mut); });
    functions.emplace_back([&]() {makeFireTemple(mut); });
    functions.emplace_back([&]() {makeTowerOfTheGods(mut); });
    functions.emplace_back([&]() {makeHerosTrial(mut); });
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
        std::string name = "Beginning";
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
        possibleCreatures.push_back(dngutil::TID::Robber);

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
        possibleCreatures.push_back(dngutil::TID::Robber);

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
        possibleCreatures.push_back(dngutil::TID::Robber);

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
        roomTemplate.push_back("################     ###");
        roomTemplate.push_back("#############         ##");
        roomTemplate.push_back("############          ##");
        roomTemplate.push_back("###wwwww####          ##");
        roomTemplate.push_back("##wwwwwwww###         ##");
        roomTemplate.push_back("##wwwwwwwiw###         #");
        roomTemplate.push_back("##wwwwwwwwww###         ");
        roomTemplate.push_back("###wwww      ####       ");
        roomTemplate.push_back("###            #########");
        roomTemplate.push_back("####     e       #######");
        roomTemplate.push_back("#####                   ");
        roomTemplate.push_back("######                  ");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##        ##############");
        roomTemplate.push_back("## o     I #############");
        roomTemplate.push_back("##          ############");
        roomTemplate.push_back("###  o #################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 5), new Secondary(
            pgame,
            ColorChar('<', dngutil::WHITE),
            Coordinate(3, 5),
            "Pointy Shield",
            false,
            dngutil::TID::Secondary,
            200,
            1.05,
            "Shield made of steel with a sharp point."
        ));
        specificObjects.emplace(Coordinate(5, 7), new Sage(pgame, Coordinate(5, 7), new DragonBlessingSpell()));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BLUE;
        std::string name = "";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("##########      ########");
        roomTemplate.push_back("##########       #######");
        roomTemplate.push_back("#########           ####");
        roomTemplate.push_back("#####          e      ##");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("##      e            ###");
        roomTemplate.push_back("##                 #####");
        roomTemplate.push_back("##              ########");
        roomTemplate.push_back("##             #########");
        roomTemplate.push_back("#           ############");
        roomTemplate.push_back("#        ###############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########          ######");
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("###         e        ###");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("           ####         ");
        roomTemplate.push_back("         #######        ");
        roomTemplate.push_back("       ##########       ");
        roomTemplate.push_back("  e   ############      ");
        roomTemplate.push_back("        #######         ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("####        ####     ###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-2, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####        ####     ###");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##        wwwwww        ");
        roomTemplate.push_back("###   e  wwwwwwww       ");
        roomTemplate.push_back("##       wwwwwwwww      ");
        roomTemplate.push_back("##        wwwwwww       ");
        roomTemplate.push_back("#          wwwww        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                   e    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("################     ###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "";
        Coordinate mapCoord(-2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }

    // Boranian Desert
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("      e                #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("#################       ");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-3, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("      e                 ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("           e            ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-4, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("        e               ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("            ####        ");
        roomTemplate.push_back("          #######       ");
        roomTemplate.push_back("        ############    ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-5, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##    e                 ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("####        e           ");
        roomTemplate.push_back("######                  ");
        roomTemplate.push_back("##########              ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-6, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("      e               E ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                       #");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(22, 6), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(22, 6),
            "Trey Hidalgo",
            ColorString("The desert is inhabited by dangerous goblins.", dngutil::WHITE)
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 7;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-3, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                e       ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 8;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-4, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("      E wwwwwww         ");
        roomTemplate.push_back("       wwwwwwwwwww      ");
        roomTemplate.push_back("        wwwwwwwww       ");
        roomTemplate.push_back("        wwwwwwwww       ");
        roomTemplate.push_back("       wwwwwwwww        ");
        roomTemplate.push_back("                E       ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.front()->getPGame()->getPlayer()->getLvl() >= 20)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            Creature* creature = pgame->generateCreature(1, dngutil::TID::DesertGryphon);
            creature->setPosition(Coordinate(12, 3));
            gameMap[3][12].push_back(creature);
            pgame->getActiveRoom()->addCreature(creature, Coordinate(12, 3));
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 4), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(6, 4),
            "Danny Hernandez",
            ColorString("I'm here collecting water for Desert Town, What are you doing?", dngutil::WHITE)
        ));

        specificObjects.emplace(Coordinate(16, 9), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(16, 9),
            "George Clamarl",
            ColorString("Legends say if a powerful person comes here a beast will challenge them.", dngutil::WHITE)
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = -999;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert Oasis";
        Coordinate mapCoord(-5, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Great strength is required"), Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##     e                ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##      e               ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                 e    ");
        roomTemplate.push_back("##                      ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 9;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-6, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                 e    ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-3, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                  e     ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-4, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("          ###           ");
        roomTemplate.push_back("         ######         ");
        roomTemplate.push_back("          ######        ");
        roomTemplate.push_back("           #####        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("      e                 ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 9;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&E                     ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("&&E                     ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 3), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(2, 3),
            "Rene Paragas",
            ColorString("I'm a guard for Desert Town, I have to make sure no Goblins come in.", dngutil::WHITE),
            false
        ));
        specificObjects.emplace(Coordinate(2, 6), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(2, 6),
            "George Serrano",
            ColorString("Being a guard is tough, I'm parched and hungry!", dngutil::WHITE),
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 3;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town Gate";
        Coordinate mapCoord(-6, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("             ###########");
        roomTemplate.push_back("              ##########");
        roomTemplate.push_back("               #########");
        roomTemplate.push_back("               #########");
        roomTemplate.push_back("         e       #######");
        roomTemplate.push_back("                    ####");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-3, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    e                   ");
        roomTemplate.push_back("                   ###  ");
        roomTemplate.push_back("               #######  ");
        roomTemplate.push_back("                 #####  ");
        roomTemplate.push_back("                    ##  ");
        roomTemplate.push_back("           e            ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 10;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-4, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("#########    ###########");
        roomTemplate.push_back("########  ee  ##########");
        roomTemplate.push_back("      #        #        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 11;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-5, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&  e                   ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&  e                   ");
        roomTemplate.push_back("&&                      ");
        roomTemplate.push_back("&&                      ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = 9;
        int backColor = dngutil::BROWN;
        std::string name = "Boranian Desert";
        Coordinate mapCoord(-6, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }

    // Desert Cave
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("########## e############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########e ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 11;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Desert Cave";
        Coordinate mapCoord(-5, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####&&&&&& &&&&&&#######");
        roomTemplate.push_back("######&&&& &&&&#########");
        roomTemplate.push_back("########&& &&###########");
        roomTemplate.push_back("#########&3&############");
        roomTemplate.push_back("########## #############");
        roomTemplate.push_back("######     #############");
        roomTemplate.push_back("###### +   #############");
        roomTemplate.push_back("######     #############");
        roomTemplate.push_back("########## #############");
        roomTemplate.push_back("##########e#############");
        roomTemplate.push_back("########## #############");
        roomTemplate.push_back("########## #############");
        roomTemplate.push_back("##########  ############");
        // STEP ON 10,1 PLACE ON 10,2
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[1][10].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[2][10].push_back(new WallObject(pgame, Coordinate(12, 11)));
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 11;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Desert Cave";
        Coordinate mapCoord(-5, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("CaveTheme")));
        mut.unlock();

    }

    // Desert Town
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("         E            &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  &&o&&    &&o&&      &&");
        roomTemplate.push_back("  &&&&&    &&&&&      &&");
        roomTemplate.push_back("  &&&&&    &&&&&      &&");
        roomTemplate.push_back("  &&&&&    &&&&&      &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAL
        specificObjects.emplace(Coordinate(4, 6), new HouseDoorObject(pgame, Coordinate(4, 6), Coordinate(500, 0), Coordinate(2, 0), dngutil::HOUSE_FLOOR));
        // ID: 0XAM
        specificObjects.emplace(Coordinate(13, 6), new HouseDoorObject(pgame, Coordinate(13, 6), Coordinate(499, 0), Coordinate(2, 0), dngutil::HOUSE_FLOOR));


        specificObjects.emplace(Coordinate(9, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(9, 2),
            "Thomas Kellog",
            ColorString("There is a girl that I like, should I sneak up on her and confess my love?", dngutil::WHITE),
            true
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 3;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-7, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("   &&&&&&&&&&&        &&");
        roomTemplate.push_back("   &&&&&&&&&&&        &&");
        roomTemplate.push_back("   &&&&&&&&&&&        &&");
        roomTemplate.push_back("   &&o&&&&&&&&        &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("           E          &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("                      &&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 9), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(11, 9),
            "Joshua Ghoslan",
            ColorString("Desert Town is the last thriving area in Bora.", dngutil::WHITE),
            true
        ));
        // ID: 0XAN
        specificObjects.emplace(Coordinate(5, 7), new HouseDoorObject(pgame, Coordinate(5, 7), Coordinate(488, 0), Coordinate(2, 3), dngutil::HOUSE_FLOOR));


        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-7, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("   E                   #");
        roomTemplate.push_back("         ww ww          ");
        roomTemplate.push_back("        ww& &ww         ");
        roomTemplate.push_back("        w&& &&w         ");
        roomTemplate.push_back("           +            ");
        roomTemplate.push_back("        w&& &&w         ");
        roomTemplate.push_back("        ww& &ww         ");
        roomTemplate.push_back("         ww ww     E    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I am currently training for the 800 meter desert race.", dngutil::WHITE));
        specificObjects.emplace(Coordinate(3, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(3, 2),
            "Kristian Rascon",
            dialogue,
            true
        ));
        dialogue.clear();
        dialogue.push_back(ColorString("Hey kid, keep it between you and me but I'm planning a revolution.", dngutil::WHITE));
        dialogue.push_back(ColorString("The king is corrupt and does not care about his kingdom or people.", dngutil::WHITE));
        specificObjects.emplace(Coordinate(19, 9), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(19, 9),
            "Vladimir Lenin",
            dialogue,
            true
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-8, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("     E                  ");
        roomTemplate.push_back("   &&o&&       &&o&&    ");
        roomTemplate.push_back("   &&&&&       &&&&&    ");
        roomTemplate.push_back("   &&&&&       &&&&&    ");
        roomTemplate.push_back("   &&&&&       &&&&&    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 5), new HouseDoorObject(pgame, Coordinate(5, 5), Coordinate(-8, -3), Coordinate(0, 0), 2));

        // ID: 0XAQ
        specificObjects.emplace(Coordinate(17,5), new HouseDoorObject(pgame, Coordinate(17,5), Coordinate(1200,1), Coordinate(2, 0), dngutil::HOUSE_FLOOR));


        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("This person is sick with the plague, and I'm making sure it doesn't spread.", dngutil::WHITE));
        dialogue.push_back(ColorString("Its strange, we thought the plague went away centuries ago...", dngutil::WHITE));
        specificObjects.emplace(Coordinate(5, 4), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(5, 4),
            "Tyrone Solano",
            dialogue,
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##    &&&&&             ");
        roomTemplate.push_back("##    &&&&&   E         ");
        roomTemplate.push_back("##    &&&&&             ");
        roomTemplate.push_back("##    &&o&&             ");
        roomTemplate.push_back("##            &&o&&     ");
        roomTemplate.push_back("##            &&&&&     ");
        roomTemplate.push_back("##            &&&&&     ");
        roomTemplate.push_back("##            &&&&&     ");
        roomTemplate.push_back("##                      ");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAO
        specificObjects.emplace(Coordinate(8, 7), new HouseDoorObject(pgame, Coordinate(8, 7), Coordinate(760, 0), Coordinate(2, 3), dngutil::HOUSE_FLOOR));
        // ID: 0XAP
        specificObjects.emplace(Coordinate(16, 8), new HouseDoorObject(pgame, Coordinate(16, 8), Coordinate(22, 22), Coordinate(2, 0), dngutil::HOUSE_FLOOR));



        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I have never left the town because they say its not safe out there.", dngutil::WHITE));
        specificObjects.emplace(Coordinate(14, 5), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(14, 5),
            "Alex Golvado",
            dialogue,
            true
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-9, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("###    &&&&&&&o&&       ");
        roomTemplate.push_back("####   &&&&&&&&&&       ");
        roomTemplate.push_back("#####  &&&&&&&&&&       ");
        roomTemplate.push_back("###### &&&&&&&&&&       ");
        roomTemplate.push_back("#######                 ");
        roomTemplate.push_back("#########               ");
        roomTemplate.push_back("#############           ");
        roomTemplate.push_back("###############         ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAR
        specificObjects.emplace(Coordinate(14, 3), new HouseDoorObject(pgame, Coordinate(14, 3), Coordinate(333, 322), Coordinate(7, 0), dngutil::HOUSE_FLOOR));
        

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "Desert Town";
        Coordinate mapCoord(-9, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##W&&&&&WWWWWWWW&&&&&W##");
        roomTemplate.push_back("##W&&&&&WWWWWWWW&&&&&W##");
        roomTemplate.push_back("##W&&&&&&&&&&&&&&&&&&W##");
        roomTemplate.push_back("##WWWWW&&&&&&&&&&WWWWW##");
        roomTemplate.push_back("#WWwWWW&&&&&&&&&&WWWWWW#");
        roomTemplate.push_back("#WW&&&&&&&&v&&&&&&&&&WW#");
        roomTemplate.push_back("#WW&&&&&   E    &&&&&WW#");
        roomTemplate.push_back("#WW&&&&&        &&&&&WW#");
        roomTemplate.push_back("#                      #");
        roomTemplate.push_back("#                      #");
        //11,9 ->12, 9
        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (pgame->getPlayer()->hasHarpPiece(3)); // fire temple
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
           /* Npc* wall = dynamic_cast<Npc*>(gameMap[9][11].back());

            if (wall == nullptr)
            {
                errorMessage("error in puzzle", __LINE__, __FILE__);
            }

            gameMap[9][11].remove(wall);
            wall->removeFromMap(true);*/
            
            Npc* guard = dynamic_cast<Npc*>(gameMap[9][11].back());
            guard->adjustPosition(dngutil::Movement::RIGHT);
            guard->changeDialogue(ColorString("Thank you for defeating the Fire Dragon! The king would like to see you.", dngutil::WHITE));
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 9), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(11, 9),
            "Guard",
            ColorString("The king cannot endanger himself outside while the Fire Dragon runs rampant.", dngutil::WHITE),
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::BROWN;
        std::string name = "New Bora Castle";
        Coordinate mapCoord(-8, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Defeat the fire Dragon."), Mp3File("VillageTheme")));
        mut.unlock();

    }
    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&     ^      &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Royal Chamber";
        Coordinate mapCoord(-8, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&        &&&&&&&&");
        roomTemplate.push_back("&&&&&&     E      &&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  && o          &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  &&            &&  &&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 4), new Sage(pgame, Coordinate(7, 4), new DragonShieldSpell()));

        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("We are forever in debt to you good sir!", dngutil::WHITE));
        dialogue.push_back(ColorString("My royal advisor over there has a reward for you.", dngutil::WHITE));
        specificObjects.emplace(Coordinate(11, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::YELLOW),
            Coordinate(11, 2),
            "King",
            dialogue,
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 4;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Royal Chamber";
        Coordinate mapCoord(-8, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    tfloor++;

    // Glorian Plains
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##       ###############");
        roomTemplate.push_back("#           ############");
        roomTemplate.push_back("         E  ############");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("             ###########");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("  e        #############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("           #############");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I was traveling with my dad when we got separated.", dngutil::WHITE));
        dialogue.push_back(ColorString("Have you seen him? No? Thats a shame...", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(9, 2),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(9, 2),
                "Billy Wadkins",
                dialogue
            ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-1, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                  e     ");
        roomTemplate.push_back("          ##            ");
        roomTemplate.push_back("  e      ####           ");
        roomTemplate.push_back("           ###          ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("      #########         ");
        roomTemplate.push_back("        #v####          ");
        roomTemplate.push_back("          ####          ");
        roomTemplate.push_back("           ##           ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    e                   ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-3, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#####  ########  #######");
        roomTemplate.push_back("###   #########        #");
        roomTemplate.push_back("##      #^####    +    #");
        roomTemplate.push_back("#         ####         #");
        roomTemplate.push_back("##         ##          #");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwwww#");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwwww#");
        roomTemplate.push_back("#wwwwwwwwwwwwwwwwwwwwww#");
        roomTemplate.push_back("#wwwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("#####wwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("#######wwwwwwwwwwwww####");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Underground Lake";
        Coordinate mapCoord(-3, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();
    }
    tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("             ###########");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("           #############");
        roomTemplate.push_back("          ##############");
        roomTemplate.push_back("          ##############");
        roomTemplate.push_back("        ################");
        roomTemplate.push_back("        ################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("      ##################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-1, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-2, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                  e     ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("     e                  ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Glorian Plains";
        Coordinate mapCoord(-3, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }


    // Glorian Beach
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("      ##################");
        roomTemplate.push_back("      ##################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("        ################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("wwwwww##################");
        roomTemplate.push_back("wwwwww##################");
        roomTemplate.push_back("wwwwww##################");
        roomTemplate.push_back("WWWWWWW#################");
        roomTemplate.push_back("WWWWWWW#################");
        roomTemplate.push_back("WWWWWWW#################");
        roomTemplate.push_back("WWWWWW##################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Glorian Beach";
        Coordinate mapCoord(-1, 4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    e                   ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Glorian Beach";
        Coordinate mapCoord(-2, 4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##  E                   ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("#wwwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("WWWWWwwwwwwwwwwwwwwwWWWW");
        roomTemplate.push_back("WWWWWWwwwwwwwwwwwwwWWWWW");
        roomTemplate.push_back("WWWWWWWwwwwwwwwwwwWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I see something out in the distance - I can't quite make", dngutil::WHITE));
        dialogue.push_back(ColorString("it out as my eyesight is poor...", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(4, 4),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(4, 4),
                "Houstan Harold",
                dialogue
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Glorian Beach";
        Coordinate mapCoord(-3, 4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }

    // Castle Town Ruins
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                & &&    ");
        roomTemplate.push_back("                &e &    ");
        roomTemplate.push_back("                &  &    ");
        roomTemplate.push_back("   &&&     &    &&&&    ");
        roomTemplate.push_back("  &                     ");
        roomTemplate.push_back("  &   &     &           ");
        roomTemplate.push_back("  &&  &          &      ");
        roomTemplate.push_back("              &&&   &   ");
        roomTemplate.push_back("       e      &   &&    ");
        roomTemplate.push_back("              &         ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Town Ruins";
        Coordinate mapCoord(-4, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    &&&&&               ");
        roomTemplate.push_back("         &              ");
        roomTemplate.push_back("    &                   ");
        roomTemplate.push_back("       &&               ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("          &   & &       ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("         &  e    &      ");
        roomTemplate.push_back("            &&&&&&      ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Town Ruins";
        Coordinate mapCoord(-5, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("  & &&&                 ");
        roomTemplate.push_back("  &   &         e       ");
        roomTemplate.push_back("     &                  ");
        roomTemplate.push_back("  && &&                 ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("            w ww        ");
        roomTemplate.push_back("    &      w& &w        ");
        roomTemplate.push_back("  &   &      +          ");
        roomTemplate.push_back("  & e &     &&&w        ");
        roomTemplate.push_back("   &       ww ww        ");
        roomTemplate.push_back("     &                  ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Town Ruins";
        Coordinate mapCoord(-4, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back(" &&          &&&        ");
        roomTemplate.push_back("                   &    ");
        roomTemplate.push_back(" &&&       &&      &    ");
        roomTemplate.push_back("              &&        ");
        roomTemplate.push_back("    &&                  ");
        roomTemplate.push_back("       &           &    ");
        roomTemplate.push_back("  &  e       &&&   &    ");
        roomTemplate.push_back("      &           &&    ");
        roomTemplate.push_back("   &                    ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Town Ruins";
        Coordinate mapCoord(-5, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    
    // Castle Ruins
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back(" 0  0  0   0   0   0    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("wwwwwwwwwwwwwwwww  0    ");
        roomTemplate.push_back("wwwwwwwwwwwwwwwww       ");
        roomTemplate.push_back("     &         ww  0    ");
        roomTemplate.push_back("&&      &      ww       ");
        roomTemplate.push_back("  &    &       ww  0    ");
        roomTemplate.push_back("     &    &    ww       ");
        roomTemplate.push_back("  &            ww  0    ");
        roomTemplate.push_back("    &    &  &  ww       ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Ruins";
        Coordinate mapCoord(-6, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##  0  0  0  0  0  0  0 ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##  0 wwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("##    wwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("##  0 ww                ");
        roomTemplate.push_back("##    ww   &&&  &   &&  ");
        roomTemplate.push_back("##  0 ww   &e           ");
        roomTemplate.push_back("##    ww     &   & &  & ");
        roomTemplate.push_back("##  0 ww    &      v&   ");
        roomTemplate.push_back("##    ww   &&&      &&  ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 5;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Ruins";
        Coordinate mapCoord(-7, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##    ww   &&&    &     ");
        roomTemplate.push_back("##  0 ww    &         & ");
        roomTemplate.push_back("##    ww      &    &&   ");
        roomTemplate.push_back("##  0 ww   &&   &&      ");
        roomTemplate.push_back("##    ww     &&         ");
        roomTemplate.push_back("##  0 wwwwwwwwwwwwwwww  ");
        roomTemplate.push_back("##    wwwwwwwwwwwwwwww  ");
        roomTemplate.push_back("##  0                   ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##  0  0  0  0  0   0   ");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 5;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Ruins";
        Coordinate mapCoord(-7, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("     && &      ww       ");
        roomTemplate.push_back("   &           ww  0    ");
        roomTemplate.push_back("      e   &    ww       ");
        roomTemplate.push_back("  &   &   &&&  ww       ");
        roomTemplate.push_back("        &      ww  0    ");
        roomTemplate.push_back("  wwwwwwwwwwwwwww       ");
        roomTemplate.push_back("  wwwwwwwwwwwwwww       ");
        roomTemplate.push_back("                   0    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    0  0  0  0  0  0    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Robber);

        int difficulty = 3;
        int backColor = dngutil::GREEN;
        std::string name = "Castle Ruins";
        Coordinate mapCoord(-6, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }

    // Castle Dungeon
    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&   &   &   &&&&&&&&");
        roomTemplate.push_back("&&&&& e & e & e &&   &&&");
        roomTemplate.push_back("&&&&&   &   &   && + &&&");
        roomTemplate.push_back("&&&&&& &&& &&& &&&   &&&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("                      &&");
        roomTemplate.push_back("&&&&&& &&& &&& &&     &&");
        roomTemplate.push_back("&&&&&   &   &   &     &&");
        roomTemplate.push_back("&&&&& e & e & e &     &&");
        roomTemplate.push_back("&&&&&   &   &   &  ^  &&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = -999;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Castle Dungeon";
        Coordinate mapCoord(-7, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&ww&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&wwwwww&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&wwwwwwww&&&&&&&&&&&&");
        roomTemplate.push_back("&&&ww                   ");
        roomTemplate.push_back("&&&wwe                  ");
        roomTemplate.push_back("&&&&wwwwwwww&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&wwwwww&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&ww&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Basilisk);

        int difficulty = -999;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Basilisk's Chamber";
        Coordinate mapCoord(-8, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();
    }
    tfloor = 2;

    // Arkala Pass
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########wwwwwwwwwww#####");
        roomTemplate.push_back("########wwwwwwwwwwww####");
        roomTemplate.push_back("########wwwwwwwwwww#####");
        roomTemplate.push_back("#######wwwwwwwwwww######");
        roomTemplate.push_back("######wwwwwwwwwww#######");
        roomTemplate.push_back("####wwwwwwwwwwwww#######");
        roomTemplate.push_back("#####wwwwwwwwww ########");
        roomTemplate.push_back("#####wwwwwww     #######");
        roomTemplate.push_back("######wwww    E  #######");
        roomTemplate.push_back("########          ######");
        roomTemplate.push_back("#######            #####");
        roomTemplate.push_back("######            ######");
        roomTemplate.push_back("#######           ######");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("If you kept going north you'd eventually reach Arkala Village", dngutil::WHITE));
        dialogue.push_back(ColorString("but as you can see, its flooded at the moment. ", dngutil::WHITE));
        specificObjects.emplace(Coordinate(14, 8), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(14, 8),
            "Glorian Hall",
            dialogue
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Pass";
        Coordinate mapCoord(-2, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########wwwww###########");
        roomTemplate.push_back("########wwwwww##########");
        roomTemplate.push_back("####    wwwwwww#########");
        roomTemplate.push_back("#### e wwwwwww##########");
        roomTemplate.push_back("####  wwwwww############");
        roomTemplate.push_back("########wwwwww     #####");
        roomTemplate.push_back("########wwwwwwww e #####");
        roomTemplate.push_back("#####wwwwwwwwwwww  #####");
        roomTemplate.push_back("######wwwwwwwwwww#######");
        roomTemplate.push_back("########wwwwwwwwww######");
        roomTemplate.push_back("#######wwwwwwwwwww######");
        roomTemplate.push_back("######wwwwwwwwwwww######");
        roomTemplate.push_back("########wwwwwwwwwww#####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Pass";
        Coordinate mapCoord(-2, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########wwwwwwwwwww#####");
        roomTemplate.push_back("#########wwwwwwwww######");
        roomTemplate.push_back("##########wwwwwwww######");
        roomTemplate.push_back("############wwwwww######");
        roomTemplate.push_back("#####     wwwwwww#######");
        roomTemplate.push_back("###v    wwwwwwwwww######");
        roomTemplate.push_back("####    wwwwwwwwww######");
        roomTemplate.push_back("##### ####wwwwwwwww#####");
        roomTemplate.push_back("###########wwwwwwww#####");
        roomTemplate.push_back("########wwwwwwwwwww#####");
        roomTemplate.push_back("#######wwwwwwwwwww######");
        roomTemplate.push_back("######wwwwwwwwwwww######");
        roomTemplate.push_back("########wwwwwwwwwww#####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Pass";
        Coordinate mapCoord(-2, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#                       ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("####                  ##");
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("########           #####");
        roomTemplate.push_back("########wwwwwwww   #####");
        roomTemplate.push_back("##########wwwwwwwww#####");
        roomTemplate.push_back("###########wwwwww#######");
        roomTemplate.push_back("########wwwwwwwwww######");
        roomTemplate.push_back("#######wwwwwwwwwww######");
        roomTemplate.push_back("######wwwwwwwwwwww######");
        roomTemplate.push_back("########wwwwwwwwwww#####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Pass";
        Coordinate mapCoord(-2, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#####           ########");
        roomTemplate.push_back("###^         o    ######");
        roomTemplate.push_back("####             #######");
        roomTemplate.push_back("#####            #######");
        roomTemplate.push_back("########################"); 
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        // 13, 5
        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(13, 5), new Sage(pgame, Coordinate(13, 5), new SpiritRollerSpell()));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 6;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Arkala Pass Cave";
        Coordinate mapCoord(-2, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    tfloor = 2;

    // Arkala Village
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("     0          ######  ");
        roomTemplate.push_back("                ######  ");
        roomTemplate.push_back("     0          o#####  ");
        roomTemplate.push_back("       E        ######  ");
        roomTemplate.push_back("     0          ######  ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  ######          0     ");
        roomTemplate.push_back("  ######                ");
        roomTemplate.push_back("  #####o          0     ");
        roomTemplate.push_back("  ######                ");
        roomTemplate.push_back("  ######          0     ");
        roomTemplate.push_back("#                       ");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I'm lost... Have you seen my son - Billy?", dngutil::WHITE));
        dialogue.push_back(ColorString("No? I guess I'll keep looking...", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(7, 4),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(7, 4),
                "Tanner Santogrossi",
                dialogue
            ));
        // ID: 0XAH
        specificObjects.emplace(Coordinate(7, 9), new HouseDoorObject(pgame, Coordinate(7, 9), Coordinate(120, 120), Coordinate(5, 2), dngutil::HOUSE_FLOOR));
        // ID: 0XAI
        specificObjects.emplace(Coordinate(16, 3), new HouseDoorObject(pgame, Coordinate(16, 3), Coordinate(119, 119), Coordinate(0, 2), dngutil::HOUSE_FLOOR));


        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-2, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########              ");
        roomTemplate.push_back("######                  ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("##      0   0   0       ");
        roomTemplate.push_back("##  0  E           0    ");
        roomTemplate.push_back("##        wwwww         ");
        roomTemplate.push_back("##  0     wwwww    0    ");
        roomTemplate.push_back("##        wwwww         ");
        roomTemplate.push_back("##  0           E  0    ");
        roomTemplate.push_back("##      0   0    0      ");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("#######                 ");
        roomTemplate.push_back("########################");
        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("This fountain is so beautiful...", dngutil::WHITE));
        dialogue.push_back(ColorString("I could watch it all day.", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(7, 4),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(7, 4),
                "Tricia Hall",
                dialogue
            ));
        specificObjects.emplace(
            Coordinate(16, 8),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(16, 8),
                "Holly Hall",
                ColorString("We haven't been able to leave ever since the pass flooded...", dngutil::WHITE)
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-3, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#####################  0");
        roomTemplate.push_back("###################     ");
        roomTemplate.push_back("##################  0   ");
        roomTemplate.push_back("#################       ");
        roomTemplate.push_back("##############   0 E    ");
        roomTemplate.push_back("#############           ");
        roomTemplate.push_back("###########   #######   ");
        roomTemplate.push_back("###########   #######   ");
        roomTemplate.push_back("###########   ######o   ");
        roomTemplate.push_back("###########   #######   ");
        roomTemplate.push_back("##########              ");
        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("This town desparately needs supplies, but none can", dngutil::WHITE));
        dialogue.push_back(ColorString("come in because the only way - Arklaka Pass - is flooded.", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(19, 6),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(19, 6),
                "Chris Mosely",
                dialogue
            ));
        // ID: 0XAK
        specificObjects.emplace(Coordinate(20, 10), new HouseDoorObject(pgame, Coordinate(20, 10), Coordinate(117, 117), Coordinate(6, 2), dngutil::HOUSE_FLOOR));


        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-3, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("          ######        ");
        roomTemplate.push_back("           #######      ");
        roomTemplate.push_back("    wwwwwwwww####       ");
        roomTemplate.push_back("   wwwwwwwwwww####      ");
        roomTemplate.push_back("  wwwwwwwwwwwww###      ");
        roomTemplate.push_back("  wwwwwwwwwwwww####     ");
        roomTemplate.push_back("   wwwwwwwwwwwww##      ");
        roomTemplate.push_back("     wwwwwwwwww###      ");
        roomTemplate.push_back("              ###       ");
        roomTemplate.push_back("                        ");
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-2, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###################  ###");
        roomTemplate.push_back("###################::###");
        roomTemplate.push_back("      wwwwwwwwww###::###");
        roomTemplate.push_back("      wwwwwwwwwww :::###");
        roomTemplate.push_back("      wwwwwwwwww  :::###");
        roomTemplate.push_back("       wwwwwwwww   :####");
        roomTemplate.push_back("       wwwwwwwww########");
        roomTemplate.push_back("       wwwwwwwwww#######");
        roomTemplate.push_back("       wwwwwwwwwww######");
        roomTemplate.push_back("          wwwwwwww######");
        roomTemplate.push_back("                  ######");
        roomTemplate.push_back("                  ######");
        roomTemplate.push_back("                 #######");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAJ
        specificObjects.emplace(Coordinate(7, 4), new HouseDoorObject(pgame, Coordinate(7, 4), Coordinate(118, 118), Coordinate(3, 3), dngutil::HOUSE_FLOOR));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-1, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back(" 0  #######  0   #######");
        roomTemplate.push_back("    #######     ########");
        roomTemplate.push_back("    #######  0   #######");
        roomTemplate.push_back(" 0  ###o###     ########");
        roomTemplate.push_back("             0  ########");
        roomTemplate.push_back("                ########");
        roomTemplate.push_back("   0             #######");
        roomTemplate.push_back("        ww ww     ######");
        roomTemplate.push_back("   0    w# #w     ######");
        roomTemplate.push_back("          +     0 ######");
        roomTemplate.push_back("   0    w# #w     ######");
        roomTemplate.push_back("        ww ww    #######");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAJ
        specificObjects.emplace(Coordinate(7, 4), new HouseDoorObject(pgame, Coordinate(7, 4), Coordinate(118, 118), Coordinate(3,3), dngutil::HOUSE_FLOOR));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village";
        Coordinate mapCoord(-1, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                 #######");
        roomTemplate.push_back("                   #####");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("###                 ####");
        roomTemplate.push_back("####  U  U  U  U    ####");
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("##### U  U  U  U    ####");
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("##### U  U  U  U   #####");
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("######   U  U  U    ####");
        roomTemplate.push_back("#######             ####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village Cemetary";
        Coordinate mapCoord(-1, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("#####  U  U  U  U   ####");
        roomTemplate.push_back("#####               ####");
        roomTemplate.push_back("####                 ###");
        roomTemplate.push_back("###     #########    ###");
        roomTemplate.push_back("###     #########     ##");
        roomTemplate.push_back("###     #########     ##");
        roomTemplate.push_back("##      #########     ##");
        roomTemplate.push_back("###     ####o####     ##");
        roomTemplate.push_back("####                 ###");
        roomTemplate.push_back("######              ####");
        roomTemplate.push_back("#########www+wwwww######");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAG
        specificObjects.emplace(Coordinate(12, 8), new HouseDoorObject(pgame, Coordinate(12, 8), Coordinate(200, 200), Coordinate(4, 4), dngutil::HOUSE_FLOOR));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 6;
        int backColor = dngutil::GREEN;
        std::string name = "Arkala Village Cemetary";
        Coordinate mapCoord(-1, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }

    // Arkala Cave
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########    ############");
        roomTemplate.push_back("#######     ############");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("                ########");
        roomTemplate.push_back("###         e    #######");
        roomTemplate.push_back("########          ######");
        roomTemplate.push_back("##########        ######");
        roomTemplate.push_back("#############   e ######");
        roomTemplate.push_back("################  ######");
        roomTemplate.push_back("################    ####");
        roomTemplate.push_back("#################   ####");
        roomTemplate.push_back("##################   ###");
        roomTemplate.push_back("###################  ###");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Arkala Cave";
        Coordinate mapCoord(-1, -9);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########  #############");
        roomTemplate.push_back("#########  #############");
        roomTemplate.push_back("#########oo#############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########::::############");
        roomTemplate.push_back("########    ############");
        roomTemplate.push_back("########    ############");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(9, 2), new DungeonCheck(pgame, Coordinate(9, 2), 6));
        specificObjects.emplace(Coordinate(10, 2), new DungeonCheck(pgame, Coordinate(10, 2), 6));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 13;
        int backColor = dngutil::MAGENTA;
        std::string name = "Arkala Cave";
        Coordinate mapCoord(-1, -10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme"), false));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######      ###########");
        roomTemplate.push_back("######        ##########");
        roomTemplate.push_back("#####          #########");
        roomTemplate.push_back("####    xxxx    ########");
        roomTemplate.push_back("###    xxxxxx    #######");
        roomTemplate.push_back("###    xxxxxx    #######");
        roomTemplate.push_back("####    xxxx    ########");
        roomTemplate.push_back("#####          #########");
        roomTemplate.push_back("######        ##########");
        roomTemplate.push_back("#######      ###########");
        roomTemplate.push_back("########    ############");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.front()->getCoord().y < 12)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[12][8].push_back(new WallObject(pgame, Coordinate(8, 12)));
            gameMap[12][9].push_back(new WallObject(pgame, Coordinate(9, 12)));
            gameMap[12][10].push_back(new WallObject(pgame, Coordinate(10, 12)));
            gameMap[12][11].push_back(new WallObject(pgame, Coordinate(11, 12)));
        };

        // 8-11,12   
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 13;
        int backColor = dngutil::MAGENTA;
        std::string name = "Hero's Trial Entrance";
        Coordinate mapCoord(-1, -11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme"), false));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("###         ############");
        roomTemplate.push_back("####       #############");
        roomTemplate.push_back("#####  +  ##############");
        roomTemplate.push_back("######   ###############");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = 13;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Arkala Cave";
        Coordinate mapCoord(-2, -9);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
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
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("          www         ##");
        roomTemplate.push_back("wwwwwwwwwwwww         ##");
        roomTemplate.push_back("wwwwwwwwwwww          ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("    ########          ##");
        roomTemplate.push_back("    ########          ##");
        roomTemplate.push_back("    ########          ##");
        roomTemplate.push_back("    ###o####        ####");
        roomTemplate.push_back("                   #####");
        roomTemplate.push_back("                ########");
        roomTemplate.push_back("            ############");
        roomTemplate.push_back("          ##############");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAF
        specificObjects.emplace(Coordinate(7,7), new HouseDoorObject(pgame, Coordinate(7, 7), Coordinate(-95, -95), Coordinate(2, 3), dngutil::HOUSE_FLOOR));
        
        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("          www         ##");
        roomTemplate.push_back("          www  0      ##");
        roomTemplate.push_back(" 0        www         ##");
        roomTemplate.push_back("     0    www     0  ###");
        roomTemplate.push_back("          www       ####");
        roomTemplate.push_back("  0    0  www  0   #####");
        roomTemplate.push_back("          www        v##");
        roomTemplate.push_back("          www      #####");
        roomTemplate.push_back("    0     www       ####");
        roomTemplate.push_back("          www        ###");
        roomTemplate.push_back("       0  www   0     ##");
        roomTemplate.push_back("  0       www         ##");
        roomTemplate.push_back("          www         ##");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAF
        specificObjects.emplace(Coordinate(7, 7), new HouseDoorObject(pgame, Coordinate(7, 7), Coordinate(-95, -95), Coordinate(2, 3), dngutil::HOUSE_FLOOR));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  #############         ");
        roomTemplate.push_back("  #############  0      ");
        roomTemplate.push_back("  #############         ");
        roomTemplate.push_back("  #############    E    ");
        roomTemplate.push_back("  #############         ");
        roomTemplate.push_back("  ###o#########         ");
        roomTemplate.push_back("                    0   ");
        roomTemplate.push_back("          0             ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  0             0       ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;
        // ID: 0XAC
        specificObjects.emplace(Coordinate(5, 6), new HouseDoorObject(pgame, Coordinate(5, 6), Coordinate(-98, -98), Coordinate(3, 5), dngutil::HOUSE_FLOOR));
        specificObjects.emplace(Coordinate(19, 4), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(19, 4),
            "Collin Werth",
            ColorString("... I don't want to talk to anyone", dngutil::WHITE)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(0, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########                ");
        roomTemplate.push_back("######wwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("#####wwwwwwwwwwwwwwwwwww");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("                  E     ");
        roomTemplate.push_back("   www          www     ");
        roomTemplate.push_back("  ww#wwE  w w  ww#ww    ");
        roomTemplate.push_back("  w###w    +   w###w    ");
        roomTemplate.push_back("  ww#ww   w w  ww#ww    ");
        roomTemplate.push_back("   www          www     ");
        roomTemplate.push_back("                 E      ");
        roomTemplate.push_back("                        "); 
        roomTemplate.push_back("####           ####  ###");

        std::vector<ColorString> dialogue;

        std::map<Coordinate, MapObject*> specificObjects;

        dialogue.push_back(ColorString("Close-combat beats ranged. Ranged beats magic. Magic beats close-combat.", dngutil::WHITE));
        dialogue.push_back(ColorString("If you have the advantage, you may be able to attack twice.", dngutil::WHITE));

        specificObjects.emplace(Coordinate(7, 6), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(7, 6),
            "John Baker",
            dialogue
        ));

        specificObjects.emplace(Coordinate(17, 10), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(17, 10),
            "Josh Chan",
            ColorString("This altar used to be the site of many important rituals.", dngutil::WHITE)
        ));
        dialogue.clear();
        dialogue.push_back(ColorString("There used to be a ", dngutil::WHITE) + ColorString("temple", dngutil::TEXT_HIGHLIGHT_COLOR) + ColorString(" in the woods to the south,", dngutil::WHITE));
        dialogue.push_back(ColorString("but it got overrun when Zorlock attacked.", dngutil::WHITE));
        specificObjects.emplace(Coordinate(18, 4), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(18, 4),
            "Harold Schmitt",
            dialogue
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(-1, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#####################   ");
        roomTemplate.push_back("#####################   ");
        roomTemplate.push_back("####################    ");
        roomTemplate.push_back("###################     ");
        roomTemplate.push_back("#################    0  ");
        roomTemplate.push_back("###############         ");
        roomTemplate.push_back("###############         ");
        roomTemplate.push_back("#############     0   0 ");
        roomTemplate.push_back("###########    0        ");
        roomTemplate.push_back("###########             ");
        roomTemplate.push_back("##########    0     0   ");
        roomTemplate.push_back("########   0            ");
        roomTemplate.push_back("########                ");

        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(-1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####################### ");
        roomTemplate.push_back("######################  ");
        roomTemplate.push_back("######################  ");
        roomTemplate.push_back("######################  ");
        roomTemplate.push_back("#####################   ");

        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(-1, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########v##########www##");
        roomTemplate.push_back("#######   wwwwwwwwwww   ");
        roomTemplate.push_back("#######wwwwwwwwwww      ");
        roomTemplate.push_back("#####wwwwwwwww          ");
        roomTemplate.push_back("####wwwwwww    0   E    ");
        roomTemplate.push_back("####www                 ");
        roomTemplate.push_back("#####      ########     ");
        roomTemplate.push_back("        0  ########     ");
        roomTemplate.push_back(" ######    ########     ");
        roomTemplate.push_back(" #####o    #o######     ");
        roomTemplate.push_back(" ######              0  ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(19, 5), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(19, 5),
            "Terry Hints",
            ColorString("There is a ", dngutil::WHITE) + ColorString("cave", dngutil::TEXT_HIGHLIGHT_COLOR) + ColorString(" across the water, I wonder what's in it?", dngutil::WHITE)
        ));
        // ID: 0XAD
        specificObjects.emplace(Coordinate(6, 10), new HouseDoorObject(pgame, Coordinate(6, 10), Coordinate(-97, -97), Coordinate(5, 1), dngutil::HOUSE_FLOOR));
        // ID: 0XAE
        specificObjects.emplace(Coordinate(12, 10), new HouseDoorObject(pgame, Coordinate(12, 10), Coordinate(-96, -96), Coordinate(1, 3), dngutil::HOUSE_FLOOR));

        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(0, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }
    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########^###############");
        roomTemplate.push_back("#######        :::::####");
        roomTemplate.push_back("#######        ::o::####");
        roomTemplate.push_back("#######        ::::#####");
        roomTemplate.push_back("#########      ::::#####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(17, 3), new Primary(
            pgame,
            ColorChar('|', dngutil::BROWN),
            Coordinate(17, 3),
            "Rock Staff",
            false,
            1.4,
            3,
            75,
            false,
            "Magical staff that hurls rocks at opponents",
            WavFile("MagicAttack1", false, false),
            dngutil::ClassType::WIZARD
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Cave";
        Coordinate mapCoord(0, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("          ##############");
        roomTemplate.push_back("  0       www    #######");
        roomTemplate.push_back("          www 0    #####");
        roomTemplate.push_back("      0   www       ####");
        roomTemplate.push_back(" 0        www    0   ###");
        roomTemplate.push_back("          www        ###");
        roomTemplate.push_back("   0      www 0    0 ###");
        roomTemplate.push_back("          www         ##");
        roomTemplate.push_back("       0  www    0    ##");
        roomTemplate.push_back("          www         ##");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Village";
        Coordinate mapCoord(1, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();

    }

    tfloor = 1;
    // Lullin Tunnel
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########      ##########");
        roomTemplate.push_back("#######  www   #########");
        roomTemplate.push_back("######  wwwwww  ####   #");
        roomTemplate.push_back("#####  www+          ^  ");
        roomTemplate.push_back("######  wwwwww  ####   #");
        roomTemplate.push_back("#######  www   #########");
        roomTemplate.push_back("########      ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Tunnel";
        Coordinate mapCoord(1, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######wwwwwwwwwww#######");
        roomTemplate.push_back("####wwwwwwwwwwwwww######");
        roomTemplate.push_back("#####wwwwwwwwwwwwwww####");
        roomTemplate.push_back("#####wwwwwwwwwwwwww#####");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("            e           ");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Tunnel - Underground Lake";
        Coordinate mapCoord(2, -3);
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("           ######   e   ");
        roomTemplate.push_back("####    e            ###");
        roomTemplate.push_back("##########       #######");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Tunnel";
        Coordinate mapCoord(3, -3);
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("     ^##################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Tunnel";
        Coordinate mapCoord(4, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("CaveTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("######wwwwwwwwwww#######");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("##wwwwwwwww   wwwwwww###");
        roomTemplate.push_back("##wwwwwwww  o  wwwwwww##");
        roomTemplate.push_back("###wwwwwwww   wwwwwwwww#");
        roomTemplate.push_back("###wwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwwww#####");
        roomTemplate.push_back("#####wwwwwwwwwwwwww#####");
        roomTemplate.push_back("#####wwwwwwwwwwwww######");
        roomTemplate.push_back("######wwwwwwwwwww#######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 5), new HeroSpirit(pgame, Coordinate(12, 5)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 8;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Lullin Tunnel - Hero's Cove";
        Coordinate mapCoord(2, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }

    tfloor = 2;
    // The Boranian Valley
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####  e               ##");
        roomTemplate.push_back("####                  ##");
        roomTemplate.push_back("####                  ##");
        roomTemplate.push_back("####     e       e    ##");
        roomTemplate.push_back("#####                 ##");
        roomTemplate.push_back("######                ##");
        roomTemplate.push_back("####v                 ##");
        roomTemplate.push_back("######        e       ##");
        roomTemplate.push_back("#####                 ##");
        roomTemplate.push_back("####                  ##");
        roomTemplate.push_back("####              e   ##");
        roomTemplate.push_back("####     e            ##");
        roomTemplate.push_back("####                  ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(4, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##################      ");
        roomTemplate.push_back("###############         ");
        roomTemplate.push_back("###########      e      ");
        roomTemplate.push_back("########                ");
        roomTemplate.push_back("######                  ");
        roomTemplate.push_back("####         e      e   ");
        roomTemplate.push_back("####    e               ");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("####                    ");
        roomTemplate.push_back("####      e       e   ##");
        roomTemplate.push_back("####                  ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(4, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    e        e   e      ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("    e                   ");
        roomTemplate.push_back("          e        e    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  e   e             e   ");
        roomTemplate.push_back("               e        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(5, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("      ##################");
        roomTemplate.push_back("        ################");
        roomTemplate.push_back("               #########");
        roomTemplate.push_back("  e       e        #####");
        roomTemplate.push_back("                     ###");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("      e           e   ##");
        roomTemplate.push_back("             e        ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##             e      ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(6, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("##         e          @@");
        roomTemplate.push_back("###               e  @@@");
        roomTemplate.push_back("###  e              @@@@");
        roomTemplate.push_back("###      ww ww    @@@@@@");
        roomTemplate.push_back("###   e  w# #w    @@@@@@");
        roomTemplate.push_back("###        +       5    ");
        roomTemplate.push_back("###   e  w# #w    @@@@@@");
        roomTemplate.push_back("###      ww ww    @@@@@@");
        roomTemplate.push_back("###                 @@@@");
        roomTemplate.push_back("###                  @@@");
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("##             e      @@");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][20].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][19].push_back(new EvilWallObject(pgame, Coordinate(3, 6)));
        };


        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(6, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin. Pure evil detected."), Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@                    ##");
        roomTemplate.push_back("@@         e          ##");
        roomTemplate.push_back("                  e   ##");
        roomTemplate.push_back("     e                ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("  e       e       e   ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("      e           e   ##");
        roomTemplate.push_back("             e        ##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(6, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("  e           e         ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("          e       e     ");
        roomTemplate.push_back("    e                   ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                    e   ");
        roomTemplate.push_back("     e       e          ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(5, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("##    e   e e    e    @@");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##        e       e     ");
        roomTemplate.push_back("#####                   ");
        roomTemplate.push_back("########                ");
        roomTemplate.push_back("#########   e   e     e ");
        roomTemplate.push_back("###########             ");
        roomTemplate.push_back("#############           ");
        roomTemplate.push_back("###############   e     ");
        roomTemplate.push_back("#################       ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 19;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(4, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }

    /*
    // Above Lullin Tunnel (Lullin Mountain)
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(2, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(3, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(2, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(3, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(2, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::GREEN;
        std::string name = "Lullin Mountain";
        Coordinate mapCoord(3, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();

    }

    */

    // Korloma Forest
    {}
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
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
        roomTemplate.push_back("                   wwwww");
        roomTemplate.push_back("  0     00    0     wwww");
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###################wwwww");
        roomTemplate.push_back("#################wwwwwww");
        roomTemplate.push_back("###############wwwwwww##");
        roomTemplate.push_back("############wwwwwww#####");
        roomTemplate.push_back("##########wwwwww########");
        roomTemplate.push_back("#######wwwwwww##########");
        roomTemplate.push_back("#####wwwwwwwww##########");
        roomTemplate.push_back("wwwwwwwwww##############");
        roomTemplate.push_back("wwwwww##################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest - Forgotten River";
        Coordinate mapCoord(4, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#####wwwwwwwwwwwwwwww###");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("wwwwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("###wwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("####wwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("####wwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("#####wwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("######wwwwwwwwwwwwwwww##");
        roomTemplate.push_back("######wwwwwwwwwwwwwww###");
        roomTemplate.push_back("######wwwwwwwwwwwwww####");
        roomTemplate.push_back("##########wwwiwwww######");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "The Korloma Forest - Forgotten Lake";
        Coordinate mapCoord(5, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
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
        roomTemplate.push_back("##########        ######");
        roomTemplate.push_back("##########   I    ######");
        roomTemplate.push_back("#########          #####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::BLUE;
        std::string name = "The Korloma Forest - Underwater";
        Coordinate mapCoord(5, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########          #####");
        roomTemplate.push_back("#########   :::   ######");
        roomTemplate.push_back("##########  :I:   ######");
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

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::BLUE;
        std::string name = "The Korloma Forest - Underwater";
        Coordinate mapCoord(5, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######    wwiww  ######");
        roomTemplate.push_back("#####       www      ###");
        roomTemplate.push_back("####                 ###");
        roomTemplate.push_back("#####   0        0   ###");
        roomTemplate.push_back("######              ####");
        roomTemplate.push_back("######  0        0  ####");
        roomTemplate.push_back("#######             ####");
        roomTemplate.push_back("####### 0        0  ####");
        roomTemplate.push_back("#######             ####");
        roomTemplate.push_back("####### 0        0  ####");
        roomTemplate.push_back("#######             ####");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "Korloma Path";
        Coordinate mapCoord(5, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######            #####");
        roomTemplate.push_back("####### 0        0 #####");
        roomTemplate.push_back("#######            #####");
        roomTemplate.push_back("######  0        0  ####");
        roomTemplate.push_back("######               ###");
        roomTemplate.push_back("#####   0        0   ###");
        roomTemplate.push_back("######              ####");
        roomTemplate.push_back("######  0        0  ####");
        roomTemplate.push_back("#######             ####");
        roomTemplate.push_back("######  0        0  ####");
        roomTemplate.push_back("######    0 o 0     ####");
        roomTemplate.push_back("#########         ######");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 10), new HerosBladeStone(pgame, Coordinate(12, 10)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 1;
        int backColor = dngutil::GREEN;
        std::string name = "Korloma Path";
        Coordinate mapCoord(5, 3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
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
        roomTemplate.push_back("# +  o #################");
        roomTemplate.push_back("#      #################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 6), new EldestSage(pgame, Coordinate(5, 6)));

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

void Map::makeHerosTrial(std::mutex& mut)
{
    unsigned int tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######      ###########");
        roomTemplate.push_back("######o       ##########");
        roomTemplate.push_back("#####          #########");
        roomTemplate.push_back("####           o########");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("###                     ");
        roomTemplate.push_back("####            ########");
        roomTemplate.push_back("#####          #########");
        roomTemplate.push_back("######        ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 3), new ManaPotion(pgame, Coordinate(6, 3), dngutil::MANA_POTION_HEAL));
        specificObjects.emplace(Coordinate(15, 5), new MagicalPotion(pgame, Coordinate(15, 5)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "Hero's Trial";
        Coordinate mapCoord(-1, -11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    for (int x = 0; x <= 8; x++)
    {
        {
            std::vector<std::string> roomTemplate;
            roomTemplate.push_back("########################");
            roomTemplate.push_back("########################");
            roomTemplate.push_back("#######          #######");
            roomTemplate.push_back("######     e      ######");
            roomTemplate.push_back("#####              #####");
            roomTemplate.push_back("####                ####");
            roomTemplate.push_back("                       #");
            roomTemplate.push_back("                       #");
            roomTemplate.push_back("####                ####");
            roomTemplate.push_back("#####              #####");
            roomTemplate.push_back("######            ######");
            roomTemplate.push_back("########################");
            roomTemplate.push_back("########################");

            auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
            {
                if (creatureList.size() == 1)
                {
                    return true;
                }
                return false;
            };

            auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
            {
                std::vector<Coordinate> deletionList;
                deletionList.push_back(Coordinate(23, 7));
                deletionList.push_back(Coordinate(23, 6));
                for (auto i : deletionList)
                {
                    WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                    if (wall == nullptr)
                    {
                        errorMessage("error in puzzle", __LINE__, __FILE__);
                    }

                    gameMap[i.y][i.x].remove(wall);
                    wall->removeFromMap(true);
                }
            };

            std::map<Coordinate, MapObject*> specificObjects;

            std::vector<dngutil::TID> possibleCreatures;
            possibleCreatures.push_back(dngutil::TID::Imp);
            possibleCreatures.push_back(dngutil::TID::Cultist);


            int trialNumber = (x + 1);
            int theDifficulty;
            if (trialNumber > 5)
            {
                theDifficulty = -2;
            }
            else
            {
                theDifficulty = -1;
            }

            int difficulty = theDifficulty;
            int backColor = dngutil::MAGENTA;
            std::string name = "Hero's Trial - Trial " + std::to_string(trialNumber);
            Coordinate mapCoord(x, -11);
            RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

            mut.lock();
            gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Slay the enemy"), Mp3File("DungeonTheme")));
            mut.unlock();

        }
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#######          #######");
        roomTemplate.push_back("######            ######");
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("               o    ####");
        roomTemplate.push_back("                    ####");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("######            ######");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        // 15,6
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (gameMap[6][15].size() == 1);
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][17].push_back(new HarpPiece(pgame, Coordinate(17, 6), 6));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::ZorlockImagePhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::ZorlockImagePhase2)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(15, 6), new SegbossTrigger(
            pgame, Coordinate(15, 6),
            new Segboss(bossparts, pgame),
            ColorChar('H', dngutil::DARKGRAY)
        ));

        std::vector<dngutil::TID> possibleCreatures;


        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "Hero's Trial - Trial 11";
        Coordinate mapCoord(9, -11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme")));
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
        specificObjects.emplace(
            Coordinate(1, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(1, 1),
                "Thomas Westenhoffer",
                ColorString("Strange purple men in armor have begun gathering around the Tower of the Gods", dngutil::WHITE)
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
    {
        // ID: 0XAC
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#############");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("#       E   #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("#           #");
        roomTemplate.push_back("###o#########");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 5), new HouseDoorObject(pgame, Coordinate(3, 5), Coordinate(0, -3), Coordinate(5, 6), 2));
        specificObjects.emplace(
            Coordinate(8, 2),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(8, 2),
                "Charlynne Werth",
                ColorString("My husband hasn't been the same ever since he was attacked on the road...", dngutil::WHITE)
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-98, -98);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAD
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######");
        roomTemplate.push_back("#    o");
        roomTemplate.push_back("######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 1), new HouseDoorObject(pgame, Coordinate(5, 1), Coordinate(0, -4), Coordinate(6, 10), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-97, -97);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAE
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#   o  #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("#o######");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(1, 3), new HouseDoorObject(pgame, Coordinate(1, 3), Coordinate(0, -4), Coordinate(12, 10), 2));

        
        specificObjects.emplace(Coordinate(4, 1), new Sage(pgame, Coordinate(4, 1), new MeditationSpell()));
        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-96, -96);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    
    {
        // ID: 0XAF
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########");
        roomTemplate.push_back("#    E #");
        roomTemplate.push_back("#      #");
        roomTemplate.push_back("##o#####");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 3), new HouseDoorObject(pgame, Coordinate(2, 3), Coordinate(1, -2), Coordinate(7, 7), 2));
        specificObjects.emplace(
            Coordinate(5, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(5, 1),
                "Al Sanchez",
                ColorString("The Lullin Tunnel spits you right out at the Tower of the Gods", dngutil::WHITE)
            ));
        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(-95, -95);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAG
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#########");
        roomTemplate.push_back("#    ####");
        roomTemplate.push_back("#  #4####");
        roomTemplate.push_back("#X # ####");
        roomTemplate.push_back("####o####");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[1][3].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[1][4].push_back(new WallObject(pgame, Coordinate(14, 11)));
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 4), new HouseDoorObject(pgame, Coordinate(4, 4), Coordinate(-1, -5), Coordinate(12, 8), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::DARKGRAY;
        std::string name = "Mausoleum";
        Coordinate mapCoord(200, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("OverworldTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAH
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######");
        roomTemplate.push_back("#E   #");
        roomTemplate.push_back("#    o");
        roomTemplate.push_back("#    #");
        roomTemplate.push_back("######");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 2), new HouseDoorObject(pgame, Coordinate(5, 2), Coordinate(-2, -7), Coordinate(7, 9), 2));
        specificObjects.emplace(
            Coordinate(1, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(1, 1),
                "Ghol Smchit",
                ColorString("Get out of my house you did not knock.", dngutil::WHITE)
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(120, 120);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAI
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("######");
        roomTemplate.push_back("#E   #");
        roomTemplate.push_back("o    #");
        roomTemplate.push_back("#    #");
        roomTemplate.push_back("######");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(0, 2), new HouseDoorObject(pgame, Coordinate(0, 2), Coordinate(-2, -7), Coordinate(16, 3), 2));
        
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("There used to be a", dngutil::WHITE) + ColorString(" temple", dngutil::TEXT_HIGHLIGHT_COLOR) + ColorString(" at the graveyard, but it sank into the ground", dngutil::WHITE));
        dialogue.push_back(ColorString("when Zorlock attacked. Only the top room remains.", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(1, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(1, 1),
                "Harry Languan",
                dialogue
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(119, 119);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAJ
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("# E   #");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("###o###");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 3), new HouseDoorObject(pgame, Coordinate(3, 3), Coordinate(-1, -7), Coordinate(7, 4), 2));

        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("Our kingdom has been falling into ruins for centuries...", dngutil::WHITE));
        dialogue.push_back(ColorString("It's sad to compare today's destroyed castle with old pictures.", dngutil::WHITE));
        specificObjects.emplace(
            Coordinate(2, 1),
            new Npc(
                pgame,
                ColorChar('A', dngutil::WHITE),
                Coordinate(2, 1),
                "Cleon Ghoslan",
                dialogue
            ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(118, 118);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAK
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#######");
        roomTemplate.push_back("#     #");
        roomTemplate.push_back("#     o");
        roomTemplate.push_back("#######");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 2), new HouseDoorObject(pgame, Coordinate(6, 2), Coordinate(-3, -8), Coordinate(20, 10), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "House";
        Coordinate mapCoord(117, 117);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }

    {
        // ID: 0XAL
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&o&&");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("& E &");
        roomTemplate.push_back("&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 0), new HouseDoorObject(pgame, Coordinate(2, 0), Coordinate(-7, -3), Coordinate(4, 6), 2));
        specificObjects.emplace(Coordinate(2, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(2, 2),
            "Tyler Nilson",
            ColorString("Outside of Desert Town there is a cave that has a ", dngutil::WHITE) + ColorString("temple", dngutil::TEXT_HIGHLIGHT_COLOR) + ColorString(" in it.", dngutil::WHITE),
            true
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(500, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAM
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&o&&");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&E  &");
        roomTemplate.push_back("&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 0), new HouseDoorObject(pgame, Coordinate(2, 0), Coordinate(-7, -3), Coordinate(13, 6), 2));
        specificObjects.emplace(Coordinate(1, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(1, 2),
            "Emma",
            ColorString("I think there is a boy that has been stalking me...", dngutil::WHITE),
            true
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(499, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAN
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&");
        roomTemplate.push_back("& o o o   &");
        roomTemplate.push_back("&       E &");
        roomTemplate.push_back("&&o&&&&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new VenderObject(
            pgame,
            Coordinate(2, 1),
            ColorString("a Potion", dngutil::RED),
            ColorChar('o', dngutil::RED),
            dngutil::TID::Potion,
            50
        ));
        specificObjects.emplace(Coordinate(4, 1), new VenderObject(
            pgame,
            Coordinate(4, 1),
            ColorString("a Mana Potion", dngutil::WHITE),
            ColorChar('o', dngutil::WHITE),
            dngutil::TID::ManaPotion,
            50
        ));
        specificObjects.emplace(Coordinate(6, 1), new VenderObject(
            pgame,
            Coordinate(6, 1),
            ColorString("some Reinforced Boots", dngutil::BROWN),
            ColorChar('*', dngutil::BROWN),
            dngutil::TID::ReinforcedBoots,
            155
        ));
        specificObjects.emplace(Coordinate(2, 3), new HouseDoorObject(pgame, Coordinate(2, 3), Coordinate(-7, -4), Coordinate(5, 7), 2));
        specificObjects.emplace(Coordinate(8, 2), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(8, 2),
            "Terra Ghoslan",
            ColorString("This is Desert Town's general store.", dngutil::WHITE),
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "General Store";
        Coordinate mapCoord(488, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAO
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&");
        roomTemplate.push_back("&E  &");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&&o&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 3), new HouseDoorObject(pgame, Coordinate(2, 3), Coordinate(-9, -4), Coordinate(8, 7), 2));
        specificObjects.emplace(Coordinate(1, 1), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(1, 1),
            "Holan Ho",
            ColorString("I love the smell of roses, but I don't see any here...", dngutil::WHITE),
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(760, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAP
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&o&&");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&&&&&");
         

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 0), new HouseDoorObject(pgame, Coordinate(2, 0), Coordinate(-9, -4), Coordinate(16, 8), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(22, 22);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAQ
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&o&&");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&   &");
        roomTemplate.push_back("&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 0), new HouseDoorObject(pgame, Coordinate(2, 0), Coordinate(-8, -3), Coordinate(17, 5), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(1200, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
    {
        // ID: 0XAR
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&o&&");
        roomTemplate.push_back("&E       &");
        roomTemplate.push_back("&        &");
        roomTemplate.push_back("&&&&&&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 0), new HouseDoorObject(pgame, Coordinate(7, 0), Coordinate(-9, -3), Coordinate(14, 3), 2));
        specificObjects.emplace(Coordinate(1, 1), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(1, 1),
            "Yoshiro Cal",
            ColorString("I saw something flash at the top of the Tower of the Gods.", dngutil::WHITE),
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "House";
        Coordinate mapCoord(333, 322);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("VillageTheme")));
        mut.unlock();
    }
}

void Map::makeTowerOfTheGods(std::mutex& mut)
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@oo                  @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@oo                  @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 2), new MagicalPotion(pgame, Coordinate(2, 2)));
        specificObjects.emplace(Coordinate(3, 2), new MagicalPotion(pgame, Coordinate(3, 2)));

        specificObjects.emplace(Coordinate(2, 10), new Potion(pgame, Coordinate(2, 10), dngutil::POTION_HEAL + 20));
        specificObjects.emplace(Coordinate(3, 10), new ManaPotion(pgame, Coordinate(3, 10), dngutil::MANA_POTION_HEAL + 20));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 12;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(7, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@         @@@        @@");
        roomTemplate.push_back("           @@@    ^   @@");
        roomTemplate.push_back("@@         @@@        @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 12;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@-@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@         @@@        @@");
        roomTemplate.push_back("           @@@    v   @@");
        roomTemplate.push_back("@@         @@@        @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 12;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@ @ @@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@ @ @@@@@@@@@@@@@");
        roomTemplate.push_back("@@       @        e    @");
        roomTemplate.push_back("@@ @@@@::@:@@@@@@@@@@@@@");
        roomTemplate.push_back("@@    @  @ @          @@");
        roomTemplate.push_back("@@@@@:@  @ @@@@@@@@ @ @@");
        roomTemplate.push_back("@@  e @  @  e     @ @ @@");
        roomTemplate.push_back("@@ @@@@  @@@@@@@@-@ @ @@");
        roomTemplate.push_back("@@ @            @^@ @ @@");
        roomTemplate.push_back("@@ @@@@@ @@@:@@@@@@@@ @@");
        roomTemplate.push_back("@@   :     @          @@");
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@         e      e     ");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@       @@@@@@@@@    @@");
        roomTemplate.push_back("@@    e  @       @   e@@");
        roomTemplate.push_back("@@       @ @@@@  @    @@");
        roomTemplate.push_back("@@       @ @o @  @    @@");
        roomTemplate.push_back("@@  e    @ @  @  @    @@");
        roomTemplate.push_back("@@       @ @  e    e  @@");
        roomTemplate.push_back("@@       @ @          @@");
        roomTemplate.push_back("@@@@@@@@ @ @@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@ @ @@@@@@@@@@@@@");

        // 12,7
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 7), new MagicalPotion(pgame, Coordinate(12, 7)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("          @     @      @ e @          @@");
        roomTemplate.push_back("@@ @@@@@@ @ @@@ @ @@@@ @ @ @ @@@@@@@  @@");
        roomTemplate.push_back("@@      @ e @ @ @ @@@@ @ @ @ @o    @  @@");
        roomTemplate.push_back("@@ @@@@ @@@@@ @   @  @ @ @ @ @@@@@ @  @@");
        roomTemplate.push_back("@@ @ e@  @    @@@@@ @@ @ @ @  e  @ @  @@");
        roomTemplate.push_back("@@ @@ @@@@ @@@@        : @ @@@@@ @ @ww@@");
        roomTemplate.push_back("@@     @     e@ @ @@@@ @@@:@   @ @ @  @@");
        roomTemplate.push_back("@@@@@ @@@ @@@@@ @ @@@@ @@@ @ @ @ @ @  @@");
        roomTemplate.push_back("@@   e          @      @@@   @   @    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

// 31,4          
        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(30, 4), new Key(pgame, Coordinate(30, 4)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(9, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@ @@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@wwwww          wwwww@@");
        roomTemplate.push_back("@@wwwwwwwwwwwwwwwwwwww@@");
        roomTemplate.push_back("@@wwwwwwwwwwwwwwwwwwww@@");
        roomTemplate.push_back("@@wwwwwwwwwwwwwwwwwwww@@");
        roomTemplate.push_back("@@wwwwwwwww   wwwwwwww@@");
        roomTemplate.push_back("@@wwwwwwww     wwwwwww@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 12;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@    e    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@ @@@@@@@ @@");
        roomTemplate.push_back("@@@@@@@@@@@@@ @@@@@@@ @@");
        roomTemplate.push_back("@@@@@@@@@@@@@ @@@@@@@ @@");
        roomTemplate.push_back("@@@@@@@@@@@@@ @@@@@@@   ");
        roomTemplate.push_back("@@@@@@@@@@@@@ @@@@@@@@@@");
        roomTemplate.push_back("@@@@     e    @@@@@@@@@@");
        roomTemplate.push_back("@@@@ @@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("     @@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(7, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("###                  @@@");
        roomTemplate.push_back("###                 @@@@");
        roomTemplate.push_back("###      ww ww    @@@@@@");
        roomTemplate.push_back("###      w# #w    @    @");
        roomTemplate.push_back("###        +      @ o  @");
        roomTemplate.push_back("###      w# #w    @    @");
        roomTemplate.push_back("###      ww ww    @@@@ @");
        roomTemplate.push_back("###                 @@ @");
        roomTemplate.push_back("###                  @  ");
        roomTemplate.push_back("##                    @@");
        roomTemplate.push_back("##                    @@");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(20, 6), new Key(pgame, Coordinate(20, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(6, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@           @@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@ @@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@ @@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@     @@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@ ^   @@@");
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        roomTemplate.push_back("&&&&&&&&&&&-&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&& &&&&&&&&&&&&");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MegaBeastPhase1);

        int difficulty = -1;
        int backColor = dngutil::LIGHTCYAN;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&& &&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&& &&&&&&&&&&&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@e@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@^@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@@@@@@@@@@@@&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DarkKnight);

        int difficulty = -4;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@ e @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@e  @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@  e@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@e  @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@  e@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -1;
        int backColor = dngutil::LIGHTCYAN;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@   e    @@       @@");
        roomTemplate.push_back("@@o@@ @@@@ o @@ @@@@@ @@");
        roomTemplate.push_back("@@ @@ @@ @   @@  e @@ @@");
        roomTemplate.push_back("@@ @@ @@ @@@@@@@@@@@@ @@");
        roomTemplate.push_back("@@ @@ @@e@@@@@@      e@@");
        roomTemplate.push_back("@@ @@ @@ @@@@@@ @@@@@ @@");
        roomTemplate.push_back("@@ @@           @@@@@ @@");
        roomTemplate.push_back("@@ @@@@@@@@@@@@@@@@@@ @@");
        roomTemplate.push_back("@@e@@@@@@@@@@@@@@@@@@ @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@   @@@@@@@@@@@");


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 2), new Key(pgame, Coordinate(11, 2)));
        specificObjects.emplace(Coordinate(2, 2), new MagicalPotion(pgame, Coordinate(2, 2)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -1;
        int backColor = dngutil::LIGHTCYAN;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&@@@@@@@@@^@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@v@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@@@@@@@@@@@@&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&@@@@@@@@@v@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@^@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@@@@@@@@@@@@&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&@@@@@@@@@^@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@v@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@@@@@@@@@@@@&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&@@@@@@@@@v@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@ @@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@^@@@@@@@@@@&&");
        roomTemplate.push_back("&&@@@@@@@@@@@@@@@@@@@@&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@  oo                @@");
        roomTemplate.push_back("@@  oo                @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@         v          @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 3), new Potion(pgame, Coordinate(4, 3), dngutil::POTION_HEAL * 2));
        specificObjects.emplace(Coordinate(5, 3), new Potion(pgame, Coordinate(5, 3), dngutil::POTION_HEAL * 2));
        specificObjects.emplace(Coordinate(4, 4), new ManaPotion(pgame, Coordinate(4, 4), dngutil::MANA_POTION_HEAL * 2));
        specificObjects.emplace(Coordinate(5, 4), new ManaPotion(pgame, Coordinate(5, 4), dngutil::MANA_POTION_HEAL * 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Top of The Tower of the Gods";
        Coordinate mapCoord(8, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW     o     WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@ @@@@@@@@@@@@");
        // 12, 5
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[5][12].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            //Creature* creature = pgame->generateCreature(1, dngutil::TID::TrueZorlock);
            //creature->setPosition(Coordinate(12, 10));
            gameMap[10][12].push_back(new HoleObject(pgame, Coordinate(10, 12), true));
            //pgame->getActiveRoom()->addCreature(creature, Coordinate(12, 10));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase2)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase3)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 5), new SegbossTrigger(
            pgame, Coordinate(12, 5),
            new Segboss(bossparts, pgame),
            ColorChar('H', dngutil::MAGENTA)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Top of The Tower of the Gods";
        Coordinate mapCoord(8, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin. Pure evil detected."), Mp3File("DangerTheme")));
        mut.unlock();
    }

    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");


        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::TrueZorlock);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "";
        Coordinate mapCoord(8, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");


        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(8, -5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");


        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(8, -6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");


        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(8, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&      ++      &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("        &&    &&        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("&&&E                    ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("&&&                     ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        //3, 6

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("Beyond this passage lies a portal that will return you to the overworld", dngutil::LIGHTRED));
        dialogue.push_back(ColorString("Beware, however, as you will never be able to return in this life", dngutil::LIGHTRED));
        specificObjects.emplace(Coordinate(3, 6), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(3, 6),
            "Grim Reaper",
            dialogue,
            false
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(8, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("x                       ");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        roomTemplate.push_back("                       #");
        //3, 6

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 4), new HouseDoorObject(pgame, Coordinate(3, 4), Coordinate(0, -2), Coordinate(6, 9), 2));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(7, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }

    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("     o                  ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        //3, 6

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 5), new HouseDoorObject(pgame, Coordinate(5, 5), Coordinate(0, 0), Coordinate(5, 5), 2, true));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "Beginning?";
        Coordinate mapCoord(7, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }
    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&    &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&        &&&&&&&&");
        roomTemplate.push_back("&&&&&              &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&              &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&              &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&   &&    &&   &&&&&");
        roomTemplate.push_back("&&&&&              &&&&&");


        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Demon Realm";
        Coordinate mapCoord(8, -9);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }

    int floorAtEntryToFort = tfloor;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&oo&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        //11(12), 3

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 3), new HouseDoorObject(pgame, Coordinate(11, 3), Coordinate(0, 0), Coordinate(11, 1), dngutil::DEMON_FLOOR));
        specificObjects.emplace(Coordinate(12, 3), new HouseDoorObject(pgame, Coordinate(12, 3), Coordinate(0, 0), Coordinate(12, 1), dngutil::DEMON_FLOOR));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "";
        Coordinate mapCoord(8, -10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DemonTheme")));
        mut.unlock();
    }

    tfloor = dngutil::DEMON_FLOOR;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########oo###########");
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
        //11(12), 3

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 1), new HouseDoorObject(pgame, Coordinate(11, 1), Coordinate(8, -10), Coordinate(11, 3), floorAtEntryToFort));
        specificObjects.emplace(Coordinate(12, 1), new HouseDoorObject(pgame, Coordinate(12, 1), Coordinate(8, -10), Coordinate(12, 3), floorAtEntryToFort));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########### ############");
        roomTemplate.push_back("########### ############");
        roomTemplate.push_back("########### ############");
        roomTemplate.push_back("###########% ###########");
        roomTemplate.push_back("###########  ###########");
        //
        // 11(12) 7
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[7][11].size() > 1 || gameMap[7][12].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            int x1 = 11;
            int x2 = 12;
            int y1 = 12;
            for (int i = 0; i < 5; i++)
            {
                pgame->getVWin()->txtmacs.clearMapArea(false, false);
                gameMap[y1 - i][x1].push_back(new WallObject(pgame, Coordinate(x1, y1 - i)));
                gameMap[y1 - i][x2].push_back(new WallObject(pgame, Coordinate(x2, y1 - i)));
                playSound(WavFile("UnlockDoor", false, false));
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        //specificObjects.emplace(Coordinate(1, 1), new HerosClaim(pgame, Coordinate(1, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin. Pure evil detected."), Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    //right
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("           e            ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########              ");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("           e            ");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");



        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("########## e            ");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");



        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###         ##         #");
        roomTemplate.push_back("### ####### ##    #### #");
        roomTemplate.push_back("### #   ### #####   ## #");
        roomTemplate.push_back("#   # # ###    # ## ## #");
        roomTemplate.push_back("# # # # ###e## #  e ## #");
        roomTemplate.push_back("  # # # ### ## # ## ##  ");
        roomTemplate.push_back("###   # ### #### ## ####");
        roomTemplate.push_back("#   ### ###      ##    #");
        roomTemplate.push_back("# #####     ########## #");
        roomTemplate.push_back("# # #########   #   ## #");
        roomTemplate.push_back("#   #o        #   #    #");
        roomTemplate.push_back("########################");

        //5, 11

        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(5, 11), new MagicalPotion(pgame, Coordinate(5, 11)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(3, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("####               #####");
        roomTemplate.push_back("####     #####   e #####");
        roomTemplate.push_back("####  e  #####     #####");
        roomTemplate.push_back("####     #####     #####");
        roomTemplate.push_back("####  e  #####   e #####");
        roomTemplate.push_back("####       o       #####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");



        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(11, 10), new Primary(
            pgame,
            ColorChar(')', dngutil::RED),
            Coordinate(11, 10),
            "Demon Bow",
            false,
            1.7,
            3,
            50,
            false,
            "A bow embued with demonic energy. Shoots Fire.",
            WavFile("FireAttack1", false, false),
            dngutil::ClassType::RANGER
        ));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("##########   ###########");
        roomTemplate.push_back("           e            ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");



        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(3, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                    ####");
        roomTemplate.push_back("############### ### ####");
        roomTemplate.push_back("############### ### ####");
        roomTemplate.push_back("####        ### ### ####");
        roomTemplate.push_back("####  ####  ### ### ####");
        roomTemplate.push_back("          e ###  e      ");
        roomTemplate.push_back("##########  ### ### ####");
        roomTemplate.push_back("##########  ### ### ####");
        roomTemplate.push_back("##########  ###         ");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########  ############");



        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(4, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##########  ############");
        roomTemplate.push_back("##########          ####");
        roomTemplate.push_back("##########     e    ####");
        roomTemplate.push_back("##########  ### ### ####");
        roomTemplate.push_back("##########  ### ### ####");
        roomTemplate.push_back("##########  ### ### ####");
        roomTemplate.push_back("            ### ### ####");
        roomTemplate.push_back("############### ### ####");
        roomTemplate.push_back("############### ### ####");
        roomTemplate.push_back("###############  o  ####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        //17, 9


        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(17, 9), new ManaPotion(pgame, Coordinate(17, 9), dngutil::MANA_POTION_HEAL * 2));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(4, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####              e  ###");
        roomTemplate.push_back("#### # # # # # # # # ###");
        roomTemplate.push_back("####        e        ###");
        roomTemplate.push_back("#### # # # # # # # # ###");
        roomTemplate.push_back("      e              ###");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("                    #   ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        // 16,5

        //20,9
        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[5][16].size() > 1)
            {
                return true;
            }
            return false;
        };

        // 4,6 --> 19,6
        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            int y = 9;
            int x = 20;
            delete gameMap[y][x].back();
            gameMap[y][x].pop_back();
            playSound(WavFile("StepOnSwitch", false, false));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(5, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Find the hidden switch."), Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#### o #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("####   #################");
        roomTemplate.push_back("       #################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        // 5, 2


        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(5, 2), new Key(pgame, Coordinate(5, 2)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(6, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    // up
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        // x: 10-13
        // y: 5
        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {

            if (pgame->getPlayer()->hasItem(dngutil::TID::HerosClaim))
            {
                return true;
            }
            else if (gameMap[5][11].size() > 1 || gameMap[5][12].size() > 1)
            {
                playSound(WavFile("Spellbook", false, false));
                pgame->getPlayer()->adjustPosition(dngutil::Movement::DOWN);
                pgame->getPlayer()->adjustPosition(dngutil::Movement::DOWN);
                pgame->getPlayer()->adjustPosition(dngutil::Movement::DOWN);
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            // nada
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -5;
        int backColor = dngutil::MAGENTA;
        std::string name = "Entrance To The Seal Room";
        Coordinate mapCoord(0, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "A large amount of energy is required."), Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########@@@@@@@@########");
        roomTemplate.push_back("########@@@@@@@@########");
        roomTemplate.push_back("#######    e     #######");
        roomTemplate.push_back("####### @      @ #######");
        roomTemplate.push_back("#######          #######");
        roomTemplate.push_back("####### @      @ #######");
        roomTemplate.push_back("#######          #######");
        roomTemplate.push_back("####### @      @ #######");
        roomTemplate.push_back("#######          #######");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########%%###########");
        roomTemplate.push_back("###########  ###########");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            static int timer = 0;
            timer++;
            if (timer % 1500 == 0)
            {
                if (creatureList.front()->getPGame()->getActiveRoom()->getRoomInfo().backColor == dngutil::MAGENTA)
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::BLACK);
                }
                else
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::MAGENTA);
                }
            }

            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::TrueZorlock);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Seal Room";
        Coordinate mapCoord(0, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "???"), Mp3File("DangerTheme")));
        mut.unlock();
    }

    // left
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########  e  #########");
        roomTemplate.push_back("##########  #  #########");
        roomTemplate.push_back("##########  #  #########");
        roomTemplate.push_back("            #           ");
        roomTemplate.push_back("##########  #  #########");
        roomTemplate.push_back("##########  #  #########");
        roomTemplate.push_back("##########  e  #########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########### v ##########");
        roomTemplate.push_back("##########     #########");
        roomTemplate.push_back("##########  e  #########");
        roomTemplate.push_back("##########     #########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("    XXXXXXXXXXXXXXXX    ");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("###XXXXXXXXXXXXXXXXXX###");
        roomTemplate.push_back("########################");
        
        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (pgame->getPlayer()->hasItem(dngutil::TID::HerosClaim))
            {
                return true;
            }
            return false;
        };

        // 4,6 --> 19,6
        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            int y = 6;
            for (int x = 4; x <= 19; x++)
            {
                delete gameMap[y][x].back();
                gameMap[y][x].pop_back();
                playSound(WavFile("UnlockDoor", false, false));
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-3, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Powerful energy is required to break this seal."), Mp3File("FinalDungeonTheme")));
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
        roomTemplate.push_back("####################o   ");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        //20, 6

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(20, 6), new Secondary(
            pgame,
            ColorChar('<', dngutil::LIGHTRED),
            Coordinate(20, 6),
            "Demon Shield",
            false,
            dngutil::TID::Secondary,
            300,
            1.4,
            "A shield embued with devilish energy"
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -5;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-4, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    // bottom floor
    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##          ^         ##");
        roomTemplate.push_back("## #######     ###### ##");
        roomTemplate.push_back("## #######     ###### ##");
        roomTemplate.push_back("## #######     ###### ##");
        roomTemplate.push_back("            e           ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("## ########   ####### ##");
        roomTemplate.push_back("## ########   ####### ##");
        roomTemplate.push_back("## ########   ####### ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("############ ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("            e           ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("## ################## ##");
        roomTemplate.push_back("## ################## ##");
        roomTemplate.push_back("##     wwwwwwwwww     ##");
        roomTemplate.push_back("       wwwwiiwwww     ##");
        roomTemplate.push_back("##     wwwwwwwwww     ##");
        roomTemplate.push_back("## ################## ##");
        roomTemplate.push_back("## ################## ##");
        roomTemplate.push_back("##         e          ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

   

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("############-###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("############-###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("############ ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### ^ ##########");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2,  1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("###      e           ###");
        roomTemplate.push_back("###               e  ###");
        roomTemplate.push_back("###   e   e          ###");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("###            e        ");
        roomTemplate.push_back("###     e            ###");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("###           e      ###");
        roomTemplate.push_back("###  o          e    ###");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("########################");
        // 5,10
        

        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(5, 10), new ManaPotion(pgame, Coordinate(5, 10), dngutil::MANA_POTION_HEAL * 2));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -1;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-3, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    // water floor
    tfloor--;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("##########              ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("########### ############");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("##########             #");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");

        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[3][11].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            int y = 6;
            int x = 23;
            delete gameMap[y][x].back();
            gameMap[y][x].pop_back();
            playSound(WavFile("StepOnSwitch", false, false));
        };

        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Find the hidden switch."), Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("##########              ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        

        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");



        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");



        std::map<Coordinate, MapObject*> specificObjects;


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(1, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("            o ##########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        //12, 6

        std::map<Coordinate, MapObject*> specificObjects;

        specificObjects.emplace(Coordinate(12, 6), new Key(pgame, Coordinate(12, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("           II           ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(1, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(1, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("              ##########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, -1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Imp);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(2, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("           e   #########");
        roomTemplate.push_back("##########    ##########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::PossessedBowman);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(3, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DistortedFinalDungeonTheme")));
        mut.unlock();
    }

    // upper stories
    tfloor = dngutil::DEMON_FLOOR;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########### ^ ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### e ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### e ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### e ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### v ##########");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########### v ##########");
        roomTemplate.push_back("####              e  # #");
        roomTemplate.push_back("#### ######   ###### # #");
        roomTemplate.push_back("##   ############### # #");
        roomTemplate.push_back("## ######   e   #### # #");
        roomTemplate.push_back("## ###### ##### #### # #");
        roomTemplate.push_back("## ###### #     #### # #");
        roomTemplate.push_back("## ###### #   ###### # #");
        roomTemplate.push_back("##        #   ######## #");
        roomTemplate.push_back("########### ^          #");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Cultist);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########### ^ ##########");
        roomTemplate.push_back("########### # ##########");
        roomTemplate.push_back("########### e ##########");
        roomTemplate.push_back("########### # ##########");
        roomTemplate.push_back("###########e  ##########");
        roomTemplate.push_back("########### # ##########");
        roomTemplate.push_back("###########  e##########");
        roomTemplate.push_back("########### # ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("########### v ##########");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::MaskVar1);

        int difficulty = -2;
        int backColor = dngutil::MAGENTA;
        std::string name = "The Demon Fortress";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    tfloor++;

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("############v###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("########    e    #######");
        roomTemplate.push_back("########    o    #######");
        roomTemplate.push_back("########         #######");
        roomTemplate.push_back("########################");
        //12, 10
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 10), new HerosClaim(pgame, Coordinate(12, 10)));


        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DarkKnight2);

        int difficulty = -3;
        int backColor = dngutil::RED;
        std::string name = "Sacred Chamber";
        Coordinate mapCoord(-2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();
    }

    /*
    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@               e    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][4].push_back(new ExitObject(pgame, Coordinate(4, 6), true, ColorChar('^', dngutil::MAGENTA)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SkeletonKing);

        int difficulty = -2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("FinalDungeonTheme")));
        mut.unlock();

    }
    tfloor++;
    {}
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@               e    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][4].push_back(new ExitObject(pgame, Coordinate(4, 6), true, ColorChar('^', dngutil::MAGENTA)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::WaterHorse);

        int difficulty = -2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("FinalDungeonTheme")));
        mut.unlock();

    }
    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@               e    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][4].push_back(new ExitObject(pgame, Coordinate(4, 6), true, ColorChar('^', dngutil::MAGENTA)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::FlameHorse);

        int difficulty = -2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("FinalDungeonTheme")));
        mut.unlock();

    }
    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@               e    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][4].push_back(new ExitObject(pgame, Coordinate(4, 6), true, ColorChar('^', dngutil::MAGENTA)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::GhostHorse);

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Tower of the Gods";
        Coordinate mapCoord(5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("FinalDungeonTheme")));
        mut.unlock();

    }
    tfloor++;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("@@@@@@@@@@@   @@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@   @@@@@@@@@@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@                    @@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -2;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Top of The Tower of the Gods";
        Coordinate mapCoord(5, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("FinalDungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWWWWWWWWWWWWWWWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW     o     WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("WWWWWWW           WWWWWW");
        roomTemplate.push_back("@@@@@@@@@@@   @@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@   @@@@@@@@@@");
        // 12, 5
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[5][12].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            Creature* creature = pgame->generateCreature(1, dngutil::TID::TrueZorlock);
            creature->setPosition(Coordinate(12, 10));
            gameMap[10][12].push_back(creature);
            pgame->getActiveRoom()->addCreature(creature, Coordinate(12, 10));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase2)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::EvilBeastPhase3)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 5), new SegbossTrigger(
            pgame, Coordinate(12, 5),
            new Segboss(bossparts, pgame),
            ColorChar('H', dngutil::MAGENTA)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "Top of The Tower of the Gods";
        Coordinate mapCoord(5, -4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin. Pure evil detected."), Mp3File("DangerTheme")));
        mut.unlock();

    }*/
}

void Map::makeFireTemple(std::mutex& mut)
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&& &&&&&&&&&&&&&&&&&& &&");
        roomTemplate.push_back("   &&              &&   ");
        roomTemplate.push_back("   &&              &&   ");
        roomTemplate.push_back("   &&           v  &&   ");
        roomTemplate.push_back("&&&&&              &&&&&");
        roomTemplate.push_back("&&&&&              &&&&&");
        roomTemplate.push_back("&&&&&&&&&& &&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&& &&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&& &&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-5, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&o&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&   &&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&  @  &&&&&&&&&&&&");
        roomTemplate.push_back("   &    @               ");
        roomTemplate.push_back("   &   @@@@@ @@@@       ");
        roomTemplate.push_back("   &    @               ");
        roomTemplate.push_back("&&&&&&&  @  &&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&   &&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][12].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            deletionList.push_back(Coordinate(3, 5));
            deletionList.push_back(Coordinate(3, 6));
            deletionList.push_back(Coordinate(3, 7));
            for (auto i : deletionList)
            {
                CastleWallObject* wall = dynamic_cast<CastleWallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(9, 2), new MagicalPotion(pgame, Coordinate(9, 2)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-6, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Complete it."), Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&             ");
        roomTemplate.push_back("&&XX    e               ");
        roomTemplate.push_back("&&&&&&&&&&&             ");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::FlameHorse);

        int difficulty = -3;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-7, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&XXXXXXXXXXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("     e              XX&&");
        roomTemplate.push_back("     e            v XX&&");
        roomTemplate.push_back("     e              XX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXX  XXXXXXXXXXXX&&");
        roomTemplate.push_back("&&XXXXXXXXXXXXXXXXXXXX&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -2;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-4, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  & & & &  & & & &  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  & & & &  & & & &  &&");
        roomTemplate.push_back("                      | ");
        roomTemplate.push_back("&&  & & & &  & &^& &  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  & & & &  & & & &  &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 1), new PowerBoots(pgame, Coordinate(2, 1)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-5, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("                  ^   &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-4, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&          e         &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&    e          e    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&&&&&&&&&&  &&&&&&&&&&&");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][12].push_back(new Primary(
                pgame,
                ColorChar('t', dngutil::BROWN),
                Coordinate(12, 6),
                "Musket",
                false,
                2.2,
                8,
                50,
                true,
                "A very innacurate but powerful gun",
                WavFile("GunAttack1", false, false),
                dngutil::ClassType::RANGER,
                dngutil::TID::Primary,
                false
            ));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-5, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing"), Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&o  X          X     &&");
        roomTemplate.push_back("&&o  X XXXXX X  X XXX &&");
        roomTemplate.push_back("&&   X e   X X  X X X &&");
        roomTemplate.push_back("&&   X XXX X X    X Xv&&");
        roomTemplate.push_back("&&XX X   X X X  XXX XX&&");
        roomTemplate.push_back("   X X X X X X   e      ");
        roomTemplate.push_back("&& X X X X XXXXXXXX X &&");
        roomTemplate.push_back("&& X X X XeX     XX X &&");
        roomTemplate.push_back("&& X   X X X X X XX X &&");
        roomTemplate.push_back("&& XXXXX X XXX X XXXX &&");
        roomTemplate.push_back("&&       X     X     X&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 2), new Potion(pgame, Coordinate(2, 2), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(2, 1), new ManaPotion(pgame, Coordinate(2, 1), dngutil::MANA_POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-6, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&::&&&&&&&&&&");
        roomTemplate.push_back("&&::::::::::::::::::::&&");
        roomTemplate.push_back("&&::::::::::::::::::::&&");
        roomTemplate.push_back("&&::::::::::::::      &&");
        roomTemplate.push_back("&&::::::              &&");
        roomTemplate.push_back("&&    ::              &&");
        roomTemplate.push_back("&&  o ::                ");
        roomTemplate.push_back("&&    ::              &&");
        roomTemplate.push_back("&&::::::::::::::      &&");
        roomTemplate.push_back("&&::::::::::::::::::::&&");
        roomTemplate.push_back("&&::::::::::::::::::::&&");
        roomTemplate.push_back("&&::::::::::::::::::::&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 6), new PowerBoots(pgame, Coordinate(4, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-7, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("||                      ");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-7, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##    o                 ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][6].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][4].push_back(new HarpPiece(pgame, Coordinate(20, 4), 3));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::DragonTail)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::DragonWings)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::DragonHead)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(6, 6), new SegbossTrigger(
            pgame, Coordinate(6, 6),
            new Segboss(bossparts, pgame),
            ColorChar('S', dngutil::RED)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -4;
        int backColor = dngutil::BLACK;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-8, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&XX&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&XXXXXXXXX&&&&&&&");
        roomTemplate.push_back("&&&&&&&XXXX    XXX&&&&&&");
        roomTemplate.push_back("       XXX   o  XXX&&&&&");
        roomTemplate.push_back("&&&&&&&&XXX     XX&&&&&&");
        roomTemplate.push_back("&&&&&&&&&XXXXXXXXX&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&XXXXXXX&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(13, 6), new Key(pgame, Coordinate(13, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -1;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-6, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("& &&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("& &&       &  :      : &");
        roomTemplate.push_back("&    &&&&& &&&&&&&&&&& &");
        roomTemplate.push_back("& && &&& & &&&&    & & &");
        roomTemplate.push_back("& && &&&  &&&&&&&& & & &");
        roomTemplate.push_back("& &&e&&&& :    : & & & &");
        roomTemplate.push_back("& && &&&  &&  && & & & &");
        roomTemplate.push_back("& && &&  &&&  &&   &e  &");
        roomTemplate.push_back("& && &  &&&&  &&:&&& & &");
        roomTemplate.push_back("& &&   &&&&&  &      & &");
        roomTemplate.push_back("&  o&&&&&&&&  & &&&&&& &");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(3, 11), new MagicalPotion(pgame, Coordinate(3, 11)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -2;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-7, -9);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&o &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&e &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&::&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&::&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&::&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&::&&&&&&&&&&");
        roomTemplate.push_back("&&&&&&&&&&&&  &&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(12, 2), new Key(pgame, Coordinate(12, 2)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -2;
        int backColor = dngutil::RED;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-7, -10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }

    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&       e           ^&&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&  e            e    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&        e           &&");
        roomTemplate.push_back("&&                    &&");  
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&                    &&");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[2][2].push_back(new Key(pgame, Coordinate(2, 2)));
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::DesertGoblin);

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-6, -7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing."), Mp3File("DungeonTheme")));
        mut.unlock();

    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");
        roomTemplate.push_back("&         e           o&");
        roomTemplate.push_back("& &&&&&&&&&&&&&& & &&&&&");
        roomTemplate.push_back("& &&&&    &&&&&& & &   &");
        roomTemplate.push_back("& &    &&&&    &&&&& & &");
        roomTemplate.push_back("& & && &&&      &&   & &");
        roomTemplate.push_back("& & ^   &  ^       &&& &");
        roomTemplate.push_back("& & & &&&&&      &&&e  &");
        roomTemplate.push_back("& & & &&&&&     &&&& & &");
        roomTemplate.push_back("& & & &&&&&&&&&&   & & &");
        roomTemplate.push_back("& & &         && & & & &");
        roomTemplate.push_back("&e  &&&&&&&&&    &   & &");
        roomTemplate.push_back("&&&&&&&&&&&&&&&&&&&&&&&&");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(22, 1), new Potion(pgame, Coordinate(22, 1), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);

        int difficulty = -3;
        int backColor = dngutil::BLACK;
        std::string name = "The Fire Temple";
        Coordinate mapCoord(-6, -8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();

    }
}

void Map::makeWaterTemple(std::mutex& mut)
{
    unsigned int tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Submerged Landbridge";
        Coordinate mapCoord(-3, 5);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Submerged Landbridge";
        Coordinate mapCoord(-3, 6);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme"), false));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwww   wwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWww  +  wwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwww   wwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "Altar Island";
        Coordinate mapCoord(-3, 7);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWWwwwwwwwwwWWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW         WWWWWWW");
        roomTemplate.push_back("WWWWWWWW   #2#   WWWWWWW");
        roomTemplate.push_back("WWWWWWWW#### ####WWWWWWW");
        roomTemplate.push_back("WWW######### ########WWW");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[12][12].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[11][12].push_back(new WallObject(pgame, Coordinate(12, 11)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-3, 8);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("OverworldTheme"), false));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("########oo       #######");
        roomTemplate.push_back("########         #######");
        roomTemplate.push_back("########         #######");
        roomTemplate.push_back("########         #######");
        roomTemplate.push_back("########   www   #######");
        roomTemplate.push_back("########  wweww  #######");
        roomTemplate.push_back("########   www   #######");
        roomTemplate.push_back("########         #######");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            deletionList.push_back(Coordinate(12, 11));
            deletionList.push_back(Coordinate(12, 12));

            for (auto i : deletionList)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

gameMap[i.y][i.x].remove(wall);
wall->removeFromMap(true);
pgame->getVWin()->txtmacs.displayGame(pgame);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(8, 3), new Potion(pgame, Coordinate(8, 3), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(9, 3), new ManaPotion(pgame, Coordinate(9, 3), dngutil::MANA_POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -1;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-3, 9);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Slay the enemy"), Mp3File("DungeonTheme"), false));
        mut.unlock();
    }
    {
    std::vector<std::string> roomTemplate;
    roomTemplate.push_back("############ ###########");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("##  # # # # # # # # # ##");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("##  # # # # # # # # # ##");
    roomTemplate.push_back("                        ");
    roomTemplate.push_back("##  # # # # # # # # # ##");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("##  # # # # # # # # # ##");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("##                    ##");
    roomTemplate.push_back("############-###########");

    std::map<Coordinate, MapObject*> specificObjects;

    std::vector<dngutil::TID> possibleCreatures;

    int difficulty = -1;
    int backColor = dngutil::CYAN;
    std::string name = "The Water Temple";
    Coordinate mapCoord(-3, 10);
    RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
    mut.lock();
    gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
    mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww  e  wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("##wwwwwwww  e  wwwwwww##");
        roomTemplate.push_back("##wwwwwwww     wwwwwww##");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("############-###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -2;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-3, 11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwiiiwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwiiiwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -2;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-3, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }

    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwiwww####");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("            e           ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("###########ww###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -1;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-2, 10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwww#  ##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwww#   ##");
        roomTemplate.push_back("##               #    ##");
        roomTemplate.push_back("                e   o ##");
        roomTemplate.push_back("##               #    ##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwww#   ##");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwww#  ##");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(20, 6), new Bubblecharm(pgame, Coordinate(20, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::WaterHorse);

        int difficulty = -2;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-1, 10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########ww###########");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("####wwwwwwwwwwwwww######");
        roomTemplate.push_back("##wwwwwwwwwwwwwww####w##");
        roomTemplate.push_back("## wwwwwwwwwwww####wiw##");
        roomTemplate.push_back("##wwwwwwwwwwww###wwwww##");
        roomTemplate.push_back("################wwwwww##");
        roomTemplate.push_back("##############wwwwwwww##");
        roomTemplate.push_back("###o   @              ##");
        roomTemplate.push_back("##o    @  e           ##");
        roomTemplate.push_back("####   @  e         ####");
        roomTemplate.push_back("#####  @            ####");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[4][2].size() > 1)
            {
                playSound(WavFile("StepOnSwitch", false, false));
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            for (int y = 8; y <= 11; y++)
            {
                deletionList.push_back(Coordinate(7, y));
            }

            for (auto i : deletionList)
            {
                EvilWallObject* wall = dynamic_cast<EvilWallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(2, 9), new Key(pgame, Coordinate(2, 9)));
        specificObjects.emplace(Coordinate(3, 8), new Speedboots(pgame, Coordinate(3, 8)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -2;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-2, 11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Find the hidden switch."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##wwwwwwwwwwwww       ##");
        roomTemplate.push_back("##wiwwwwwwwwww        ##");
        roomTemplate.push_back("##wwwwwwwwww          ##");
        roomTemplate.push_back("##wwwwwww             ##");
        roomTemplate.push_back("##www                 ##");
        roomTemplate.push_back("##                      ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("############ ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-4, 10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##     e        e     ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("###                 ####");
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[9][12].push_back(new Primary(
                pgame,
                ColorChar('{', dngutil::BROWN),
                Coordinate(12, 9),
                "Crossbow",
                false,
                1.4,
                5,
                85,
                true,
                "A very accurate and forceful bow",
                WavFile("BowAttack1", false, false),
                dngutil::ClassType::RANGER
            ));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -1;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-4, 11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing"), Mp3File("DungeonTheme")));
        mut.unlock();
    }

    tfloor = 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##          ############");
        roomTemplate.push_back("## I           #########");
        roomTemplate.push_back("##                ######");
        roomTemplate.push_back("##                 #####");
        roomTemplate.push_back("##                  ####");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("###                   ##");
        roomTemplate.push_back("#####                 ##");
        roomTemplate.push_back("#######           o  ###");
        roomTemplate.push_back("#########           ####");
        roomTemplate.push_back("###########        #####");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        //18,9
        specificObjects.emplace(Coordinate(18, 9), new Key(pgame, Coordinate(18, 9)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-4, 10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####o               ####");
        roomTemplate.push_back("####            I   ####");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                   o##");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("####   e        e   ####");
        roomTemplate.push_back("#####              #####");
        //4,1
        //21,9
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 1), new MagicalPotion(pgame, Coordinate(4, 1)));
        specificObjects.emplace(Coordinate(21, 9), new ManaPotion(pgame, Coordinate(21, 9), dngutil::MANA_POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-2, 10);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("#####              #####");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("####                 ###");
        roomTemplate.push_back("#####               I ##");
        roomTemplate.push_back("#######              ###");
        roomTemplate.push_back("##########         #####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-2, 11);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    | ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##         III        ##");
        roomTemplate.push_back("##         III        ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            deletionList.push_back(Coordinate(0, 6));
            deletionList.push_back(Coordinate(1, 6));
            for (auto i : deletionList)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-3, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Slay the enemy"), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("                 o    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][17].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][20].push_back(new HarpPiece(pgame, Coordinate(20, 6), 2));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::BloodJawPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::BloodJawPhase2)));
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::BloodJawPhase3)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(17, 6), new SegbossTrigger(
            pgame, Coordinate(17, 6),
            new Segboss(bossparts, pgame),
            ColorChar('@', dngutil::LIGHTBLUE)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -4;
        int backColor = dngutil::BLACK;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-2, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("######   ##### #      ##");
        roomTemplate.push_back("#### # # #     ###### ##");
        roomTemplate.push_back("##     # e # #   #### ##");
        roomTemplate.push_back("## ############# #### ##");
        roomTemplate.push_back("   #############        ");
        roomTemplate.push_back("## ############# #### ##");
        roomTemplate.push_back("## #o       #    #### ##");
        roomTemplate.push_back("## ####  #  #  #####  ##");
        roomTemplate.push_back("##    #  #  e  #     ###");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
      
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(4, 8), new Potion(pgame, Coordinate(4, 8), dngutil::POTION_HEAL));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-4, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##  I                   ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Mage);

        int difficulty = -2;
        int backColor = dngutil::BLUE;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-5, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }

    tfloor = 2;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##wwwww  ###############");
        roomTemplate.push_back("##wwiww     e     e o ##");
        roomTemplate.push_back("##wwwww  ###############");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        //20,6
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(20, 6), new Key(pgame, Coordinate(20, 6)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -3;
        int backColor = dngutil::CYAN;
        std::string name = "The Water Temple";
        Coordinate mapCoord(-5, 12);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
}

void Map::makeSpiritTemple(std::mutex& mut)
{
    unsigned int tfloor = dngutil::HOUSE_FLOOR - 1;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#                     ##");
        roomTemplate.push_back("# #  #  #  # #  #  #  ##");
        roomTemplate.push_back("# #                   ##");
        roomTemplate.push_back("###  #  #  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("     #  #  # #  #  #    ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   #  #  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##   #  #  # #  #  #  ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("############ ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(200, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("##         #          ##");
        roomTemplate.push_back("##  ###### # ######   ##");
        roomTemplate.push_back("##   #            #   ##");
        roomTemplate.push_back("##   #  #  #  e   #   ##");
        roomTemplate.push_back("##   #  #  #      #o  ##");
        roomTemplate.push_back("##   #  #  #############");
        roomTemplate.push_back("##   #  #        e    ##");
        roomTemplate.push_back("##         ##         ##");
        roomTemplate.push_back("###################   ##");
        roomTemplate.push_back("##     #        #     ##");
        roomTemplate.push_back("## o       ##         ##");
        roomTemplate.push_back("########################");
        // 7, 11
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            static int timer = 0;
            timer++;
            if (timer % 7500 == 0)
            {
                if (creatureList.front()->getPGame()->getActiveRoom()->getRoomInfo().backColor == dngutil::DARKGRAY)
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::BLACK);
                }
                else
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::DARKGRAY);
                }
            }

            if (gameMap[11][7].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::LIGHTGRAY);
        };

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(19, 5), new ManaPotion(pgame, Coordinate(19, 5), dngutil::MANA_POTION_HEAL));
        specificObjects.emplace(Coordinate(3, 11), new MagicalPotion(pgame, Coordinate(3, 11)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = -1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(200, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "The light is possessed."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("##wwwwwwwww  wwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwww  wwwwwwwww##");
        roomTemplate.push_back("                       |");
        roomTemplate.push_back("##wwwwwwwww  wwwwwwwww##");
        roomTemplate.push_back("##wwwwwwwww  wwwwwwwww##");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("###########  ###########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(201, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("###########  ###########");
        roomTemplate.push_back("##########w  w##########");
        roomTemplate.push_back("#########ww  ww#########");
        roomTemplate.push_back("####wwwwww    wwwwww####");
        roomTemplate.push_back("###ww     e  e     ww###");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            for (int x = 13; x <= 23; x++)
            {
                deletionList.push_back(Coordinate(x, 5));
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
                pgame->getVWin()->txtmacs.displayGame(pgame);
                Sleep(150);
            }
            stopSound(SoundType::WAV);
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(201, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("######@###@##@###@######");
        roomTemplate.push_back("##########@##@###@######");
        roomTemplate.push_back("##########@######@######");
        roomTemplate.push_back("#################@######");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWW WWW WW WWW WWWW##");
        roomTemplate.push_back("##WWW  WW  WW  WW  WWW##");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            // y = 11
            // x = 6, 10, 13, 17
            static bool first = false;
            static bool second = false;
            static bool third = false;
            static bool fourth = false;
            if (gameMap[11][6].size() == 2)
            {
                if (!first)
                {
                    playSound(WavFile("StepOnSwitch", false, false));
                }
                first = true;
                second = false;
                third = false;
                fourth = false;
            }
            else if (gameMap[11][13].size() == 2)
            {
                if (first && !third && !fourth)
                {
                    if (!second)
                    {
                        playSound(WavFile("StepOnSwitch", false, false));
                    }
                    second = true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            else if (gameMap[11][10].size() == 2)
            {
                if (first && second && !fourth)
                {
                    if (!third)
                    {
                        playSound(WavFile("StepOnSwitch", false, false));
                    }
                    third = true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            else if (gameMap[11][17].size() == 2)
            {
                if (first && second && third)
                {
                    return true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            std::vector<Coordinate> deletionList;
            deletionList.push_back(Coordinate(22, 6));
            deletionList.push_back(Coordinate(23, 6));
            for (auto i : deletionList)
            {
                WallObject* wall = dynamic_cast<WallObject*>(gameMap[i.y][i.x].back());

                if (wall == nullptr)
                {
                    errorMessage("error in puzzle", __LINE__, __FILE__);
                }

                gameMap[i.y][i.x].remove(wall);
                wall->removeFromMap(true);
            }
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(202, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {}
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####WWWWWWWWWWWWWWWW####");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("#WWWWWWWWWWWWWWWWWWWWWW#");
        roomTemplate.push_back("                  e     ");
        roomTemplate.push_back("#WWWWWWWWWWWWWWWWWWWWWW#");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("####WWWWWWWWWWWWWWWWW###");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][19].push_back(new Flute(pgame, Coordinate(19, 6)));
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::GhostHorse);

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(203, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Slay the enemy."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("##########     #########");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("                      ||");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("##########     #########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(204, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#########o     #########");
        roomTemplate.push_back("#########wwwwww#########");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("###wwwwwwwwwwwwwwwwww###");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("##wwwwwwwwwwwwwwwwwwww##");
        roomTemplate.push_back("###wwwwwwwwwwwwwwwwww###");
        roomTemplate.push_back("####wwwwwwwwwwwwwwww####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(9, 1), new MagicalPotion(pgame, Coordinate(9, 1)));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -3;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(205, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("                 o    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][17].size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][20].push_back(new HarpPiece(pgame, Coordinate(20, 6), 4));
        };

        std::vector<SegEnemy*> bossparts;
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::MegaBeastPhase1)));
        bossparts[0]->setFirst();
        bossparts.push_back(dynamic_cast<SegEnemy*>(pgame->generateCreature(1, dngutil::TID::MegaBeastPhase2)));

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(17, 6), new SegbossTrigger(
            pgame, Coordinate(17, 6),
            new Segboss(bossparts, pgame),
            ColorChar('*', dngutil::WHITE)
        ));

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -4;
        int backColor = dngutil::BLACK;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(206, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("############ ###########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("####        @       ####");
        roomTemplate.push_back("###        @@@       ###");
        roomTemplate.push_back("##        @ @ @       ##");
        roomTemplate.push_back("##          @         ##");
        roomTemplate.push_back("##          @         ##");
        roomTemplate.push_back("###                  ###");
        roomTemplate.push_back("####        @       ####");
        roomTemplate.push_back("##########     #########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        auto puzzleSolved = [this](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            return (gameMap[8][12].size() > 1);
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[2][12].push_back(new Key(pgame, Coordinate(12, 2)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(204, 199);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Complete it."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########   ##########");
        roomTemplate.push_back("###########    #########");
        roomTemplate.push_back("####     ####       ####");
        roomTemplate.push_back("###   e   ####       ###");
        roomTemplate.push_back("##         ####   e   ##");
        roomTemplate.push_back("##      ##v####       ##");
        roomTemplate.push_back("##      #######       ##");
        roomTemplate.push_back("###      #####       ###");
        roomTemplate.push_back("####                ####");
        roomTemplate.push_back("#######        #########");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -3;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(204, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("##WWW  WW  WW  WW  WWW##");
        roomTemplate.push_back("##WWW  WW  WW  WW  WWW##");
        roomTemplate.push_back("##WWW  WW  WW  WW  WWW##");
        roomTemplate.push_back("##WW                WW##");
        roomTemplate.push_back("##W                  W##");
        roomTemplate.push_back("                      ##");
        roomTemplate.push_back("##                   ###");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(202, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##w                  w##");
        roomTemplate.push_back("##  #              ## ##");
        roomTemplate.push_back("##                  # ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("## ##                 ##");
        roomTemplate.push_back("## ##              ## ##");
        roomTemplate.push_back("##w                  w##");
        roomTemplate.push_back("###########  ###########");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            static bool first = false;
            static bool second = false;
            static bool third = false;
            static bool fourth = false;
            if (gameMap[1][2].size() == 3)
            {
                if (!first)
                {
                    playSound(WavFile("StepOnSwitch", false, false));
                }
                first = true;
                second = false;
                third = false;
                fourth = false;
            }
            else if (gameMap[11][21].size() == 3)
            {
                if (first && !third && !fourth)
                {
                    if (!second)
                    {
                        playSound(WavFile("StepOnSwitch", false, false));
                    }
                    second = true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            else if (gameMap[1][21].size() == 3)
            {
                if (first && second && !fourth)
                {
                    if (!third)
                    {
                        playSound(WavFile("StepOnSwitch", false, false));
                    }
                    third = true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            else if (gameMap[11][2].size() == 3)
            {
                if (first && second && third)
                {
                    return true;
                }
                else
                {
                    if (!(!first && !second && !third && !fourth))
                    {
                        playSound(WavFile("PuzzleError", false, false));
                    }
                    first = false;
                    second = false;
                    third = false;
                    fourth = false;
                }
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][12].push_back(new Secondary(
                pgame,
                ColorChar('{', dngutil::LIGHTGREEN),
                Coordinate(12, 6),
                "Energy Shield",
                false,
                dngutil::TID::Secondary,
                150,
                1.2,
                "Fueled by the energy of the dead"
            ));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(201, 199);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {}
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("####         e #  # ####");
        roomTemplate.push_back("#### ### ##### #    ####");
        roomTemplate.push_back("#### # #     #   ## ####");
        roomTemplate.push_back("#### # ##### ###### ####");
        roomTemplate.push_back("       # e   ######     ");
        roomTemplate.push_back("#### # # #####    # ####");
        roomTemplate.push_back("#### # #     e #### ####");
        roomTemplate.push_back("#### # ##### # #### ####");
        roomTemplate.push_back("#### # #     #      ####");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::LSKnight);
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(199, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WW    WWWWWWWWWWWWWW##");
        roomTemplate.push_back("##W  X                  ");
        roomTemplate.push_back("##WW    WWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");  
        roomTemplate.push_back("##WWWWWWWWWWWWWWWWWWWW##");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;

        int difficulty = -1;
        int backColor = dngutil::LIGHTGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(198, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DungeonTheme")));
        mut.unlock();
    }

    tfloor--;
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##               e    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("##          e         ##");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("########################");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (creatureList.size() == 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            pgame->getActiveRoom()->setBackgroundColor(dngutil::DARKGRAY);
            gameMap[6][6].push_back(new ExitObject(pgame, Coordinate(5, 6), true, ColorChar('^', dngutil::WHITE)));
            gameMap[6][20].push_back(new Key(pgame, Coordinate(20, 6)));
        };

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -1;
        int backColor = dngutil::BLACK;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(198, 200);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing."), Mp3File("DungeonTheme")));
        mut.unlock();
    }
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("########################");
        roomTemplate.push_back("#           ######     #");
        roomTemplate.push_back("# #### #### ## ### # # #");
        roomTemplate.push_back("# ##  e   #     ## # # #");
        roomTemplate.push_back("# ## ##########e # #e# #");
        roomTemplate.push_back("# ##  ##      # ## # # #");
        roomTemplate.push_back("#  ## ####^##   #  #o# #");
        roomTemplate.push_back("## #  #   ### ### #### #");
        roomTemplate.push_back("#  ## ### #####   #    #");
        roomTemplate.push_back("## ##     ##    ### ####");
        roomTemplate.push_back("##  ######## ###### ## #");
        roomTemplate.push_back("###  e       ##o       #");
        roomTemplate.push_back("########################");
        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            static int timer = 0;
            timer++;
            int timerMod;
            if (creatureList.front()->getPGame()->getActiveRoom()->getRoomInfo().backColor == dngutil::DARKGRAY)
            {
                timerMod = 2500;
            }
            else
            {
                timerMod = 7500;
            }
            if (timer % timerMod == 0)
            {
                if (creatureList.front()->getPGame()->getActiveRoom()->getRoomInfo().backColor == dngutil::DARKGRAY)
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::BLACK);
                }
                else
                {
                    creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::DARKGRAY);
                }
            }

            if (gameMap[10][19].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            creatureList.front()->getPGame()->getActiveRoom()->setBackgroundColor(dngutil::LIGHTGRAY);
        };
        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(20, 6), new Potion(pgame, Coordinate(20, 6), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(15, 11), new Key(pgame, Coordinate(15, 11)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::BloodSkeleton);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = -3;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(204, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "The lights are possessed."), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DungeonTheme")));
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
                pgame->getVWin()->txtmacs.displayGame(pgame);
                Sleep(5);
            }
            stopSound(SoundType::WAV);
        };
        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Bowman);

        int difficulty = -3;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(2, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing."), Mp3File("DungeonTheme")));
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

        int difficulty = -5;
        int backColor = dngutil::BROWN;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(2, 1);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing."), Mp3File("DungeonTheme")));
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

        int difficulty = -5;
        int backColor = dngutil::BLACK;
        std::string name = "The Forest Temple";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Unknown origin"), Mp3File("DangerTheme")));
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
        specificObjects.emplace(Coordinate(15, 10), new ManaPotion(pgame, Coordinate(15, 10), dngutil::MANA_POTION_HEAL));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Slay the enemy."), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction, "Be the last one standing"), Mp3File("DungeonTheme")));
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