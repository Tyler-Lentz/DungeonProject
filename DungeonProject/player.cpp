#include "player.h"
#include "creature.h"
#include "mapobject.h"
#include "coordinate.h"
#include "colorstring.h"
#include "item.h"
#include "utilities.h"
#include "game.h"
#include "room.h"
#include "virtualwindow.h"

#include <Windows.h>

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
            false,
            "A sword that has been passed down for generations in your family."
        ),
        new Secondary(
            pgame,
            ColorChar('(', dngutil::DARKGRAY),
            Coordinate(-1, -1),
            "Shield",
            false,
            dngutil::TID::Secondary,
            50,
            .95,
            "A shield that has been passed down for generations in your family."
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
    int adjustedSpeed = static_cast<int>(dngutil::MAX_SPD * 1.5);
    bool canmove = !((getLastMoveTime() + ((adjustedSpeed - getSpd())) > GetTickCount()));

    if (canmove)
    {
        if (keypress(VK_LEFT))
        {
            return adjustPosition(dngutil::Movement::LEFT);
        }
        else if (keypress(VK_RIGHT))
        {
            return adjustPosition(dngutil::Movement::RIGHT);
        }
        else if (keypress(VK_UP))
        {
            return adjustPosition(dngutil::Movement::UP);
        }
        else if (keypress(VK_DOWN))
        {
            return adjustPosition(dngutil::Movement::DOWN);
        }
        else if (keypress('I'))
        {
            inventoryMenu();
            getPGame()->getVWin()->txtmacs.displayGame(getPGame());
        }
    }
    return false;
}


MapObject* Player::makeSave(Game* game)
{
    return new Player(*this, game);
}

Collision Player::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider->isAggressive())
    {
        // TODO: game_pointer->getMusicPlayer()->stopMp3();

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        // TODO: game_pointer->getMusicPlayer()->soundEffect("EnterBattle.wav", false, true);
        if (battle(collider))
        {
            getPGame()->getVWin()->txtmacs.displayOverworldInfo(getPGame());
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(collider);
            collider->removeFromMap(true);
            return Collision(true, true, true);
        }
        else
        {
            it++;
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
            removeFromMap(false);
            return Collision(false, false);
        }
    }
    return Collision(false, true);
}

void Player::printStats(int startingXCoord, int startingCursorY)
{
    Coordinate vcursor(startingXCoord, startingCursorY);

    getPGame()->getVWin()->putcen(ColorString(
        " Level: " + std::to_string(getLvl()) +
        " Health: " + std::to_string(getHp()) + "/" + std::to_string(getMaxhp()) +
        " Attack: " + std::to_string(getAtt()) +
        " Defense: " + std::to_string(getDef()) +
        " Luck: " + std::to_string(getLck()) +
        " Speed: " + std::to_string(getSpd()), dngutil::WHITE), vcursor.y);
    vcursor.y++; vcursor.x = 0;

    getPGame()->getVWin()->txtmacs.clearLine(vcursor.y);

    getPGame()->getVWin()->putcen(ColorString(
        " " + getPrimary().getName() +
        " and " + getSecondary().getName() +
        " ", dngutil::WHITE), vcursor.y);
}

ColorString Player::getExperienceBar()
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / static_cast<double>(expToLevel));

    int numOfCircles = static_cast<int>(scaleFactor * exp);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    return ColorString(((std::string((size_t)numOfCircles, '=')) + temp), dngutil::MAGENTA);
}

void Player::addExperience(size_t experience)
{
    size_t overFlowXp = 0;

    VirtualWindow* vwin = getPGame()->getVWin();
    vwin->txtmacs.clearDivider("bottom");

    // TODO: game_pointer->getMusicPlayer()->soundEffect("Experience.wav", true, true);
    for (unsigned int i = 0; i < experience; i++)
    {
        vwin->txtmacs.clearMapArea(false, NULL);
        Coordinate vcursor(0, static_cast<int>(dngutil::CONSOLEY / 2.0));
        exp++;
        if (exp >= expToLevel)
        {
            overFlowXp = (experience - i);
            // TODO: game_pointer->getMusicPlayer()->soundEffect("Levelup.wav", false, false);
            break;
        }

        vwin->putcen(getExperienceBar(), vcursor.y);
        if (!keypress(VK_RETURN))
        {
            Sleep(20);
        }
    }
    // TODO: game_pointer->getMusicPlayer()->stopSound();

    if (exp >= expToLevel)
    {
        exp -= expToLevel;
        expToLevel = static_cast<int>((0.5 * (getLvl() * getLvl())) + 50);
        increaseLvl(1);

        Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 4);
        vwin->putcen(ColorString("Level Up! You are now level " + std::to_string(getLvl()), dngutil::WHITE), vcursor.y);
        
        vcursor.y++;

        vwin->txtmacs.displayLevelupStats(vcursor, this);
        Sleep(1000);

        for (int i = 0; i < 5; i++)
        {
            switch (i)
            {
            case 0:
            {
                int healthIncrease = random(4, 8);
                increaseMaxhp(healthIncrease);
                increaseHealth(healthIncrease);
            }
            if (getMaxhp() > dngutil::MAX_HP)
            {
                setMaxhp(dngutil::MAX_HP);
            }
            if (getHp() > dngutil::MAX_HP)
            {
                setHp(dngutil::MAX_HP);
            }
            break;

            case 1:
                increaseAtt(random(3, 4));
                if (getAtt() > dngutil::MAX_ATT)
                {
                    setAtt(dngutil::MAX_ATT);
                }
                break;

            case 2:
                increaseDef(random(2, 4));
                if (getDef() > dngutil::MAX_DEF)
                {
                    setDef(dngutil::MAX_DEF);
                }
                break;

            case 3:
                increaseLck(random(2, 6));
                if (getLck() > dngutil::MAX_LCK)
                {
                    setLck(dngutil::MAX_LCK);
                }
                break;

            case 4:
                increaseSpd(random(4, 8));
                if (getSpd() > dngutil::MAX_SPD)
                {
                    setSpd(dngutil::MAX_SPD);
                }
                break;
            }
            vwin->txtmacs.displayLevelupStats(vcursor, this);
            // TODO: game_pointer->getMusicPlayer()->soundEffect("PickupItem.wav", false, false);
        }
    }
    if (overFlowXp > 0)
    {
        addExperience(overFlowXp);
    }
}

void Player::inventoryMenu() // How not to program in three easy steps. 1: Dont do this.
{
    bool exitFunction = false;

    int positions[2];

    getPGame()->getVWin()->txtmacs.displayInventory(positions, this);

    if (false)
    {
    beginning:
        getPGame()->getVWin()->txtmacs.displayInventory(positions, this);
    }

    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
    Coordinate vcursor(0, positions[0]);

    while (!exitFunction)
    {
        getPGame()->getVWin()->put(ColorString("->", dngutil::CYAN), vcursor);

        while (!exitFunction)
        {
            if (keypress(VK_UP) && vcursor.y > positions[0])
            {
                Sleep(dngutil::MENU_DELAY);
                vcursor.x = 0;
                getPGame()->getVWin()->put(ColorString("   ", dngutil::LIGHTGRAY), vcursor);

                vcursor.y--;
                break;
            }
            else if (keypress(VK_DOWN) && vcursor.y < positions[1])
            {
                Sleep(dngutil::MENU_DELAY);
                vcursor.x = 0;
                getPGame()->getVWin()->put(ColorString("   ", dngutil::LIGHTGRAY), vcursor);

                vcursor.y++;
                break;
            }
            else if (keypress(VK_RETURN) && !inventory.empty())
            {
                Sleep(dngutil::MENU_DELAY);
                size_t itemPosition = (abs(positions[0] - vcursor.y));
                Item* itemModifying = inventory[itemPosition];
                itemModifying->action(this, itemPosition);

                if (itemModifying->isConsumable())
                {
                    delete itemModifying;
                    std::vector<Item*>::iterator it = inventory.begin();
                    it += itemPosition;
                    inventory.erase(it);
                }
                vcursor.y++;
                pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin());
                goto beginning;
            }
            else if (keypress(VK_ESCAPE))
            {
                exitFunction = true;
            }
        }
    }
    getPGame()->getVWin()->txtmacs.displayGame(getPGame());
}

const Inventory& Player::getInventory() const
{
    return inventory;
}

bool Player::swapPrimary(Item*& itemToSwap)
{
    Item* tempItem = itemToSwap;
    Primary* backupPrimary = getPrimaryMemory();

    itemToSwap = getPrimaryMemory();
    getPrimaryMemory() = dynamic_cast<Primary*>(tempItem);
    if (getPrimaryMemory() == nullptr)
    {
        getPrimaryMemory() = backupPrimary;
        itemToSwap = tempItem;
        return false;
    }
    return true;
}

bool Player::swapSecondary(Item*& itemToSwap)
{
    Item* tempItem = itemToSwap;
    Secondary* backupSecondary = getSecondaryMemory();

    itemToSwap = getSecondaryMemory();
    getSecondaryMemory() = dynamic_cast<Secondary*>(tempItem);
    if (getSecondaryMemory() == nullptr)
    {
        getSecondaryMemory() = backupSecondary;
        itemToSwap = tempItem;
        return false;
    }
    return true;
}

Inventory& Player::getInventoryNotConst()
{
    return inventory;
}
//------------------------------------------------------------