#include "spell.h"
#include "player.h"
#include "game.h"
#include "room.h"
#include "utilities.h"
#include <string>
#include <vector>
Spell::Spell(std::string n, std::string d, int m, dngutil::SPELLTID i)
{
    name = n;
    manaReq = m;
    description = d;
    id = i;
}

dngutil::SPELLTID Spell::getSpellId() const
{
    return id;
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
    :Spell("Dragon's Blessing", "Heals 50 HP", 30, dngutil::SPELLTID::DragonBlessing)
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
    :Spell("Seal Revealer", "Reveals any puzzles in the room", 0, dngutil::SPELLTID::SealRevealer)
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

SpiritRollerSpell::SpiritRollerSpell()
    :Spell("Spirit Roller", "Chance to increase a stat", 50, dngutil::SPELLTID::SpiritRoller)
{
}

std::string SpiritRollerSpell::castSpell(Player* player, Game* game)
{
    int ranNum = random(0, 6);

    if (ranNum != 2 && ranNum != 3)// did not get the two good stat ups, so lets try again if your luck is good
    {
        if (random(player->getLck(), dngutil::MAX_LCK + 3) == dngutil::MAX_LCK + 3)
        {
            ranNum = random(2, 3);
        }
    }

    int ranAmount = random(1, 3);
    std::string output;

    switch (ranNum)
    {
    case 0: // max hp
        player->increaseMaxhp(ranAmount);
        output = "The book lights up... and your Max HP rises by " + std::to_string(ranAmount);
        break;
    case 1: // max mana
        player->increaseMaxMana(ranAmount);
        output = "The book lights up... and your Max Mana rises by " + std::to_string(ranAmount);
        break;
    case 2: // attack
        player->increaseAtt(ranAmount);
        output = "The book lights up... and your Attack rises by " + std::to_string(ranAmount);
        break;
    case 3: // defense
        player->increaseDef(ranAmount);
        output = "The book lights up... and your Defense rises by " + std::to_string(ranAmount);
        break;
    case 4: // speed
        player->increaseSpd(ranAmount);
        output = "The book lights up... and your Speed rises by " + std::to_string(ranAmount);
        break;
    case 5: // luck
        player->increaseLck(ranAmount);
        output = "The book lights up... and your Luck rises by " + std::to_string(ranAmount);
        break;
    case 6: // bad, see what'll happen
        int newRanNum = random(1, 4);
        if (newRanNum == 3)
        {
            output = "The book lights up... and the spirits violently attack!";
            player->setHp(1);
        }
        else
        {
            output = "The book lights up... but nothing happens.";
        }
        break;
    }
    return output;
}

void SpiritRollerSpell::playCastSound()
{
    playSound(WavFile("Spellbook", false, false));
}