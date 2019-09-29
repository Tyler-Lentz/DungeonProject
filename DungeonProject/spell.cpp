#include "spell.h"
#include "player.h"
#include "game.h"
#include "room.h"
#include <string>
#include <vector>
Spell::Spell(std::string n, std::string d, int m)
{
    name = n;
    manaReq = m;
    description = d;
}

std::string Spell::getSpellDescription() const
{
    return description;
}

std::string Spell::getSpellName() const
{
    return name;
}

int Spell::getManaReq() const
{
    return manaReq;
}

DragonBlessingSpell::DragonBlessingSpell()
    :Spell("Dragon's Blessing", "Heals 50 HP", 30)
{
}

std::string DragonBlessingSpell::castSpell(Player* player, Game* game)
{
    int prevHp = player->getHp();
    player->increaseHealth(50);
    int newHp = player->getHp();
    std::string output = "Health increased from " + std::to_string(prevHp) + " to " + std::to_string(newHp);
    return output;
}

void DragonBlessingSpell::playCastSound()
{
    playSound(WavFile("Spellbook", false, false));
}

SealRevealerSpell::SealRevealerSpell()
    :Spell("Seal Revealer", "Reveals any puzzles in the room", 0)
{
}

std::string SealRevealerSpell::castSpell(Player* player, Game* game)
{
    std::string output = "";
    if (game->getActiveRoom()->hasPuzzle())
    {
        playSound(WavFile("Puzzle", false, true));
        std::string hint = game->getActiveRoom()->getPuzzleHint();
        output = ("There is a seal in this area. " + hint);
    }
    else
    {
        output = "There is not a seal in this area.";
    }
    return output;
}

void SealRevealerSpell::playCastSound()
{
    playSound(WavFile("Spellbook", false, false));
}