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


//------------------------------------------------------------
// Player Functions

Player::Player(
    Game* pgame,
    Coordinate coord
)
    :Creature(
        pgame,
        ColorChar('A', pgame->getDifficulty().color),
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
            100,
            1,
            "A very simple shield."
        ),
        dngutil::P_PLAYER
    )
{
    this->exp = 0;
    this->expToLevel = getExpToLevel(getLvl());
    inventory.push_back(new Potion(getPGame(), Coordinate(-1, -1), dngutil::POTION_HEAL));

    hpEv = 0;
    attEv = 0;
    defEv = 0;
    lckEv = 0;
    spdEv = 0;


    steps = pgame->getDifficulty().beastSteps;
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

void Player::dungeonBeastSequence()
{
    VirtualWindow* v = getPGame()->getVWin();
    TextMacros& t = v->txtmacs;
    stopMp3();
    Sleep(150);
    soundEffect("EnterBattle.wav", false, true);
    t.clearMapArea(true, 20);
    t.clearDivider("bottom");

    Coordinate vcursor(15, t.DIVIDER_LINES[1] + 1);

    startMp3("BeastTheme.mp3");
    int color = dngutil::WHITE;
    v->put(ColorString(R"(		             \                  /)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		    _________))                ((__________)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		   /.-------./\\    \    /    //\.--------.\)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		  //#######//##\\   ))  ((   //##\\########\\)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		 //#######//###((  ((    ))  ))###\\########\\)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		((#######((#####\\  \\  //  //#####))########)))", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		 \##' `###\######\\  \)(/  //######/####' `##/)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		  )'    ``#)'  `##\`->oo<-'/##'  `(#''     `()", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		          (       ``\`..'/''       ))", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                     \""()", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                      `- ))", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                      / /)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                     ( /\)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                     /\| \)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                    (  \)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                        ))", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                       /)", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                      ()", color), vcursor); Sleep(50); vcursor.y++;
    v->put(ColorString(R"(		                      ` )", color), vcursor); Sleep(50);
    
    soundEffect("Screech.wav", false, false); 

    vcursor.y = t.BOTTOM_DIVIDER_TEXT_LINE;
    v->putcen(ColorString("The Dungeon Beast has appeared!", dngutil::WHITE), vcursor.y++);
    pressEnter(vcursor, v);
    t.clearDivider("bottom");
    int healthbarLine = t.DIVIDER_LINES[2] + 4;
    int healthDecrease = static_cast<int>(getHp() / 2.0);
    int sleepTime = static_cast<int>(5000.0 / healthDecrease);

    soundEffect("HealthDrain.wav", false, true);
    for (int i = 0; i < healthDecrease; i++)
    {
        decreaseHealth(1);
        v->putcen(getHealthBar(), healthbarLine);
        Sleep(sleepTime); vcursor.y++;
    }

    t.clearMapArea(true, 50);
    t.clearDivider("bottom");
    t.displayGame(getPGame());

    stopMp3();

    startMp3("Overworld.mp3");
    soundEffect("ExitToMap.wav", false, true);
}

bool Player::movement()
{
    int adjustedSpeed = static_cast<int>(dngutil::MAX_SPD * 1.5);
    bool canmove = !((getLastMoveTime() + ((adjustedSpeed - getSpd())) > GetTickCount()));

    if (steps < 1 && getPGame()->shouldSpawnBeast())
    {
        dungeonBeastSequence();
        steps = getPGame()->getDifficulty().beastSteps;
    }

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
            Sleep(50); vcursor.y++;
            soundEffect("FindItem.wav", false, false);
            break;
        }

        if (!keypress(VK_RETURN))
        {
            Sleep(50); vcursor.y++;
        }
    }
    stopSound();
    Sleep(450);
    vwin->txtmacs.clearMapArea(false, NULL);

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
        getPGame()->adjustScore(dngutil::BASE_SCORE_INCREASE_LEVEL);

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

        setHp(getMaxhp());
    }
    
    if (overFlowXp > 0)
    {
        addExperience(overFlowXp, dngutil::EvType::NONE);
    }
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