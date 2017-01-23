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
            "A rusty old sword.",
            "Attack1.wav"
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
            "A very simple shield."
        ),
        dngutil::P_PLAYER
    )
{
    this->exp = 0;
    this->expToLevel = getExpToLevel(getLvl());
}

Player::Player(const Player& other, Game* game)
    :Creature(other, game),
    inventory(other.inventory, game)
{
    this->exp = other.exp;
    this->expToLevel = other.expToLevel;
}

void Player::addToInventory(Item* item)
{
    inventory.push_back(item);
}

void Player::removeFromInventory(unsigned int index)
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
        stopMp3();

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        soundEffect("EnterBattle.wav", false, true);
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

    getPGame()->getVWin()->putcen(ColorString(
        "       " + getPrimary().getName() +
        " and " + getSecondary().getName() +
        "       ", dngutil::WHITE), vcursor.y);
}

ColorString Player::getExperienceBar()
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / static_cast<double>(expToLevel));

    int numOfCircles = static_cast<int>(scaleFactor * exp);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    return ColorString(((std::string((unsigned int)numOfCircles, '=')) + temp), dngutil::MAGENTA);
}

void Player::addExperience(unsigned int experience)
{
    unsigned int overFlowXp = 0;

    VirtualWindow* vwin = getPGame()->getVWin();
    vwin->txtmacs.clearDivider("bottom");

    soundEffect("Experience.wav", true, true); 
    vwin->txtmacs.clearMapArea(false, NULL);
    for (unsigned int i = 0; i < experience; i++)
    {
        Coordinate vcursor(0, static_cast<int>(dngutil::CONSOLEY / 2.0));
        exp++;     
        vwin->putcen(getExperienceBar(), vcursor.y);
        if (exp >= expToLevel)
        {
            overFlowXp = (experience - i);
            Sleep(50);
            soundEffect("FindItem.wav", false, false);
            break;
        }

        if (!keypress(VK_RETURN))
        {
            Sleep(50);
        }
    }
    vwin->txtmacs.clearMapArea(false, NULL);
    stopSound();

    if (exp >= expToLevel)
    {
        exp -= expToLevel;
        expToLevel = getExpToLevel(getLvl());
        increaseLvl(1);

        Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 4);
        vwin->putcen(ColorString("Level Up! You are now level " + std::to_string(getLvl()), dngutil::WHITE), vcursor.y);
        soundEffect("LevelUp.wav", false, false);

        vcursor.y++;

        int prevAtt = getAtt();
        int prevHp = getHp();
        int prevDef = getDef();
        int prevLck = getLck();
        int prevSpd = getSpd();

        levelUpStats();

        vcursor.y += 2; vcursor.x = 0;

        vwin->putcen(ColorString("Health + " + std::to_string(getHp() - prevHp), dngutil::RED), vcursor.y++);
        soundEffect("PickupItem.wav", false, false);
        vwin->putcen(ColorString("Attack + " + std::to_string(getAtt() - prevAtt), dngutil::GREEN), vcursor.y++);
        soundEffect("PickupItem.wav", false, false);
        vwin->putcen(ColorString("Defense + " + std::to_string(getDef() - prevDef), dngutil::BLUE), vcursor.y++);
        soundEffect("PickupItem.wav", false, false);
        vwin->putcen(ColorString("Luck + " + std::to_string(getLck() - prevLck), dngutil::YELLOW), vcursor.y++);
        soundEffect("PickupItem.wav", false, false);
        vwin->putcen(ColorString("Speed + " + std::to_string(getSpd() - prevSpd), dngutil::CYAN), vcursor.y);
        soundEffect("PickupItem.wav", false, false);

        vcursor.y = vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;
        pressEnter(vcursor, vwin);
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
                unsigned int itemPosition = (abs(positions[0] - vcursor.y));
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