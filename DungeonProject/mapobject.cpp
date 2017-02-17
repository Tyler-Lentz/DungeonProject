#include "mapobject.h"
#include "utilities.h"
#include "game.h"
#include "room.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "enemy.h"

#include <list>

//---------------------------------------------------------------
// MapObject functions

MapObject::MapObject(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId,
    int priority,
    dngutil::BTID bTypeId,
    bool prematureCheck
)
    :coord(coord.x, coord.y)
{
    this->pgame = pgame;
    this->mapRep = mapRep;
    this->name = name;
    this->moveable = moveable;
    this->rawoutput = rawoutput;
    this->aggressive = aggressive;
    this->typeId = typeId;
    this->priority = priority;
    this->bTypeId = bTypeId;
    this->prematureCheck = prematureCheck;
}

void MapObject::setPosition(Coordinate coord)
{
    this->coord = coord;
}

const Coordinate& MapObject::getCoord() const
{
    return coord;
}

const ColorChar& MapObject::getMapRep() const
{
    return mapRep;
}

const std::string& MapObject::getName() const
{
    return name;
}

const bool& MapObject::isMoveable() const
{
    return moveable;
}

const bool& MapObject::isRawoutput() const
{
    return rawoutput;
}

const bool& MapObject::isAggressive() const
{
    return aggressive;
}

const bool& MapObject::hasPrematureCheck() const
{
    return prematureCheck;
}

const dngutil::TID& MapObject::getTypeId() const
{
    return typeId;
}

const dngutil::BTID& MapObject::getBTypeId() const
{
    return bTypeId;
}

const int& MapObject::getPriority() const
{
    return priority;
}

Game* MapObject::getPGame()
{
    return pgame;
}

void MapObject::removeFromMap(bool deleteit)
{
    getPGame()->getActiveRoom()->addCoordToList(coord);
    coord = Coordinate(-1, -1);
    if (deleteit)
    {
        getPGame()->getDeletionList().push_back(this);
    }
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Exit Object Functions

Collision ExitObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        if (up)
        {
            soundEffect("StairsUp.wav", false, false);
        }
        else
        {
            soundEffect("StairsDown.wav", false, false);
        }
        Coordinate coord(getPGame()->getActiveRoom()->getRoomInfo().mapCoord);
        if (up)
        {
            getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor + 1);
            if (getPGame()->getActiveFloor().count(coord) == 1)
            {
                if (
                    getPGame()->getActiveFloor()[coord]->checkMovement(collider->getCoord(),
                    getPGame()->getPlayer()) == dngutil::MovementTypes::VALID
                   )
                {
                    getPGame()->getActiveRoom()->getObjects(collider->getCoord()).remove(collider);
                    getPGame()->setActiveRoom(getPGame()->getActiveFloor()[coord]);
                    getPGame()->getActiveRoom()->getObjects(collider->getCoord()).push_back(collider);
                    getPGame()->getVWin()->txtmacs.displayGame(getPGame());
                    getPGame()->clearDeletionList();
                    return Collision(true, true, true);
                }
                else
                {
                    getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor - 1);
                }
            }
            else
            {
                getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor - 1);
            }
        }
        else
        {
            getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor - 1);
            if (getPGame()->getActiveFloor().count(coord) == 1)
            {
                if (
                    getPGame()->getActiveFloor()[coord]->checkMovement(collider->getCoord(),
                    getPGame()->getPlayer()) == dngutil::MovementTypes::VALID
                   )
                {
                    getPGame()->getActiveRoom()->getObjects(collider->getCoord()).remove(collider);
                    getPGame()->setActiveRoom(getPGame()->getActiveFloor()[coord]);
                    getPGame()->getActiveRoom()->getObjects(collider->getCoord()).push_back(collider);
                    getPGame()->getVWin()->txtmacs.displayGame(getPGame());
                    getPGame()->clearDeletionList();
                    return Collision(true, true, true);
                }
                else
                {
                    getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor + 1);
                }
            }
            else
            {
                getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor + 1);
            }
        }
    }
    return Collision(false, true);
}
//---------------------------------------------------------------

//---------------------------------------------------------------
// HoleObject Functions
HoleObject::HoleObject(Game* game, Coordinate coord) :
    MapObject(
        game,
        ColorChar(' ', getColor(dngutil::BLACK, dngutil::BLACK)),
        coord,
        "HOLE",
        true,
        true,
        false,
        dngutil::TID::Hole,
        dngutil::P_STAIRCASE,
        dngutil::BTID::None,
        true
    ) {}

Collision HoleObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        Coordinate coord(getPGame()->getActiveRoom()->getRoomInfo().mapCoord);

        getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor - 1);
        if (getPGame()->getActiveFloor().count(coord) == 1)
        {
            Coordinate coordToTest = collider->getCoord();
            Creature* temp = dynamic_cast<Creature*>(collider);
            switch (temp->getLastMovement())
            {
            case dngutil::Movement::UP:
                coordToTest.y--;
                break;
            case dngutil::Movement::DOWN:
                coordToTest.y++;
                break;
            case dngutil::Movement::LEFT:
                coordToTest.x--;
                break;
            case dngutil::Movement::RIGHT:
                coordToTest.x++;
                break;
            }

            if (getPGame()->getActiveFloor()[coord]->checkMovement(coordToTest, getPGame()->getPlayer()) == dngutil::MovementTypes::VALID)
            {
                getPGame()->getActiveRoom()->getObjects(collider->getCoord()).remove(collider);
                getPGame()->setActiveRoom(getPGame()->getActiveFloor()[coord]);
                collider->setPosition(coordToTest);
                getPGame()->getActiveRoom()->getObjects(collider->getCoord()).push_back(collider);
                getPGame()->getVWin()->txtmacs.displayGame(getPGame());
                getPGame()->clearDeletionList();
                getPGame()->getVWin()->txtmacs.fallingScreen(getPGame());
                return Collision(true, true, false);
            }
            else
            {
                getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor + 1);
            }
        }
        else
        {
            getPGame()->setActiveFloor(getPGame()->getActiveRoom()->getRoomInfo().floor + 1);
        }
    }
    return Collision(false, true);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// DoorObject Functions
DoorObject::DoorObject(Game* game, Coordinate coord, ColorChar mapRep) :
    MapObject(
        game,
        mapRep,
        coord,
        "LOCKEDDOOR",
        false,
        false,
        false,
        dngutil::TID::LockedDoor,
        dngutil::P_WALL,
        dngutil::BTID::None,
        true
    ) {}

Collision DoorObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    // dont use the it passed through in this function
    if (collider == getPGame()->getPlayer())
    {
        Player* p = getPGame()->getPlayer();
        for (auto itr = p->getInventory().begin(); itr != p->getInventory().end(); itr++)
        {
            if ((*itr)->getTypeId() == dngutil::TID::Key)
            {
                soundEffect("UnlockDoor.wav", false, false);
                Item* key = (*itr);
                p->getInventoryNotConst().erase(itr);
                delete key;
                getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
                removeFromMap(true);
                return Collision(true, false, true);
            }
        }
        return Collision(false, true, false);
    }
    else
    {
        return Collision(false, true, false);
    }
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// altar object functions

AltarObject::AltarObject(Game* game, Coordinate coord)
    :MapObject(
        game,
        ColorChar('+', dngutil::WHITE),
        coord,
        "ALTAR",
        true,
        false,
        false,
        dngutil::TID::Altar,
        dngutil::P_ITEM,
        dngutil::BTID::None,
        false
    )
{

}

Collision AltarObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        stopMp3();
        VirtualWindow* v = getPGame()->getVWin();
        TextMacros& t = v->txtmacs;

        t.clearMapArea(true, 10);
        t.clearDivider("bottom");
        int background = dngutil::DARKGRAY;
        int cross = dngutil::WHITE;

        Coordinate vcursor(15, t.DIVIDER_LINES[1] + 4);
        v->putcen(ColorString(R"(	            .----------------------------._              )", background), vcursor.y++);
        v->putcen(ColorString(R"(           _.-'          '-        .           '-._         )", background), vcursor.y++);
        v->putcen(ColorString(R"(         .'      _|   .    . - .        ._         '.       )", background), vcursor.y++);
        v->putcen(ColorString(R"(      _.'    '           .'     '.               _| |       )", background), vcursor.y++);
        v->putcen(ColorString(R"(     /  _|        _|    ''       ''  |_     '    .  '.      )", background), vcursor.y++);
        v->putcen(ColorString(R"(    |      . -- .      ''         ''      . -- .     |      )", background), vcursor.y++);
        v->putcen(ColorString(R"(   .'    .'      '.   -||         ||    .'      '.   '.     )", background), vcursor.y++);
        v->putcen(ColorString(R"(   | '  ''        ''   ||   )", background) + ColorString(".-.", cross) + ColorString(R"(   ||_  ''        ''   |     )", background), vcursor.y++);
        v->putcen(ColorString(R"(   '.  ''          ''  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   ||  ''          ''  |     )", background), vcursor.y++);
        v->putcen(ColorString(R"(    | -||          ||- ')", background) + ColorString("____|!|____", cross) + ColorString(R"(' -||          ||- |     )", background), vcursor.y++);
        v->putcen(ColorString(R"(    |  ||          ||  )", background) + ColorString("|____-+-____|", cross) + ColorString(R"(  ||          ||  '.    )", background), vcursor.y++);
        v->putcen(ColorString(R"(   .' -||          ||_ ||   )", background) + ColorString("|!|", cross) + ColorString(R"(   ||  ||          ||  _|    )", background), vcursor.y++);
        v->putcen(ColorString(R"(   |_.-||          ||  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   || _||          ||-._|    )", background), vcursor.y++);
        v->putcen(ColorString(R"(_.-' |_||          ||  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   ||  ||          ||_| '-._ )", background), vcursor.y++);
        v->putcen(ColorString(R"(_| |_  |:;;.,::;,.';|--|:;;.)", background) + ColorString("| |", cross) + ColorString(R"(,.';|--|:;;.,::;,.';|     |_ )", background), vcursor.y++);
        v->putcen(ColorString(R"(         :;;.,::;,.';   :;;.)", background) + ColorString("| |", cross) + ColorString(R"(,.';    :;;.,::;,.';  _|   -')", background), vcursor.y++);
        v->putcen(ColorString(R"(   |_                       )", background) + ColorString("| |", cross) + ColorString(R"(                         |_. )", background), vcursor.y++);
        v->putcen(ColorString(R"( _      _|                __)", background) + ColorString("| |", cross) + ColorString(R"(__              |_     _     )", background), vcursor.y++);
        v->putcen(ColorString(R"(|________________________/_______\___________________|______)", background), vcursor.y++);
        v->putcen(ColorString(R"(,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.)", background), vcursor.y++);
        v->putcen(ColorString(R"(------------------------------------------------------------)", background), vcursor.y++);

        startMp3("AltarSong.mp3");
        
        for (int i = 0; i < 800; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }

        stopMp3();
        
        getPGame()->getPlayer()->resetSteps();

        vcursor.y = t.BOTTOM_DIVIDER_TEXT_LINE;
        v->putcen(ColorString("Your prayers have warded away the beast", dngutil::WHITE), vcursor.y++);
        pressEnter(vcursor, v);
        t.clearDivider("bottom");
        t.clearMapArea(false, NULL);
        t.displayGame(getPGame());

        startMp3(getPGame()->getOverworldMusic());

    }
    return Collision(false, true);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// beast trigger
BeastTrigger::BeastTrigger(Game* game, Coordinate coord)
    :MapObject(
        game,
        ColorChar(' ', dngutil::BACKGROUND_COLOR),
        coord,
        "beasttrigger",
        true,
        false,
        false,
        dngutil::TID::BeastTrigger,
        dngutil::P_EMPTY,
        dngutil::BTID::None,
        false
    )
{

}
Collision BeastTrigger::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        getPGame()->getPlayer()->dungeonBeastSequence();
        getPGame()->setBeastSpawn(true);
        getPGame()->getPlayer()->resetSteps();
        it++;
        getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
        removeFromMap(true);
        getPGame()->getVWin()->txtmacs.displayGame(getPGame());
        return Collision(false, false);
    }
    return Collision(false, true);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Segboss trigger
SegbossTrigger::SegbossTrigger(Game* game, Coordinate coord, Segboss* segboss, ColorChar colchar)
    :MapObject(
        game,
        colchar,
        coord,
        "SEGBOSSTRIGGER",
        true,
        false,
        false,
        dngutil::TID::SegbossTrigger,
        dngutil::P_ENEMY,
        dngutil::BTID::None,
        false
    )
{
        this->segboss = segboss;
}

SegbossTrigger::~SegbossTrigger()
{
    delete segboss;
}

Collision SegbossTrigger::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        stopMp3();
        soundEffect("EnterBattle.wav", false, true);
        getPGame()->getVWin()->txtmacs.clearMapArea(true, 35);
        segboss->segmentedBattle(getPGame()->getPlayer());
        return Collision(true, true, true);
    }
    return Collision(false, true);
}
//---------------------------------------------------------------