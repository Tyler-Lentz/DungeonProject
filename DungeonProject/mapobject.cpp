#include "mapobject.h"
#include "utilities.h"
#include "game.h"
#include "room.h"
#include "creature.h"
#include "player.h"
#include "virtualwindow.h"
#include "enemy.h"
#include "savefile.h"

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

void MapObject::changeMapChar(ColorChar Char)
{
    mapRep = Char;
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

void MapObject::setName(std::string name)
{
    this->name = name;
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Exit Object Functions

Collision ExitObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        if (itemNeeded == dngutil::TID::Empty)
        {
            if (up)
            {
                playSound(WavFile("StairsUp", false, false));
            }
            else
            {
                playSound(WavFile("StairsDown", false, false));
            }
        }
        else
        {
            if (!getPGame()->getPlayer()->hasItem(itemNeeded, true))
            {
                return Collision(false, true);
            }
            else
            {
                if (itemNeeded == dngutil::TID::Bubblecharm)
                {
                    playSound(WavFile("WaterStaircase", false, false));
                }
            }
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
                    if (getPGame()->getActiveRoom()->hasPuzzle())
                    {
                        playSound(WavFile("Puzzle", false, true));
                    }
                    if (getPGame()->getOverworldMusic().getFilename() != getPGame()->getActiveRoom()->getMusic().getFilename())
                    {
                        // if this changes change it where it is in adjustposition and down further and  in holeobject
                        getPGame()->setOverworldMusic(getPGame()->getActiveRoom()->getMusic());
                        stopSound(SoundType::MP3);
                        getPGame()->getOverworldMusic().play();
                    }
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
                    if (getPGame()->getActiveRoom()->hasPuzzle())
                    {
                        playSound(WavFile("Puzzle", false, true));
                    }
                    if (getPGame()->getOverworldMusic().getFilename() != getPGame()->getActiveRoom()->getMusic().getFilename())
                    {
                        // if this changes change it where it is in adjustposition and up further
                        getPGame()->setOverworldMusic(getPGame()->getActiveRoom()->getMusic());
                        stopSound(SoundType::MP3);
                        getPGame()->getOverworldMusic().play();
                    }
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
                if (getPGame()->getActiveRoom()->hasPuzzle())
                {
                    playSound(WavFile("Puzzle", false, true));
                }
                if (getPGame()->getOverworldMusic().getFilename() != getPGame()->getActiveRoom()->getMusic().getFilename())
                {
                    // if this changes change it where it is in adjustposition and down further
                    getPGame()->setOverworldMusic(getPGame()->getActiveRoom()->getMusic());
                    stopSound(SoundType::MP3);
                    getPGame()->getOverworldMusic().play();
                }
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
// Outside Object Functions
Water::Water(Game* game, Coordinate coord, bool deep) :
    MapObject(
        game,
        ColorChar(' ', getColor(dngutil::BLUE, dngutil::BLUE)),
        coord,
        "OUTSIDE",
        false,
        true,
        false,
        dngutil::TID::Water,
        dngutil::P_EMPTY,
        dngutil::BTID::None,
        true
    ) 
{
    this->deep = deep;
}

Collision Water::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    Player* p = getPGame()->getPlayer();
    // dont use the it passed through in this function
    if (collider == p)
    {
        if (p->getBootsMemory()->getTypeId() == dngutil::TID::Waterboots && !deep)
        {
            return Collision(true, false, true);
        }
    }
    return Collision(false, true, false);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Tree Object Functions
TreeObject::TreeObject(Game* game, Coordinate coord) :
    MapObject(
        game,
        ColorChar('o', dngutil::BLACK),
        coord,
        "TREE",
        false,
        false,
        false,
        dngutil::TID::Tree,
        dngutil::P_EMPTY,
        dngutil::BTID::None,
        true
    ) {}
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
                playSound(WavFile("UnlockDoor", false, false));
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
// Smashable Rock Functions
SmashableRockObject::SmashableRockObject(Game* game, Coordinate coord) :
    MapObject(
        game,
        ColorChar(static_cast<unsigned char>(178),getColor(dngutil::BROWN, dngutil::YELLOW)),
        coord,
        "SMASHABLEROCK",
        false,
        true,
        false,
        dngutil::TID::SmashableRock,
        dngutil::P_WALL,
        dngutil::BTID::None,
        true
    ) {}

Collision SmashableRockObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    // dont use the it passed through in this function
    if (collider == getPGame()->getPlayer() && getPGame()->getPlayer()->hasItem(dngutil::TID::PowerBoots, true))
    {
        getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
        removeFromMap(true);
        playSound(WavFile("UnlockDoor", false, false));
        return Collision(true, false, true);
    }
    else
    {
        return Collision(false, true, false);
    }
}

//---------------------------------------------------------------


//---------------------------------------------------------------
// DungeonCheck Functions
DungeonCheck::DungeonCheck(Game* game, Coordinate coord, int harp) :
    MapObject(
        game,
        ColorChar(' ', dngutil::BACKGROUND_COLOR),
        coord,
        "DUNGEONCHECK",
        false,
        false,
        false,
        dngutil::TID::DungeonCheck,
        dngutil::P_EMPTY,
        dngutil::BTID::None,
        true
    ) 
{
    harpNumber = harp;
    allow5Cross = true;
}

Collision DungeonCheck::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    // dont use the it passed through in this function
    if (collider == getPGame()->getPlayer())
    {
        if (harpNumber == 5)
        {
            if (getPGame()->getPlayer()->hasFullHarp() && allow5Cross)
            {
                getPGame()->getPlayer()->setDungeonStart();
                getPGame()->setDungeonLevel(getPGame()->getPlayer()->getLvl());

                stopSound(SoundType::MP3);
                VirtualWindow* t = getPGame()->getVWin();
                Coordinate vcursor(0, t->txtmacs.DIVIDER_LINES[1] + 5);
                t->txtmacs.clearMapArea(true, 5);
                t->txtmacs.clearDivider("bottom");
                t->putcen(ColorString(R"(         ____                   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(         SSSS____.              )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(         (WW);;;;;\             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(         `WW'____ |     ,_____  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||||\ \___/,---. ) )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |||||\____/||| //  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||||||||||||" //   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |||||||||||' //    )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |||||||||"  //     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||||||||'  //      )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |||||||"  //       )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||||||'  //        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||||"   //         )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |||"   //          )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU ||'   //           )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(          UU |"   //            )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(         ,UU,'   ||             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(       (~~~~~~~~~~~~]""'        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                t->putcen(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
                playSound(Mp3File("FindVeryImportantItem"));
                Sleep(12000);
                stopSound(SoundType::MP3);
                t->txtmacs.displayGame(getPGame());
                getPGame()->getOverworldMusic().play();
                allow5Cross = false;
                return Collision(true, false, true);
            }
            else if (allow5Cross)
            {
                getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                getPGame()->getVWin()->putcen(
                    ColorString("You cannot enter without the full power of the Harp", dngutil::MAGENTA),
                    getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE,
                    true
                );
                pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin());
                getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                getPGame()->getVWin()->txtmacs.displayOverworldInfo(getPGame());
            }
        }
        else if (!getPGame()->getPlayer()->hasHarpPiece(harpNumber))
        {
            getPGame()->getPlayer()->setDungeonStart();
            getPGame()->setDungeonLevel(getPGame()->getPlayer()->getLvl());
            return Collision(true, false, true);
        }
    }
    return Collision(false, true, false);
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
        stopSound(SoundType::MP3);
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

        playSound(Mp3File("AltarSong"));

        for (int i = 0; i < 800; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }

        stopSound(SoundType::MP3);

        vcursor.y = t.BOTTOM_DIVIDER_TEXT_LINE;
        getPGame()->getPlayer()->increaseHealth(getPGame()->getPlayer()->getMaxhp());
        saveGame(getPGame()->getPlayer(), getPGame());
        v->putcen(ColorString("Your game has been saved and your health is at its full natural stat.", dngutil::WHITE), vcursor.y++);
        pressEnter(vcursor, v);
        t.clearDivider("bottom");
        t.clearMapArea(false, NULL);
        t.displayGame(getPGame());

        getPGame()->getOverworldMusic().play();

    }
    return Collision(false, true);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Vender Object functions

VenderObject::VenderObject(Game* game, Coordinate coord, ColorString name, ColorChar mapRep, dngutil::TID itemTid, int price)
    :MapObject(
        game,
        mapRep,
        coord,
        "VENDER",
        true,
        false,
        false,
        dngutil::TID::Vender,
        dngutil::P_WALL,
        dngutil::BTID::None,
        true
    )
{
    itemName = name;
    this->itemTid = itemTid;
    this->price = price;
}

Collision VenderObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        VirtualWindow* t = getPGame()->getVWin();
        TextMacros& txt = t->txtmacs;

        txt.clearDivider("bottom");
        int topLine = txt.BOTTOM_DIVIDER_TEXT_LINE - 1;

        t->putcen(ColorString("That would be ", dngutil::WHITE) + itemName + ColorString(". It costs " + std::to_string(price) + " gold.", dngutil::WHITE), topLine, true);
        t->putcen(ColorString("Would you like to buy it? You have " + std::to_string(getPGame()->getPlayer()->getGold()) + " gold (Y = yes N = no)", dngutil::WHITE), topLine + 1, true);

        while (true)
        {
            if (keypress('Y'))
            {
                txt.clearDivider("bottom");
                if (getPGame()->getPlayer()->getGold() >= price)
                {
                    getPGame()->getPlayer()->changeGold(price * -1);
                    getPGame()->getPlayer()->addToInventory(getItemFromId(itemTid, getPGame()));
                    playSound(WavFile("FindItem", false, false));
                    t->putcen(ColorString("Thank you for your business", dngutil::WHITE), topLine, true);
                    pressEnter(Coordinate(0, topLine + 1), t);
                }
                else
                {
                    playSound(WavFile("PuzzleError", false, false));
                    t->putcen(ColorString("You don't have enough gold", dngutil::WHITE), topLine);
                    pressEnter(Coordinate(0, topLine + 1), t);
                }
                break;
            }
            else if (keypress('N'))
            {
                break;
            }
        }

        txt.clearDivider("bottom");
        txt.displayOverworldInfo(getPGame());
    }
    return Collision(false, true, false);
}

//---------------------------------------------------------------


//---------------------------------------------------------------
// Hero Blade Stone functions

HerosBladeStone::HerosBladeStone(Game* game, Coordinate coord)
    :MapObject(
        game,
        ColorChar(' ', getColor(dngutil::DARKGRAY, dngutil::DARKGRAY)),
        coord,
        "HEROSBLADESTONE",
        true,
        true,
        false,
        dngutil::TID::HerosBladeStone,
        dngutil::P_WALL,
        dngutil::BTID::None,
        true
    )
{

}

Collision HerosBladeStone::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer() && !getPGame()->getPlayer()->hasItem(dngutil::TID::HerosBlade))
    {
        stopSound(SoundType::MP3);
        VirtualWindow* v = getPGame()->getVWin();
        TextMacros& t = v->txtmacs;
        Coordinate vcursor(30, t.DIVIDER_LINES[1] + 10);

        t.clearMapArea(true, 10);
        t.clearDivider("bottom");

        int handleColor = dngutil::MAGENTA;
        int bladeColor = dngutil::YELLOW;
        int rockColor = dngutil::DARKGRAY;
        int flowerColor = dngutil::RED;
        int stemColor = dngutil::GREEN;

        v->put(ColorString(R"(        _)", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(       (_))", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(       |=|)", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(       |=|)", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(   /|__|_|__|\)", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(  (    ( )    ))", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(   \|\/\"/\/|/)", handleColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(     |  Y  |)", bladeColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(     |  |  |)", bladeColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(     |  |  |)", bladeColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(    _)", rockColor) + ColorString(R"(|  |  |)", bladeColor), vcursor); vcursor.y++;
        v->put(ColorString(R"( __/ )", rockColor) + ColorString(R"(|  |  |)", bladeColor) + ColorChar('\\', rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(/  \ )", rockColor) + ColorString(R"(|  |  |)", bladeColor) + ColorString(R"(  \)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(   __)", rockColor) + ColorString(R"(|  |  |)", bladeColor) + ColorString(R"(   |)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(/\/  )", rockColor) + ColorString(R"(|  |  |)", bladeColor) + ColorString(R"(   |\)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"( <   +)", rockColor) + ColorString(R"(\ |  |)", bladeColor) + ColorString(R"(\ />  \)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(  >   + )", rockColor) + ColorString(R"(\  |)", bladeColor) + ColorString(R"( LJ    |)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(        + )", rockColor) + ColorString(R"(\|)", bladeColor) + ColorString(R"(+  \  < \)", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(  (O))", flowerColor) + ColorString(R"(      +    |    ))", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(   |  )", stemColor) + ColorString(R"(           \  /\ )", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"( ( | ))", stemColor) + ColorString(R"(   (o)  )", flowerColor) + ColorString(R"(    \/  ))", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(_)", rockColor) + ColorString(R"(\\|//)", stemColor) + ColorString(R"(__)", rockColor) + ColorString(R"(( | ))", stemColor) + ColorString(R"(______)_/ )", rockColor), vcursor); vcursor.y++;
        v->put(ColorString(R"(        \\|//        )", stemColor), vcursor); vcursor.y++;

        playSound(Mp3File("FindVeryImportantItem"));
        Sleep(12000);
        stopSound(SoundType::MP3);

        getPGame()->getPlayer()->addToInventory(new HerosBlade(getPGame(), Coordinate(-1, -1)));

        int l = t.BOTTOM_DIVIDER_TEXT_LINE;
        v->putcen(ColorString("You have found the Hero's Blade!", dngutil::YELLOW), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        t.clearDivider("bottom");
        t.clearMapArea(false, NULL);
        t.displayGame(getPGame());

        getPGame()->getOverworldMusic().play();
    }
    return Collision(false, true, false);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// Hero Spirit functions

HeroSpirit::HeroSpirit(Game* game, Coordinate coord)
    :MapObject(
        game,
        ColorChar('A', dngutil::DARKGRAY),
        coord,
        "Hero's Spirit",
        true,
        false,
        false,
        dngutil::TID::HeroSpirit,
        dngutil::P_WALL,
        dngutil::BTID::None,
        false
    )
{

}

Collision HeroSpirit::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer() && !getPGame()->getPlayer()->hasItem(dngutil::TID::HerosTunic))
    {
        stopSound(SoundType::MP3);
        VirtualWindow* v = getPGame()->getVWin();
        TextMacros& t = v->txtmacs;
        Coordinate vcursor(10, t.DIVIDER_LINES[1] + 1);

        t.clearMapArea(true, 10);
        t.clearDivider("bottom");

        v->put(ColorString(R"(                         /\     /\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                        (_ \   (  \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                        (_ `\iRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                         \_  \VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                           \o_)lI<= (=\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                          / `   ) , _\ ))", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                          )    /   (_(/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                         /_ __( \ __ _|)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                            _|     |_)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                   ,edMMNBRB)\ \_  /(PRBNMMbn.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                  dMMNBRPAV/o(_____)o\VAPRBNMMb)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                 fMMWBRYIli\ o  o  o /rlIPRBWMMj)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                 MMBRRSZIlti+._.-._.+ilIZSRRBMMM)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                 /_o__o_/KZYI0lLRBNWMMMN\_o___o_\)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                /_____/flZJlDYTPRKBBNWMM \ ___( \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(          NNBR0PRRBNMMMMMMMlIPZSKRNWMMMV  \__._\ \)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(          NMBRR00PRBNNMMMMM_o___o___o__/   )_ __)o))", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(          NMMBRR00PRRBNNMMM___o___o___/    /___/ /)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(          NMMBBRR00PRRBNNMM_/\_/\_/\_(  __/___x)/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(          NMMBBRR00PRRBNNMM \/ \/ \/ __ )    /(`)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(        __NMMBBRR00PRRBNNMM_________|_ /   _'))", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(       /__        ________        ___)(<  (====.)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(       \_____________________________)_\ __\---')", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(            VMBBR00RBNMNV           |__|\/)", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(             lMBRR00RNMV  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(              VMB00BMNV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(               VMB0BMV   )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                b'ger  )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                 VMV     )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        v->put(ColorString(R"(                  V     )", dngutil::DARKGRAY), vcursor); vcursor.y++;

        int l = t.BOTTOM_DIVIDER_TEXT_LINE;
        v->putcen(ColorString(getPGame()->getPlayer()->getName() + ", you have done well to find me.", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("After I fell to Zorlock I came here to wait for the one who would avenge me.", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("My power is weakening - when I depart my spell on Zorlock will too.", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("To aid you on your quest I give you - The Hero's Tunic - the one I wore.", dngutil::WHITE), l, true);
        playSound(WavFile("FindItem", false, false));
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("Additionally - you should find the blade that I used to seal away Zorlock", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        v->putcen(ColorString("I returned it deep within the Korloma Forest - you will likely need", dngutil::WHITE), l, true);
        v->putcen(ColorString("a lot of gear to retrieve it from its resting place.", dngutil::WHITE), l + 1, true);
        pressEnter(Coordinate(0, l + 2), v);
        t.clearLine(l);
        t.clearLine(l + 1);
        t.clearLine(l + 2);

        getPGame()->getPlayer()->addToInventory(new HerosTunic(getPGame(), Coordinate(-1, -1)));

        v->putcen(ColorString(getPGame()->getPlayer()->getName() + ", please avenge me...", dngutil::WHITE), l, true);
        pressEnter(Coordinate(0, l + 1), v);
        t.clearLine(l);
        t.clearLine(l + 1);

        t.clearDivider("bottom");
        t.clearMapArea(false, NULL);
        t.displayGame(getPGame());

        getPGame()->getOverworldMusic().play();

    }
    return Collision(false, true);
}

//---------------------------------------------------------------


//---------------------------------------------------------------
// harp piece functions

HarpPiece::HarpPiece(Game* game, Coordinate coord, int pieceNumber)
    :MapObject(
        game,
        ColorChar('O', dngutil::YELLOW),
        coord,
        "HARPPIECE",
        true,
        false,
        false,
        dngutil::TID::HarpPiece,
        dngutil::P_ITEM,
        dngutil::BTID::None,
        false
    )
{
    this->pieceNumber = pieceNumber;
}

Collision HarpPiece::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        stopSound(SoundType::MP3);
        VirtualWindow* t = getPGame()->getVWin();
        TextMacros& txt = t->txtmacs;

        txt.clearMapArea(true, 10);
        txt.clearDivider("bottom");
        int background = dngutil::DARKGRAY;
        int cross = dngutil::WHITE;

        Coordinate vcursor(15, txt.DIVIDER_LINES[1] + 4);
        
        if (pieceNumber == 1)
        {
            t->putcen(ColorString(R"(         ____                   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(         SSSS____.              )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(         (WW);;;;;\             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(         `WW'____ |     ,_____  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU ||||\ \___/,---. ) )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        }
        else if (pieceNumber == 2)
        {
            t->putcen(ColorString(R"(          UU |||||\____/||| //  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU ||||||||||||" //   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU |||||||||||' //    )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU |||||||||"  //     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        }
        else if (pieceNumber == 3)
        {
            t->putcen(ColorString(R"(          UU ||||||||'  //      )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU |||||||"  //       )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU ||||||'  //        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU ||||"   //         )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU |||"   //          )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        }
        else if (pieceNumber == 4)
        {
            t->putcen(ColorString(R"(          UU ||'   //           )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(          UU |"   //            )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(         ,UU,'   ||             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(       (~~~~~~~~~~~~]""'        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
            t->putcen(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        }

        playSound(WavFile("GetHarpPiece", false, false));

        stopSound(SoundType::MP3);

        getPGame()->getPlayer()->setHarp(pieceNumber, true);

        vcursor.y = txt.BOTTOM_DIVIDER_TEXT_LINE;
        t->putcen(ColorString("You found a piece of the harp!", dngutil::WHITE), vcursor.y++);
        pressEnter(vcursor, t);

        t->clearScreen();
        playSound(WavFile("WarpToDungeonStart", false, false));

        getPGame()->getPlayer()->gotoDungeonStart(pieceNumber);
        txt.displayGame(getPGame());

        getPGame()->getOverworldMusic().play();
        return Collision(true, true, true);

    }
    return Collision(false, true);
}

//---------------------------------------------------------------

//---------------------------------------------------------------
// house door functions

HouseDoorObject::HouseDoorObject(Game* game, Coordinate coord, Coordinate mapc, Coordinate roomc, int floor)
    :MapObject(
        game,
        ColorChar(' ', getColor(dngutil::YELLOW, dngutil::YELLOW)),
        coord,
        "HOUSEDOOR",
        true,
        true,
        false,
        dngutil::TID::HarpPiece,
        dngutil::P_WALL,
        dngutil::BTID::None,
        false
    )
{
    newRoomCoord = roomc;
    newMapCoord = mapc;
    this->floor = floor;
}

Collision HouseDoorObject::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        VirtualWindow* t = getPGame()->getVWin();
        TextMacros& txt = t->txtmacs;

        getPGame()->getActiveRoom()->setAll();
        getPGame()->getActiveRoom()->drawRoom();
        getPGame()->getActiveRoom()->getObjects(getPGame()->getPlayer()->getCoord()).remove(getPGame()->getPlayer());

        getPGame()->setActiveRoom(newMapCoord, floor);
        getPGame()->getPlayer()->setPosition(newRoomCoord);
        playSound(WavFile("UnlockDoor", false, false));
        return Collision(true, true, true);
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
        stopSound(SoundType::MP3);
        playSound(WavFile("EnterBattle", false, true));
        getPGame()->getVWin()->txtmacs.clearMapArea(true, 35);
        if (segboss->segmentedBattle(getPGame()->getPlayer()))
        {
            getPGame()->getVWin()->txtmacs.displayOverworldInfo(getPGame());
            it++;
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
            removeFromMap(true);
            return Collision(false, false);
        }
        return Collision(true, true, true);
    }
    return Collision(false, true);
}
//---------------------------------------------------------------