#ifndef SPELL_H
#define SPELL_H

#include "utilities.h"

#include <string>

class Player;
class Game;

class Spell
{
public:
    Spell(std::string name, std::string description, int manaReq, dngutil::SPELLTID i);

    std::string getSpellName() const;
    std::string getSpellDescription() const;
    int getManaReq() const;
    dngutil::SPELLTID getSpellId() const;

    virtual std::string castSpell(Player* player, Game* game) = 0;
    virtual void playCastSound() = 0;
private:
    std::string name;
    std::string description;
    int manaReq;
    dngutil::SPELLTID id;
};

class SealRevealerSpell : public Spell
{
public:
    SealRevealerSpell();

    std::string castSpell(Player* player, Game* game) override;
    void playCastSound() override;
};

class DragonBlessingSpell : public Spell
{
public:
    DragonBlessingSpell();

    std::string castSpell(Player* player, Game* game) override;
    void playCastSound() override;
};

class MeditationSpell : public Spell
{
public:
    MeditationSpell();

    std::string castSpell(Player* player, Game* game) override;
    void playCastSound() override;
};

class DragonShieldSpell : public Spell
{
public:
    DragonShieldSpell();

    std::string castSpell(Player* player, Game* game) override;
    void playCastSound() override;
};

class SpiritRollerSpell : public Spell
{
public:
    SpiritRollerSpell();

    std::string castSpell(Player* player, Game* game) override;
    void playCastSound() override;
};
#endif