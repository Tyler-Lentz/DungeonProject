#ifndef NPC_H
#define NPC_H

#include "creature.h"
#include "game.h"
#include "colorstring.h"
#include "utilities.h"

#include <string>
#include <vector>

class Npc : public Creature
{
public:
    Npc(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        std::vector<ColorString> dialogue
    );

    Npc(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        ColorString dialogue
    );

    bool movement() override;
    void printStats(int LONGEST_LINE_LENGTH, int startingCursorY) override {}
    Collision mapAction(MapObject* collider, std::list<MapObject*>::iterator& it) override;

    void levelUpStats() override {}
private:
    std::vector<ColorString> dialogue;
};

#endif