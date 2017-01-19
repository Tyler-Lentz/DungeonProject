#include "room.h"
#include "game.h"
#include "mapobject.h"
#include "creature.h"
#include "item.h"
#include "utilities.h"
#include "player.h"
#include "virtualwindow.h"

#include <array>
#include <list>

//---------------------------------------------------------------
// Room Functions

Room::Room(const Room& other, Game* game)
    :roomInfo(other.roomInfo)
{
    for (unsigned int y = 0; y < dngutil::MAPSIZE; y++)
    {
        for (unsigned int x = 0; x < dngutil::MAPSIZE; x++)
        {
            Coordinate coord(x, y);
            for (auto it = other.gameMap[y][x].begin(); it != other.gameMap[y][x].end(); it++)
            {
                MapObject* object = nullptr;

                if ((*it)->getTypeId() != dngutil::TID::Player)
                {
                    object = (*it)->makeSave(game);
                }
                else
                {
                    object = game->getPlayer();
                }

                if ((*it)->getBTypeId() == dngutil::BTID::Creature)
                {
                    Creature* creature = dynamic_cast<Creature*>(object);
                    creatureList.push_back(creature);
                }
                else if ((*it)->getBTypeId() == dngutil::BTID::Item)
                {
                    objects.emplace(coord, object);
                }


                sortPriority(this->gameMap[y][x], object);
            }
        }
    }

    this->adjustedPositions = other.adjustedPositions;

    this->roomX = other.roomX;
    this->roomY = other.roomY;

    this->game_pointer = game;

    this->puzzle = other.puzzle;
}

Room::Room(Game* t_game_pointer, RoomInfo roomToGenerate, Puzzle* puzzle)
    :roomInfo(roomToGenerate)
{
    this->puzzle = puzzle;

    game_pointer = t_game_pointer;

    roomX = roomInfo.roomTemplate[0].size();
    roomY = roomInfo.roomTemplate.size();

    // Assigning an empty position to everything
    Coordinate coord(0, 0);
    for (; coord.x < roomX; coord.x++)
    {
        coord.y = 0;
        for (; coord.y < roomY; coord.y++)
        {
            getObjects(coord).push_back(new EmptyObject(game_pointer, coord));
        }
    }

    for (int i = 0; i < roomY; i++)
    {
        for (int j = 0; j < roomX; j++)
        {
            Creature* creature = nullptr;
            switch (roomInfo.roomTemplate[i][j])
            {
            case '#':
                gameMap[i][j].push_back(new WallObject(game_pointer, Coordinate(j, i)));
                break;

            case 'A':
                gameMap[i][j].push_back(game_pointer->getPlayer());
                addCreature(game_pointer->getPlayer(), Coordinate(j, i));

                if (game_pointer->getActiveRoom() != nullptr)
                {
                    errorMessage("Player is being initialized more than once.", __LINE__, __FILE__);
                }

                game_pointer->setActiveRoom(this);
                break;

            case 'e':
                dngutil::TID tid;
                tid = roomInfo.possibleCreatures[random(roomInfo.possibleCreatures.size() - 1)];
                creature = game_pointer->generateCreature(roomInfo.difficulty, tid);
                gameMap[i][j].push_back(creature);
                addCreature(creature, Coordinate(j, i));
                break;

            case '^':
                gameMap[i][j].push_back(new ExitObject(game_pointer, Coordinate(j, i), true, ColorChar('^', dngutil::BROWN)));
                break;

            case 'v':
                gameMap[i][j].push_back(new ExitObject(game_pointer, Coordinate(j, i), false, ColorChar('v', dngutil::BROWN)));
                break;

            case 'o':
                if (roomInfo.specificObjects.count(Coordinate(j, i)) != 1)
                {
                    errorMessage(
                        "ERROR: on floor " + std::to_string(roomInfo.floor) + " " + roomInfo.name + ", item not set correctly",
                        __LINE__, __FILE__);
                }
                gameMap[i][j].push_back(roomInfo.specificObjects[Coordinate(j, i)]);
                roomInfo.specificObjects.erase(Coordinate(j, i));
                break;

            case '-':
            case '|':
                gameMap[i][j].push_back(new DoorObject(
                    game_pointer, Coordinate(j, i), ColorChar(roomInfo.roomTemplate[i][j], dngutil::BROWN)
                ));
                break;

            case 'X':
                gameMap[i][j].push_back(new HoleObject(game_pointer, Coordinate(j, i)));
                break;
            }
        }
    }
}

Room::~Room()
{
    Coordinate coord(0, 0);
    for (; coord.x < dngutil::MAPSIZE; coord.x++)
    {
        coord.y = 0;
        for (; coord.y < dngutil::MAPSIZE; coord.y++)
        {
            for (std::list<MapObject*>::iterator it = getObjects(coord).begin(); it != getObjects(coord).end(); it++)
            {
                if (*it != game_pointer->getPlayer())
                {
                    delete (*it);
                }
            }
            getObjects(coord).clear();
        }
    }

    if (puzzle != nullptr)
    {
        delete puzzle;
    }
}

int Room::getRoomX() const
{
    return roomX;
}

int Room::getRoomY() const
{
    return roomY;
}

dngutil::MovementTypes Room::checkMovement(Coordinate coord, Creature* creature) const
{
    bool onMap = ((coord.x >= 0 && coord.x <= roomX - 1) && (coord.y >= 0 && coord.y <= roomY - 1));
    if (onMap)
    {
        if (gameMap[coord.y][coord.x].back()->hasPrematureCheck())
        {
            // this doesnt do anything but it needs to be passed through
            std::list<MapObject*>::iterator it;
            *it = nullptr;
            Collision col = gameMap[coord.y][coord.x].back()->mapAction(creature, it);
            if (col.returnTrue)
            {
                return dngutil::MovementTypes::VALID;
            }
        }
    }

    if (!onMap && creature == game_pointer->getPlayer())
    {
        return dngutil::MovementTypes::NEW_ROOM;
    }
    else if (!onMap && creature != game_pointer->getPlayer())
    {
        return dngutil::MovementTypes::INVALID;
    }

    for (std::list<MapObject*>::const_iterator it = gameMap[coord.y][coord.x].begin(); it != gameMap[coord.y][coord.x].end(); it++)
    {
        if (!(*it)->isMoveable())
        {
            return dngutil::MovementTypes::INVALID;
        }
    }

    return dngutil::MovementTypes::VALID;
}

void Room::drawRoom()
{
    while (!adjustedPositions.empty())
    {
        Coordinate vcursor(0, game_pointer->getVWin()->txtmacs.DIVIDER_LINES[1] + 1);
        int emptyWidth = ((dngutil::CONSOLEX - game_pointer->getActiveRoom()->getRoomX()) / 2);
        vcursor.x += emptyWidth;

        Coordinate coord = adjustedPositions.front();
        adjustedPositions.pop_front();

        vcursor.x += coord.x;
        vcursor.y += coord.y;

        ColorChar cchar = getObjects(coord).back()->getMapRep();

        if (cchar.color == dngutil::BACKGROUND_COLOR)
        {
            cchar.color = roomInfo.backColor;
        }

        if (!getObjects(coord).back()->isRawoutput())
        {
            cchar.color = getColor(cchar.color, roomInfo.backColor);
        }

        int creatureNum = 0;
        for (auto it = getObjects(coord).begin(); it != getObjects(coord).end(); it++)
        {
            if ((*it)->getBTypeId() == dngutil::BTID::Creature)
            {
                creatureNum++;
            }
        }

        if (creatureNum > 9)
        {
            creatureNum = 9;
        }

        if (creatureNum > 1)
        {
            cchar.character = creatureNum + '0';
        }

        game_pointer->getVWin()->put(cchar, vcursor);
    }

    game_pointer->getVWin()->txtmacs.displayOverworldInfo(game_pointer);
}

void Room::addCoordToList(Coordinate coord)
{
    adjustedPositions.push_back(coord);
}

void Room::setAll()
{
    for (int y = 0; y < roomY; y++)
    {
        for (int x = 0; x < roomX; x++)
        {
            adjustedPositions.push_back(Coordinate(x, y));
        }
    }
}

bool Room::eraseItem(Coordinate coord)
{
    if (objects.count(coord) == 0)
    {
        return false;
    }

    objects.erase(coord);
    return true;
}

std::list<MapObject*>& Room::getObjects(Coordinate coord)
{
    return gameMap[coord.y][coord.x];
}

std::list<Creature*>& Room::getCreatureList()
{
    return creatureList;
}

void Room::addCreature(Creature* type, Coordinate coord)
{
    creatureList.push_back(type);
    type->setPosition(coord);
}

const RoomInfo& Room::getRoomInfo() const
{
    return roomInfo;
}

const std::list<Coordinate>& Room::getAdjustedPositions() const
{
    return adjustedPositions;
}

const Puzzle& Room::getPuzzle() const
{
    return *puzzle;
}

const GAMEMAP& Room::getGameMap() const
{
    return gameMap;
}

GAMEMAP& Room::getGameMapNotConst()
{
    return gameMap;
}

void Room::setPuzzleAsSolved()
{
    delete puzzle;
    puzzle = nullptr;
}

//---------------------------------------------------------------