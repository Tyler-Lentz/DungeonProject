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
#include "savefile.h"

//------------------------------------------------------------
// Player Functions

Player::Player(
    Game* pgame,
    Coordinate coord,
    std::string name
)
    :Creature(
        pgame,
        ColorChar('A', dngutil::noArmorPlayerColor),
        coord,
        name,
        true,
        false,
        false,
        dngutil::TID::Player,
        50, // health
        10, // attack
        5, // defense
        1, // luck
        10, // speed
        1, // level
        new Primary(
            pgame,
            ColorChar('i', dngutil::WHITE),
            Coordinate(-1, -1),
            "Sword",
            false,
            1,
            3,
            80,
            false,
            "A rusty old sword",
            WavFile("Attack1", false, false),
            dngutil::ClassType::ADVENTURER
        ),
        new Secondary(
            pgame,
            ColorChar('(', dngutil::DARKGRAY),
            Coordinate(-1, -1),
            "Shield",
            false,
            dngutil::TID::Secondary,
            100,
            1,
            "A very bland shield."
        ),
        dngutil::P_PLAYER,
        dngutil::ClassType::ADVENTURER
    ), startingMaxSpeedMultiplier(1.5)
{
    harp1 = false;
    harp2 = false;
    harp3 = false;
    harp4 = false;

    maxSpeedMultiplier = startingMaxSpeedMultiplier;

    startingDungeonMapCoord = Coordinate(0, 0);
    startingDungeonRoomCoord = Coordinate(0, 0);

    this->exp = 0;
    this->expToLevel = getExpToLevel(getLvl());

    inventory.push_back(new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL));
    inventory.push_back(new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL));

    armor = new BlueTunic(getPGame(), Coordinate(-1, -1));
    boots = new StandardBoots(getPGame(), Coordinate(-1, -1));

    hpEv = 0;
    attEv = 0;
    defEv = 0;
    lckEv = 0;
    spdEv = 0;

    gold = 0;

    steps = pgame->getDifficulty().beastSteps;
}

void Player::setArmor(Equipment* equip)
{
    armor = equip;
}

void Player::setBoots(Equipment* equip)
{
    boots = equip;
}

void Player::gotoDungeonStart(int harpPiece)
{
    int floorToGoTo = 2;
    if (harpPiece == 4)
    {
        floorToGoTo = dngutil::HOUSE_FLOOR;
    }
    getPGame()->setActiveRoom(startingDungeonMapCoord, floorToGoTo);
    setPosition(startingDungeonRoomCoord);
    startingDungeonMapCoord = Coordinate(0, 0);
    startingDungeonRoomCoord = Coordinate(0, 0);
    getPGame()->setOverworldMusic(getPGame()->getActiveRoom()->getMusic());
}

int Player::getGold() const
{
    return gold;
}

void Player::changeGold(int amount)
{
    gold += amount;
}

void Player::setGold(int amount)
{
    gold = amount;
}

void Player::setDungeonStart(Coordinate room, Coordinate map)
{
    startingDungeonRoomCoord = room;
    startingDungeonMapCoord = map;
}

void Player::setDungeonStart()
{
    startingDungeonRoomCoord = this->getCoord();
    startingDungeonMapCoord = getPGame()->getActiveRoom()->getRoomInfo().mapCoord;
}

Coordinate Player::getStartingDungeonMapCoord() const
{
    return startingDungeonMapCoord;
}
Coordinate Player::getStartingDungeonRoomCoord() const
{
    return startingDungeonRoomCoord;
}

bool Player::hasItem(dngutil::TID tid, bool onlyCountHolding)
{
    if (!onlyCountHolding)
    {
        for (auto& i : inventory)
        {
            if (i->getTypeId() == tid)
            {
                return true;
            }
        }
    }

    if (getPrimary().getTypeId() == tid || getSecondary().getTypeId() == tid)
    {
        return true;
    }
    if (getArmorMemory()->getTypeId() == tid || getBootsMemory()->getTypeId() == tid)
    {
        return true;
    }
    return false;
}

void Player::setHarp(int number, bool value)
{
    switch (number)
    {
    case 1:
        harp1 = value;
        break;
    case 2:
        harp2 = value;
        break;
    case 3:
        harp3 = value;
        break;
    case 4:
        harp4 = value;
        break;
    default: errorMessage("Invalid number passed to setHarp()", __LINE__, __FILE__);
    }
}

bool Player::hasFullHarp() const
{
    return (harp1 && harp2 && harp3 && harp4);
}

bool Player::hasHarpPiece(int number) const
{
    switch (number)
    {
    case 1: return harp1;
    case 2: return harp2;
    case 3: return harp3;
    case 4: return harp4;
    }
    errorMessage("Invalid number passed to getHarpPiece()", __LINE__, __FILE__);

    // so i dont get a warning or if somehow it gets to here
    return false;
}

void Player::setExp(int value)
{
    exp = value;
}

void Player::setExpToLevel(int value)
{
    expToLevel = value;
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

void Player::upgradePrimary(dngutil::TID itemToUpgrade)
{
    if (!hasItem(itemToUpgrade, true))
    {
        return;
    }

    switch (itemToUpgrade)
    {
    case dngutil::TID::HerosBlade:
        delete getPrimaryMemory();
        setPrimary(new HerosBlade2(getPGame(), Coordinate(-1, -1)));
        break;
    }

    playSound(WavFile("FindItem", false, false));
}

void Player::setMaxSpeedMultiplier(double value)
{
    maxSpeedMultiplier = value;
}

bool Player::movement()
{
    int adjustedSpeed = static_cast<int>(dngutil::MAX_SPD * maxSpeedMultiplier);
    // make speed boots adjust the multiplier to max_spd
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
        else if (keypress('U'))
        {
            statsMenu();
        }
        else if (keypress('M'))
        {
            mapMenu();
        }
    }
    return false;
}

void Player::minusStep()
{
    steps--;
}

Collision Player::mapAction(MapObject* collider, std::list<MapObject*>::iterator& it)
{
    if (collider->isAggressive())
    {
        stopSound(SoundType::MP3);

        getPGame()->getActiveRoom()->drawRoom();
        Sleep(100);

        playSound(WavFile("EnterBattle", false, true));
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

void Player::resetSteps()
{
    steps = getPGame()->getDifficulty().beastSteps;
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
        getPrimary().getName() +
        " - " + getSecondary().getName() +
        " - " + getArmorMemory()->getName() +
        " - " + getBootsMemory()->getName()
        , dngutil::WHITE), vcursor.y);
}

ColorString Player::getExperienceBar()
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / static_cast<double>(expToLevel));

    int numOfCircles = static_cast<int>(scaleFactor * exp);

    std::string temp;
    int color = dngutil::MAGENTA;
    if ((MAXIMUM_CHARACTERS - numOfCircles) >= 0)
    {
        temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    }
    else
    {
        color = dngutil::LIGHTGREEN;
    }

    if (numOfCircles > MAXIMUM_CHARACTERS)
    {
        numOfCircles = MAXIMUM_CHARACTERS;
    }

    return ColorString(((std::string((unsigned int)numOfCircles, '=')) + temp), color);
}

void Player::addExperience(unsigned int experience, dngutil::EvType ev)
{
    switch (ev)
    {
    case dngutil::EvType::HEALTH:
        hpEv += 0.5;
        break;
    case dngutil::EvType::ATTACK:
        attEv += 0.25;
        break;
    case dngutil::EvType::DEFENSE:
        defEv += 0.25;
        break;
    case dngutil::EvType::SPEED:
        spdEv += 1;
        break;
    case dngutil::EvType::LUCK:
        lckEv += 2;
        break;
    }

    unsigned int overFlowXp = 0;

    VirtualWindow* vwin = getPGame()->getVWin();
    vwin->txtmacs.clearDivider("bottom");

    playSound(WavFile("Experience", true, true)); 
    vwin->txtmacs.clearMapArea(false, NULL);
    int xpBarLine = static_cast<int>(dngutil::CONSOLEY / 2.0);
    vwin->putcen(ColorString("******************************************", dngutil::BLUE), xpBarLine - 2);
    vwin->putcen(ColorString("******************************************", dngutil::BLUE), xpBarLine + 2);

    for (unsigned int i = 0; i < experience; i++)
    {
        Coordinate vcursor(0, xpBarLine);
        exp++;     
        vwin->putcen(getExperienceBar(), vcursor.y);
        if (exp >= expToLevel)
        {
            overFlowXp = (experience - i);
            Sleep(50); vcursor.y++;
            playSound(WavFile("FindItem", false, false));
            break;
        }

        if (!keypress(VK_RETURN))
        {
            Sleep(50); vcursor.y++;
        }
    }
    stopSound(SoundType::WAV);
    Sleep(450);
    vwin->txtmacs.clearMapArea(false, NULL);

    if (exp >= expToLevel)
    {
        exp -= expToLevel;
        expToLevel = getExpToLevel(getLvl());
        increaseLvl(1);

        Coordinate vcursor(0, vwin->txtmacs.DIVIDER_LINES[1] + 4);
        vwin->putcen(ColorString("Level Up! You are now level " + std::to_string(getLvl()), dngutil::WHITE), vcursor.y);

        vcursor.y++;

        int prevAtt = getAtt();
        int prevMaxhp = getMaxhp();
        int prevDef = getDef();
        int prevLck = getLck();
        int prevSpd = getSpd();

        levelUpStats();
        getPGame()->adjustScore(dngutil::BASE_SCORE_INCREASE_LEVEL);

        int hpChange = getMaxhp() - prevMaxhp;
        int attChange = getAtt() - prevAtt;
        int defChange = getDef() - prevDef;
        int lckChange = getLck() - prevLck;
        int spdChange = getSpd() - prevSpd;

        vcursor.y += 2; vcursor.x = 0;

        statIncreaseDisplay(hpChange, prevMaxhp, dngutil::RED, "Max Health: ", vwin, vcursor);
        statIncreaseDisplay(attChange, prevAtt, dngutil::GREEN, "Attack: ", vwin, vcursor);
        statIncreaseDisplay(defChange, prevDef, dngutil::BLUE, "Defense: ", vwin, vcursor);
        statIncreaseDisplay(lckChange, prevLck, dngutil::YELLOW, "Luck: ", vwin, vcursor);
        statIncreaseDisplay(spdChange, prevSpd, dngutil::CYAN, "Speed: ", vwin, vcursor);
        
        vcursor.y += 3;

        playSound(WavFile("RefillHealth", true, true));
        while (getHp() < static_cast<int>(getMaxhp()))
        {
            increaseHealth(1);
            vwin->putcen(getHealthBar(), vcursor.y);
            if (!keypress(VK_RETURN))
            {
                Sleep(50);
            }
        }
        stopSound(SoundType::WAV);

        while (keypress(VK_RETURN)) // wait for a keyrelease so it doesnt zoom through while holding enter
        {

        }

        vcursor.y += 2;

        vcursor.y = vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;
        pressEnter(vcursor, vwin);
    }
    
    if (overFlowXp > 0)
    {
        addExperience(overFlowXp, dngutil::EvType::NONE);
    }
}

int Player::getExp() const
{
    return exp;
}

int Player::getPlayersExpToLevel() const
{
    return expToLevel;
}

void Player::levelUpStats()
{
    Creature::levelUpStats();
    std::array<double, 5> stats;
    stats[0] = hpEv; stats[1] = attEv; stats[2] = defEv; stats[3] = lckEv; stats[4] = spdEv;

    for (unsigned int i = 0; i < stats.size(); i++)
    {
        int boost = static_cast<int>(stats[i]);
        switch (i)
        {
        case 0:
            increaseHealth(boost);
            hpEv -= boost;
            break;
        case 1:
            increaseAtt(boost);
            attEv -= boost;
            break;
        case 2:
            increaseDef(boost);
            defEv -= boost;
            break;
        case 3:
            increaseLck(boost);
            lckEv -= boost;
            break;
        case 4:
            increaseSpd(boost);
            spdEv -= boost;
            break;
        }
    }

    int h = getPGame()->getDifficulty().healthIncreaseBoost;
    if (h > 0)
    {
        increaseHealth(h);
    }
    else
    {
        decreaseHealth(h);
    }
}

void Player::inventoryMenu() // How not to program in three easy steps. 1: Dont do this.
{
    VirtualWindow* v = getPGame()->getVWin();
    TextMacros& t = v->txtmacs;

    int topline = t.POSITION_FOR_TOP_DIVIDER_TEXT;
    t.clearLine(topline);
    t.clearDivider("bottom");
    v->putcen(ColorString("Press Esc to exit inventory", dngutil::WHITE), topline);

    const int PAGE_SIZE = 30;
    double temp = static_cast<double>(inventory.size()) / PAGE_SIZE;
    const int numberOfPages = static_cast<int>(ceil(temp));

    std::vector<std::vector<Item*>> invMenu;
    invMenu.resize(numberOfPages);

    for (int i = 0; i < static_cast<int>(inventory.size()); i++)
    {
        //for (int j = (i * PAGE_SIZE); (j < ((i + 1) * PAGE_SIZE)) && (j < inventory.size()); j++)
        {
            invMenu[static_cast<int>(i / PAGE_SIZE)].push_back(inventory[i]);
        }
    }

    int pageLine = topline + 1;
    int currentPage = 0;
    int topcoord = t.DIVIDER_LINES[1] + 3; // this is also in the displayInventory function
    int bottomcoord;
    int currentcoord = topcoord;

    auto displayPage = [&](int page)
    {
        if (page < 0 || page >= numberOfPages)
        {
            if (inventory.empty())
            {
                t.clearLine(pageLine);
                t.clearMapArea(false, NULL);
                std::vector<Item*> emptyVec;
                t.displayInventory(emptyVec, this);
                return;
            }

            errorMessage("Error in PAGE NUMBERS while displaying inventory", __LINE__, __FILE__);
        }

        bool newPage;
        if (page == currentPage)
        {
            newPage = false;
        }
        else
        {
            newPage = true;
        }

        t.clearLine(pageLine);
        t.clearMapArea(false, NULL);
        std::string pageNumberString = " Page " + std::to_string(page) + " ";
        if (page > 0)
        {
            pageNumberString = "<--" + pageNumberString;
        }
        if ((page >= 0) && (page < (numberOfPages - 1)))
        {
            pageNumberString += "-->";
        }
        if (page == (numberOfPages - 1))
        {
            bottomcoord = topcoord + ((PAGE_SIZE - 1) - ((numberOfPages * PAGE_SIZE) - (inventory.size()))); // lol what
        }
        else
        {
            bottomcoord = topcoord + (PAGE_SIZE - 1);
        }

        if (newPage)
        {
            currentPage = page;
            currentcoord = topcoord;
        }

        v->putcen(ColorString(pageNumberString, dngutil::WHITE), pageLine);
        t.displayInventory(invMenu[page], this);
    };

    displayPage(0);
    while (true)
    {
        v->put(ColorString("->", dngutil::LIGHTCYAN), Coordinate(0, currentcoord));
        if (keypress(VK_LEFT))
        {
            if (currentPage > 0)
            {
                displayPage(currentPage - 1);
                continue;
            }
        }
        if (keypress(VK_RIGHT))
        {
            if (currentPage < (numberOfPages - 1))
            {
                displayPage(currentPage + 1);
                continue;
            }
        }
        if (keypress(VK_UP))
        {
            Sleep(dngutil::MENU_DELAY);
            v->put(ColorString("  ", dngutil::LIGHTGRAY), Coordinate(0, currentcoord));

            if (currentcoord > topcoord)
            {
                currentcoord--;
            }
            else
            {
                currentcoord = bottomcoord;
            }

            continue;
        }
        if (keypress(VK_DOWN))
        {
            Sleep(dngutil::MENU_DELAY);
            v->put(ColorString("  ", dngutil::LIGHTGRAY), Coordinate(0, currentcoord));

            if (currentcoord < bottomcoord)
            {
                currentcoord++;
            }
            else
            {
                currentcoord = topcoord;
            }

            continue;
        }
        if (keypress(VK_RETURN) && !inventory.empty())
        {
            Sleep(dngutil::MENU_DELAY * 2);
            unsigned int itemIndex = (currentPage * PAGE_SIZE) + abs(currentcoord - topcoord);
            Item* itemModifying = inventory[itemIndex];
            itemModifying->action(this, itemIndex);

            if (itemModifying->isConsumable())
            {
                delete itemModifying;
                std::vector<Item*>::iterator it = inventory.begin();
                it += itemIndex;
                inventory.erase(it);
            }

            pressEnter(Coordinate(0, t.BOTTOM_DIVIDER_TEXT_LINE + 1), v);
            inventoryMenu();
            break;
        }
        if (keypress(VK_ESCAPE))
        {
            break;
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

bool Player::swapArmor(Item*& itemToSwap)
{
    Item* tempItem = itemToSwap;
    Equipment* backupArmor = armor;

    itemToSwap = armor;
    armor = dynamic_cast<Equipment*>(tempItem);
    if (armor == nullptr)
    {
        armor = backupArmor;
        itemToSwap = tempItem;
        return false;
    }
    return true;
}

bool Player::swapBoots(Item*& itemToSwap)
{
    Item* tempItem = itemToSwap;
    Equipment* backupBoots = boots;

    itemToSwap = boots;
    boots = dynamic_cast<Equipment*>(tempItem);
    if (boots == nullptr)
    {
        boots = backupBoots;
        itemToSwap = tempItem;
        return false;
    }
    return true;
}

Equipment*& Player::getBootsMemory()
{
    return boots;
}

Equipment*& Player::getArmorMemory()
{
    return armor;
}

Inventory& Player::getInventoryNotConst()
{
    return inventory;
}

void Player::mapMenu()
{
    VirtualWindow* v = getPGame()->getVWin();
    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");

    Coordinate origin(40, v->txtmacs.DIVIDER_LINES[1] + 20);

    int oldActiveFloor = getPGame()->getRawFloor();
    getPGame()->setActiveFloor(2);
    for (auto i : getPGame()->getActiveFloor())
    {
        Coordinate position(origin.x + i.first.x, origin.y + i.first.y);
        ColorChar toDraw;
        toDraw.color = i.second->getRoomInfo().backColor;
        if (i.second == getPGame()->getActiveRoom())
        {
            toDraw = getPGame()->getPlayer()->getMapRep();
        }
        else
        {
            if (i.second->getMusic().getFilename() == "DungeonTheme" || !i.second->shouldDisplayOnMap())
            {
                toDraw.character = ' ';
            }
            else
            {
                toDraw.character = '.';
            }
        }

        
        v->put(toDraw, position);
    }

    getPGame()->setActiveFloor(oldActiveFloor);

    v->putcen(ColorString("- The Kingdom of Bora -", dngutil::YELLOW), v->txtmacs.DIVIDER_LINES[1] + 2);
    pressEnter(Coordinate(0, v->txtmacs.BOTTOM_DIVIDER_TEXT_LINE), v);

    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");
    v->txtmacs.displayGame(getPGame());
}

void Player::statsMenu()
{
    VirtualWindow* v = getPGame()->getVWin();
    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");
    int line = v->txtmacs.DIVIDER_LINES[1] + 3;
    v->putcen(ColorString(getPrimary().getName() + " Stats", dngutil::LIGHTGRAY), line++);



    v->putcen(ColorString("Attack Type: " + getClassName(getPrimary().getClass()), dngutil::LIGHTGREEN), line++);
    v->putcen(ColorString("Attack Speed: " + std::to_string(getPrimary().getAttSpeed()) + " seconds", dngutil::LIGHTBLUE), line++);
    v->putcen(ColorString("Damage Multiplier: " + std::to_string(getPrimary().getDmgMultiplier()), dngutil::LIGHTRED), line++);
    v->putcen(ColorString("Accuracy: " + std::to_string(getPrimary().getAccuracy()) + "%", dngutil::LIGHTMAGENTA), line++);

    line += 2;
    v->putcen(ColorString(getSecondary().getName() + " Stats", dngutil::LIGHTGRAY), line++);
    v->putcen(ColorString("Deflect Time: " + std::to_string(getSecondary().getDeflectTime()) + " miliseconds", dngutil::LIGHTBLUE), line++);
    v->putcen(ColorString("Defense Multiplier: " + std::to_string(getSecondary().getDefenseBoost()), dngutil::LIGHTRED), line++);

    line = v->txtmacs.BOTTOM_DIVIDER_TEXT_LINE;

    pressEnter(Coordinate(0, line), v);
    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");
    v->txtmacs.displayGame(getPGame());
}

ColorString Player::getStepString()
{
    int color;
    if (steps > static_cast<int>(getPGame()->getDifficulty().beastSteps * .66))
    {
        color = dngutil::GREEN;
    }
    else if (steps > static_cast<int>(getPGame()->getDifficulty().beastSteps * .33))
    {
        color = dngutil::YELLOW;
    }
    else
    {
        color = dngutil::RED;
    }

    return ColorString(std::to_string(steps), color);
}

int Player::getStepCount()
{
    return steps;
}
//------------------------------------------------------------