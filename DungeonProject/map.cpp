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
    functions.emplace_back([&]() {makeSpiritTemple(mut); });
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
        possibleCreatures.push_back(dngutil::TID::Bowman);
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
                "Rylan Wadkins",
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
        int backColor = dngutil::YELLOW;
        std::string name = "Glorian Beach";
        Coordinate mapCoord(-1, 4);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("OverworldTheme")));
        mut.unlock();
    }

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
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("#                       ");
        roomTemplate.push_back("##                    ##");
        roomTemplate.push_back("####                ####");
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
        roomTemplate.push_back("#####     e     ########");
        roomTemplate.push_back("###^              ######");
        roomTemplate.push_back("####             #######");
        roomTemplate.push_back("#####     e      #######");
        roomTemplate.push_back("########################"); 
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");
        roomTemplate.push_back("########################");

        std::map<Coordinate, MapObject*> specificObjects;

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
        roomTemplate.push_back("     0           #####  ");
        roomTemplate.push_back("       E        ######  ");
        roomTemplate.push_back("     0          ######  ");
        roomTemplate.push_back("                        ");
        roomTemplate.push_back("  ######          0     ");
        roomTemplate.push_back("  ######                ");
        roomTemplate.push_back("  #####o          0     ");
        roomTemplate.push_back("  ######                ");
        roomTemplate.push_back("  ######          0     ");
        roomTemplate.push_back("                        ");

        std::map<Coordinate, MapObject*> specificObjects;
        std::vector<ColorString> dialogue;
        dialogue.push_back(ColorString("I'm lost... Have you seen my son - Rylan?", dngutil::WHITE));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("SpiritTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("SpiritTheme")));
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

        std::map<Coordinate, MapObject*> specificObjects;
        specificObjects.emplace(Coordinate(7, 6), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(7, 6),
            "John Baker",
            ColorString("This is really wonderful weather we are having!", dngutil::WHITE)
        ));

        specificObjects.emplace(Coordinate(17, 10), new Npc(
            pgame,
            ColorChar('A', dngutil::WHITE),
            Coordinate(17, 10),
            "Josh Chan",
            ColorString("This altar used to be the site of many important rituals.", dngutil::WHITE)
        ));

        std::vector<ColorString> dialogue;
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("SpiritTheme")));
        mut.unlock();

    }

    tfloor = 2;
    // The Boranian Valley
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####  e               @@");
        roomTemplate.push_back("####                  @@");
        roomTemplate.push_back("####                  @@");
        roomTemplate.push_back("####     e       e    @@");
        roomTemplate.push_back("#####                 @@");
        roomTemplate.push_back("######                @@");
        roomTemplate.push_back("####v                 @@");
        roomTemplate.push_back("######        e       @@");
        roomTemplate.push_back("#####                 @@");
        roomTemplate.push_back("####                  @@");
        roomTemplate.push_back("####              e   @@");
        roomTemplate.push_back("####     e            @@");
        roomTemplate.push_back("####                  @@");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
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
        roomTemplate.push_back("####      e       e   @@");
        roomTemplate.push_back("####                  @@");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
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
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");
        roomTemplate.push_back("@@@@@@@@@@@@@@@@@@@@@@@@");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
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
        roomTemplate.push_back("@@                    ##");
        roomTemplate.push_back("@@             e      ##");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
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
        roomTemplate.push_back("@@                    ##");
        roomTemplate.push_back("@@         e          ##");
        roomTemplate.push_back("@@@               e   ##");
        roomTemplate.push_back("@@@@ e                ##");
        roomTemplate.push_back("@@@@@       ww ww     ##");
        roomTemplate.push_back("@@@@@@e     w# #w     ##");
        roomTemplate.push_back("    5         +       ##");
        roomTemplate.push_back("@@@@@@e     w# #w     ##");
        roomTemplate.push_back("@@@@@       ww ww     ##");
        roomTemplate.push_back("@@@@                  ##");
        roomTemplate.push_back("@@@                   ##");
        roomTemplate.push_back("@@                    ##");
        roomTemplate.push_back("@@             e      ##");

        auto puzzleSolved = [](const std::list<Creature*>& creatureList, const GAMEMAP& gameMap) -> bool
        {
            if (gameMap[6][2].size() > 1)
            {
                return true;
            }
            return false;
        };

        auto puzzleAction = [this](std::list<Creature*> creatureList, GAMEMAP& gameMap) -> void
        {
            gameMap[6][3].push_back(new EvilWallObject(pgame, Coordinate(3, 6)));
        };


        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);
        possibleCreatures.push_back(dngutil::TID::LSKnight);

        int difficulty = 12;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(6, -3);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DangerTheme")));
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

        int difficulty = 12;
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

        int difficulty = 12;
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

        int difficulty = 12;
        int backColor = dngutil::BROWN;
        std::string name = "The Boranian Valley - Outside the Tower of the Gods";
        Coordinate mapCoord(4, -2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("DangerTheme")));
        mut.unlock();

    }

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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("SpiritTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr, Mp3File("SpiritTheme")));
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
        specificObjects.emplace(Coordinate(19, 5), new Potion(pgame, Coordinate(19, 5), dngutil::POTION_HEAL));
        specificObjects.emplace(Coordinate(3, 11), new MagicalPotion(pgame, Coordinate(3, 11)));

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::SSKnight);

        int difficulty = -1;
        int backColor = dngutil::DARKGRAY;
        std::string name = "The Spirit Temple";
        Coordinate mapCoord(200, 201);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DangerTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DungeonTheme")));
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
                pgame->getVWin()->txtmacs.displayGame(pgame);
                Sleep(50);
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
        std::string name = "The Forest Temple";
        Coordinate mapCoord(0, 2);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);
        mut.lock();
        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, new Puzzle(puzzleSolved, puzzleAction), Mp3File("DangerTheme")));
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