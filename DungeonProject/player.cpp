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
        20, // speed
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
    )
{
    harp1 = true;
    harp2 = true;
    harp3 = true;
    harp4 = true;

    startingDungeonMapCoord = Coordinate(0, 0);
    startingDungeonRoomCoord = Coordinate(0, 0);

    this->exp = 0;
    this->expToLevel = getExpToLevel(getLvl());
    inventory.push_back(new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL));

    armor = new BlueTunic(getPGame(), Coordinate(-1, -1));
    boots = new StandardBoots(getPGame(), Coordinate(-1, -1));

    inventory.push_back(new Waterboots(getPGame(), Coordinate(-1, -1)));
    inventory.push_back(new Bubblecharm(getPGame(), Coordinate(-1, -1)));
    for (int i = 0; i < 15; i++)
    {
        increaseLvl(1);
        levelUpStats();
    }

    hpEv = 0;
    attEv = 0;
    defEv = 0;
    lckEv = 0;
    spdEv = 0;

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

void Player::chooseClass()
{
    VirtualWindow* v = getPGame()->getVWin();
    TextMacros& t = v->txtmacs;

    t.clearMapArea(false, NULL);
    t.clearDivider("bottom");
    Coordinate vcursor(0, t.DIVIDER_LINES[1] + 5);

    playSound(WavFile("Levelup", false, true));

    v->putcen(ColorString("Choose your class:", dngutil::LIGHTBLUE), vcursor.y++);
    vcursor.y++;

    v->putcen(ColorString("(1) - Knight [+15 Health, Melee Weapons]", dngutil::WHITE), vcursor.y++);
    vcursor.y += 2;

    v->putcen(ColorString("(2) - Ranger [+50 Speed, +25 luck, Bows/Guns]", dngutil::WHITE), vcursor.y++);
    vcursor.y += 2;

    v->putcen(ColorString("(3) - Wizard [+5 Attack, +30 Speed, Magical Weapons]", dngutil::WHITE), vcursor.y++);

    while (true)
    {
        if (keypress('1'))
        {
            setClass(dngutil::ClassType::KNIGHT);
            increaseMaxhp(15);
            increaseHealth(15);
            inventory.push_back(
                new Primary(
                    getPGame(),
                    ColorChar('I', dngutil::WHITE),
                    Coordinate(-1, -1),
                    "Lance",
                    false,
                    1.5,
                    3,
                    80,
                    false,
                    "A Knight's lance",
                    WavFile("Attack3", false, false),
                    dngutil::ClassType::KNIGHT
                )
            );
            break;
        }
        if (keypress('2'))
        {
            setClass(dngutil::ClassType::RANGER);
            increaseSpd(50);
            increaseLck(25);
            inventory.push_back(
                new Primary(
                    getPGame(),
                    ColorChar('(', dngutil::GREEN),
                    Coordinate(-1, -1),
                    "Bow of the Wind",
                    false,
                    1.1,
                    2,
                    70,
                    false,
                    "A Ranger's bow, blessed by the wind",
                    WavFile("BowAttack1", false, false),
                    dngutil::ClassType::RANGER
                )
            );
            break;
        }
        if (keypress('3'))
        {
            setClass(dngutil::ClassType::WIZARD);
            increaseAtt(2);
            increaseSpd(30);
            inventory.push_back(
                new Primary(
                    getPGame(),
                    ColorChar('i', dngutil::GREEN),
                    Coordinate(-1, -1),
                    "Wand",
                    false,
                    1.4,
                    3,
                    100,
                    false,
                    "A Wizard's magical wand",
                    WavFile("MagicAttack1", false, false),
                    dngutil::ClassType::WIZARD
                )
            );
            break;
        }
    }
    t.clearMapArea(false, NULL);

    vcursor.y = t.BOTTOM_DIVIDER_TEXT_LINE;
    v->putcen(ColorString("You have created a weapon for your new skills", dngutil::WHITE), vcursor.y++);
    pressEnter(vcursor, v);
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
        else if (keypress('U'))
        {
            statsMenu();
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

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    return ColorString(((std::string((unsigned int)numOfCircles, '=')) + temp), dngutil::MAGENTA);
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
    int topline = getPGame()->getVWin()->txtmacs.POSITION_FOR_TOP_DIVIDER_TEXT;
    getPGame()->getVWin()->txtmacs.clearLine(topline);
    getPGame()->getVWin()->putcen(ColorString("Press Esc to exit inventory", dngutil::WHITE), topline);


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
            else if (keypress('U') && !inventory.empty())
            {
                Sleep(dngutil::MENU_DELAY);
                unsigned int itemPosition = (abs(positions[0] - vcursor.y));
                Item* itemModifying = inventory[itemPosition];

                switch (itemModifying->getTypeId())
                {
                case dngutil::TID::Primary:
                {
                    Primary* primary = dynamic_cast<Primary*>(itemModifying);
                    int attackColor;
                    std::string attackSign;
                    if (getPrimary().getDmgMultiplier() > primary->getDmgMultiplier())
                    {
                        attackColor = dngutil::RED;
                        attackSign = "- ";
                    }
                    else
                    {
                        attackColor = dngutil::GREEN;
                        attackSign = "+ ";
                    }

                    int speedColor;
                    std::string speedSign;
                    if (getPrimary().getAttSpeed() > primary->getAttSpeed())
                    {
                        speedColor = dngutil::GREEN;
                        speedSign = "- ";
                    }
                    else
                    {
                        speedColor = dngutil::RED;
                        speedSign = "+ ";
                    }

                    getPGame()->getVWin()->putcen(
                        ColorString(attackSign + std::to_string(abs(primary->getDmgMultiplier() - getPrimary().getDmgMultiplier())) + " Damage ", attackColor) +
                        ColorString(speedSign + std::to_string(abs(primary->getAttSpeed() - getPrimary().getAttSpeed())) + " Speed", speedColor),
                        getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE
                    );
                    pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin());

                    break;
                }
                case dngutil::TID::Secondary:
                {
                    Secondary* secondary = dynamic_cast<Secondary*>(itemModifying);
                    int deflectColor;
                    std::string deflectSign;
                    if (getSecondary().getDeflectTime() > secondary->getDeflectTime())
                    {
                        deflectColor = dngutil::RED;
                        deflectSign = "- ";
                    }
                    else
                    {
                        deflectColor = dngutil::GREEN;
                        deflectSign = "+ ";
                    }

                    int defenseColor;
                    std::string defenseSign;
                    if (getSecondary().getDefenseBoost() > secondary->getDefenseBoost())
                    {
                        defenseColor = dngutil::RED;
                        defenseSign = "- ";
                    }
                    else
                    {
                        defenseColor = dngutil::GREEN;
                        defenseSign = "+ ";
                    }

                    getPGame()->getVWin()->putcen(
                        ColorString(deflectSign + std::to_string(abs(secondary->getDeflectTime() - getSecondary().getDeflectTime())) + " Deflect Time ", deflectColor) +
                        ColorString(defenseSign + std::to_string(abs(secondary->getDefenseBoost() - getSecondary().getDefenseBoost())) + " Defense", defenseColor),
                        getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE
                    );
                    pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1), getPGame()->getVWin());
                    break;
                }

                }
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

void Player::statsMenu()
{
    VirtualWindow* v = getPGame()->getVWin();
    v->txtmacs.clearMapArea(false, NULL);
    v->txtmacs.clearDivider("bottom");
    int line = v->txtmacs.DIVIDER_LINES[1] + 3;
    v->putcen(ColorString(getPrimary().getName() + " Stats", dngutil::LIGHTRED), line++);
    v->putcen(ColorString("Attack Speed: " + std::to_string(getPrimary().getAttSpeed()) + " seconds", dngutil::LIGHTRED), line++);
    v->putcen(ColorString("Damage Multiplier: " + std::to_string(getPrimary().getDmgMultiplier()), dngutil::LIGHTRED), line++);
    v->putcen(ColorString("Accuracy: " + std::to_string(getPrimary().getAccuracy()) + "%", dngutil::LIGHTRED), line++);

    line += 2;
    v->putcen(ColorString(getSecondary().getName() + " Stats", dngutil::LIGHTBLUE), line++);
    v->putcen(ColorString("Deflect Time: " + std::to_string(getSecondary().getDeflectTime()) + " miliseconds", dngutil::LIGHTBLUE), line++);
    v->putcen(ColorString("Defense Multiplier: " + std::to_string(getSecondary().getDefenseBoost()), dngutil::LIGHTBLUE), line++);

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