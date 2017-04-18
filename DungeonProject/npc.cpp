#include "npc.h"
#include "colorstring.h"
#include "game.h"
#include "coordinate.h"
#include "player.h"
#include "virtualwindow.h"
#include "room.h"

#include <string>
#include <vector>
#include <Windows.h>

Npc::Npc(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    std::vector<ColorString> dialogue
) :
    Creature(
        pgame, mapRep, coord, name,
        true, false, false,
        dngutil::TID::Npc,
        0, 0, 0, 0, 0, 0,
        nullptr, nullptr,
        dngutil::P_PLAYER,
        dngutil::ClassType::ADVENTURER,
        true
    )
{
    this->dialogue = dialogue;
}

Npc::Npc(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    ColorString dialogue
) :
    Creature(
        pgame, mapRep, coord, name,
        true, false, false,
        dngutil::TID::Npc,
        0, 0, 0, 0, 0, 0,
        nullptr, nullptr,
        dngutil::P_PLAYER,
        dngutil::ClassType::ADVENTURER,
        true
    )
{
    this->dialogue.push_back(dialogue);
}

bool Npc::movement()
{
    if (!((getLastMoveTime() + 3500) > GetTickCount()))
    {
        switch (random(dngutil::MOVEMENT_RANDOM_CHANCE))
        {
        case 0:
            return adjustPosition(dngutil::Movement::UP);
            break;
        case 1:
            return adjustPosition(dngutil::Movement::DOWN);
            break;
        case 2:
            return adjustPosition(dngutil::Movement::LEFT);
            break;
        case 3:
            return adjustPosition(dngutil::Movement::RIGHT);
            break;
        }
    }
    return false;
}

Collision Npc::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider == getPGame()->getPlayer())
    {
        auto startTime = GetTickCount();

        VirtualWindow* t = getPGame()->getVWin();
        TextMacros& txt = t->txtmacs;

        txt.clearDivider("bottom");
        t->putcen(getRandomDialogue(), txt.BOTTOM_DIVIDER_TEXT_LINE, true);
        pressEnter(Coordinate(0, txt.BOTTOM_DIVIDER_TEXT_LINE + 1), t);
        txt.clearDivider("bottom");
        txt.displayOverworldInfo(getPGame());

        increaseLastMoveTime(GetTickCount() - startTime);
    }
    return Collision(false, true, false);
}

ColorString Npc::getRandomDialogue()
{
    if (!dialogue.empty())
    {
        return dialogue[random(0, dialogue.size() - 1)];
    }
    else
    {
        return ColorString("", dngutil::WHITE);
    }
}