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

    overworldMusic = Mp3File("OverworldTheme");

    gamespace.resize(15);

    unsigned int tfloor = 2;
    pgame->setActiveFloor(tfloor); // sets starting floor
    {
        std::vector<std::string> roomTemplate;
        roomTemplate.push_back("####    ###    ");
        roomTemplate.push_back("              #");
        roomTemplate.push_back("              #");
        roomTemplate.push_back("#     +        ");
        roomTemplate.push_back("               ");
        roomTemplate.push_back("       A      #");
        roomTemplate.push_back("#             #");
        roomTemplate.push_back("               ");
        roomTemplate.push_back("    ### ##     ");

        std::map<Coordinate, MapObject*> specificObjects;

        std::vector<dngutil::TID> possibleCreatures;
        possibleCreatures.push_back(dngutil::TID::Skeleton);

        int difficulty = 0;
        int backColor = dngutil::BROWN;
        std::string name = "The Begining";
        Coordinate mapCoord(0, 0);
        RoomInfo rminfo(roomTemplate, specificObjects, name, difficulty, backColor, possibleCreatures, tfloor, mapCoord);

        gamespace[tfloor].emplace(mapCoord, new Room(pgame, rminfo, nullptr));
        pgame->setActiveRoom(gamespace[tfloor][mapCoord]); // sets starting active room
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