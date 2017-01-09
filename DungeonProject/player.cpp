#include "player.h"
#include "creature.h"
#include "mapobject.h"
#include "coordinate.h"
#include "colorstring.h"
#include "item.h"
#include "utilities.h"

//------------------------------------------------------------
// Player Functions

Player::Player(
    Game* pgame,
    Coordinate coord
)
    :Creature(
        pgame,
        ColorChar('A', dngutil::GREEN),
        coord,
        "Player",
        true,
        false,
        false,
        dngutil::TID::Player,
        50, // health
        10, // attack
        5, // defense
        1, // luck
        20, // speed
        1, // level
        new Primary(
            pgame,
            ColorChar('i', dngutil::WHITE),
            Coordinate(-1, -1),
            "Sword",
            false,
            dngutil::TID::Primary,
            1,
            3,
            80,
            false
        ),
        new Secondary(
            pgame,
            ColorChar('(', dngutil::DARKGRAY),
            Coordinate(-1, -1),
            "Shield",
            false,
            dngutil::TID::Secondary,
            50,
            .95
        ),
        dngutil::P_PLAYER
    )
{
    this->exp = 0;
    // inventory is initialized as empty
}

Player::Player(const Player& other, Game* game)
    :Creature(other, game),
    inventory(other.inventory, game)
{
    this->exp = other.exp;
}

void Player::addToInventory(Item* item)
{
    inventory.push_back(item);
}

void Player::removeFromInventory(size_t index)
{
    auto it = inventory.begin();
    it += index;
    inventory.erase(it);
}

bool Player::movement()
{
    // TODO: based on keypresses, move the player
}


MapObject* Player::makeSave(Game* game)
{
    return new Player(*this, game);
}

Collision Player::mapAction(MapObject* collider)
{
    // TODO: implement this
}

void Player::printStats(int LONGEST_LINE_LENGTH, int startingCursorY) const
{
    // TODO: this
}

ColorString Player::getExperienceBar()
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / static_cast<double>(expToLevel));

    int numOfCircles = static_cast<int>(scaleFactor * exp);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    return ColorString(((std::string((size_t)numOfCircles, '=')) + temp), dngutil::MAGENTA);
}
//------------------------------------------------------------