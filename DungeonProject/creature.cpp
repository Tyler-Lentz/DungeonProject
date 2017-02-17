#include "creature.h"
#include "mapobject.h"
#include "utilities.h"
#include "item.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "virtualwindow.h"
#include "room.h"
#include "coordinate.h"

#include <list>
#include <string>
#include <chrono>

//------------------------------------------------------------
// Creature Functions
Creature::Creature(
    Game* pgame,
    ColorChar mapRep,
    Coordinate coord,
    std::string name,
    bool moveable,
    bool rawoutput,
    bool aggressive,
    dngutil::TID typeId,
    int hp,
    unsigned int att,
    unsigned int def,
    unsigned int lck,
    unsigned int spd,
    unsigned int lvl,
    Primary* primary,
    Secondary* secondary,
    int priority,
    dngutil::ClassType classType
)
    :MapObject(
        pgame,
        mapRep,
        coord,
        name,
        moveable,
        rawoutput,
        aggressive,
        typeId,
        priority,
        dngutil::BTID::Creature,
        false
    )
{
    this->hp = hp;
    this->maxhp = hp;
    this->att = att;
    this->def = def;
    this->lck = lck;
    this->spd = spd;
    this->lvl = lvl;

    this->primary = primary;
    this->secondary = secondary;

    this->lastMoveTime = 0;
    this->lastMovement = dngutil::Movement::UP;

    this->canMiss = true;

    this->classType = classType;
}

Creature::~Creature()
{
    if (primary != nullptr)
    {
        delete primary;
    }
    if (secondary != nullptr)
    {
        delete secondary;
    }
}

bool Creature::isDead() const
{
    return (hp < 1);
}

dngutil::ClassType Creature::getClass()
{
    return classType;
}

const unsigned int& Creature::getMaxhp() const
{
    return maxhp;
}

const int& Creature::getHp() const
{
    return hp;
}

const unsigned int& Creature::getAtt() const
{
    return att;
}

const unsigned int& Creature::getDef() const
{
    return def;
}

const unsigned int& Creature::getLck() const
{
    return lck;
}

const unsigned int& Creature::getSpd() const
{
    return spd;
}

const unsigned int& Creature::getLvl() const
{
    return lvl;
}

const Primary& Creature::getPrimary() const
{
    return *primary;
}

const unsigned long& Creature::getLastMoveTime() const
{
    return lastMoveTime;
}

Primary*& Creature::getPrimaryMemory()
{
    return primary;
}

void Creature::setPrimary(Primary* primary)
{
    this->primary = primary;
}

const Secondary& Creature::getSecondary() const
{
    return *secondary;
}

Secondary*& Creature::getSecondaryMemory()
{
    return secondary;
}

void Creature::setSecondary(Secondary* secondary)
{
    this->secondary = secondary;
}

void Creature::increaseMaxhp(unsigned int amount)
{
    maxhp += amount;
    if (getMaxhp() > dngutil::MAX_HP)
    {
        maxhp = dngutil::MAX_HP;
    }
}

void Creature::increaseAtt(unsigned int amount)
{
    att += amount;
    if (att > dngutil::MAX_ATT)
    {
        att = dngutil::MAX_ATT;
    }
}

void Creature::increaseDef(unsigned int amount)
{
    def += amount;
    if (def > dngutil::MAX_DEF)
    {
        def = dngutil::MAX_DEF;
    }
}

void Creature::increaseSpd(unsigned int amount)
{
    spd += amount;
    if (spd > dngutil::MAX_SPD)
    {
        spd = dngutil::MAX_SPD;
    }
}

void Creature::increaseLvl(unsigned int amount)
{
    lvl += amount;
}

void Creature::increaseLck(unsigned int amount)
{
    lck += amount;
    if (lck > dngutil::MAX_LCK)
    {
        lck = dngutil::MAX_LCK;
    }
}

void Creature::setMaxhp(unsigned int amount)
{
    maxhp = amount;
    if (maxhp > dngutil::MAX_HP)
    {
        maxhp = dngutil::MAX_HP;
    }
}

void Creature::setHp(int amount)
{
    hp = amount;
    if (hp > static_cast<int>(maxhp))
    {
        hp = maxhp;
    }
}

void Creature::setAtt(unsigned int amount)
{
    att = amount;
    if (att > dngutil::MAX_ATT)
    {
        att = dngutil::MAX_ATT;
    }
}

void Creature::setDef(unsigned int amount)
{
    def = amount;
    if (def > dngutil::MAX_DEF)
    {
        def = dngutil::MAX_DEF;
    }
}

void Creature::setSpd(unsigned int amount)
{
    spd = amount;
    if (spd > dngutil::MAX_SPD)
    {
        spd = dngutil::MAX_SPD;
    }
}

void Creature::setLvl(unsigned int amount)
{
    lvl = amount;
}

void Creature::setLck(unsigned int amount)
{
    lck = amount;
    if (lck > dngutil::MAX_LCK)
    {
        lck = dngutil::MAX_LCK;
    }
}

unsigned int Creature::increaseHealth(unsigned int amount)
{
    unsigned int tempHealth = hp;
    hp += amount;
    if (static_cast<unsigned int>(hp) > maxhp)
    {
        hp = maxhp;
    }
    return hp - tempHealth;
}

unsigned int Creature::decreaseHealth(unsigned int amount)
{
    hp -= amount;
    return amount;
}

ColorString Creature::getHealthBar() const
{
    int adjustedHealth = hp;
    if (adjustedHealth < 0)
    {
        adjustedHealth = 0;
    }
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / (double)maxhp);

    int numOfCircles = (int)(scaleFactor * adjustedHealth);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    std::string healthbar = temp + (std::string((unsigned int)numOfCircles, '='));

    int color;
    if (hp > static_cast<int>(maxhp * .66))
    {
        color = dngutil::GREEN;
    }
    else if (hp > static_cast<int>(maxhp * .33))
    {
        color = dngutil::YELLOW;
    }
    else
    {
        color = dngutil::RED;
    }

    return ColorString(healthbar, color);
}

bool Creature::battle(MapObject* t_enemy)
{
    Enemy* enemy = dynamic_cast<Enemy*>(t_enemy);
    Player* player = getPGame()->getPlayer();
    VirtualWindow* vwin = getPGame()->getVWin();

    if (enemy->getLvl() + 1 < player->getLvl())
    {
        enemy->increaseLvl(1);
        enemy->levelUpStats();
    }

    int enemyWeaponSpeed = enemy->getPrimary().getAttSpeed();
    int enemyTimer = 0;

    int playerWeaponSpeed = player->getPrimary().getAttSpeed();
    int playerTimer = 0;
    if (player->getPrimary().getStartReady())
    {
        playerTimer = playerWeaponSpeed;
    }

    if (enemy->getPrimary().getStartReady())
    {
        enemyTimer = enemyWeaponSpeed;
    }

    int prevTime = (int)time(NULL);

    vwin->txtmacs.clearMapArea(true, 20);
    vwin->txtmacs.clearDivider("bottom");

    startMp3(enemy->getBattleMusic());

    while (true)
    {
        enemy->printSelf();
        vwin->txtmacs.displayHealthBars(enemy, player);

        while (true)
        {

            if (prevTime != (int)time(NULL))
            {
                if (playerTimer < playerWeaponSpeed)
                {
                    if (!keypress(VK_RETURN))
                    {
                        playerTimer++;
                    }
                }
                if (enemyTimer < enemyWeaponSpeed)
                {
                    enemyTimer++;
                }
                prevTime = (int)time(NULL);
            }

            vwin->txtmacs.outputBattleInfo(playerTimer, playerWeaponSpeed, enemyTimer, enemyWeaponSpeed);

            if (playerTimer >= playerWeaponSpeed && keypress(VK_RETURN))
            {
                playerTimer = 0;

                Damage damage = player->getDamageDealt(enemy);
                for (int i = 0; i < damage.damage; i++)
                {
                    enemy->decreaseHealth(1);
                    vwin->txtmacs.displayHealthBars(enemy, player);
                    Sleep(getHealthbarSleepTime(damage.damage));
                }

                vwin->putcen(
                    ColorString("-" + std::to_string(damage.damage), dngutil::GREEN) +
                    ((damage.damageDeflected > 0) ? ColorString(", " + std::to_string(damage.damageDeflected) + " DEFLECTED", dngutil::GREEN) : ColorString("", dngutil::BLACK))
                    , vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);
                Sleep(300);

                if (enemy->isDead())
                {
                    enemy->printSelf();
                    vwin->txtmacs.displayHealthBars(enemy, player);
                    stopMp3();

                    Sleep(100);
                    soundEffect(enemy->getDeathSound(), false, false);

                    enemy->deathSequence();

                    getPGame()->getActiveRoom()->setAll();
                    getPGame()->getVWin()->txtmacs.clearMapArea(true, 20);
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                    pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE), getPGame()->getVWin());
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                    startMp3(getPGame()->getOverworldMusic());
                    soundEffect("ExitToMap.wav", false, true);

                    getPGame()->getActiveRoom()->getCreatureList().remove(enemy);
                    return true;
                }
                break;
            }

            if (enemyTimer >= enemyWeaponSpeed)
            {
                enemyTimer = 0;
                Damage damage = enemy->getDamageDealt(player);
                for (int i = 0; i < damage.damage; i++)
                {
                    player->decreaseHealth(1);
                    vwin->txtmacs.displayHealthBars(enemy, player);
                    Sleep(getHealthbarSleepTime(damage.damage));
                }

                vwin->putcen(
                    ColorString("-" + std::to_string(damage.damage), dngutil::GREEN) +
                    ((damage.damageDeflected > 0) ? ColorString(", " + std::to_string(damage.damageDeflected) + " DEFLECTED", dngutil::GREEN) : ColorString("", dngutil::BLACK))
                    , vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);
                Sleep(300);

                if (player->isDead())
                {
                    bool revived = false;

                    for (auto it = player->getInventoryNotConst().begin(); it != player->getInventoryNotConst().end(); it++)
                    {
                        if ((*it)->getTypeId() == dngutil::TID::MagicalPotion)
                        {
                            Item* potion = *it;
                            player->getInventoryNotConst().erase(it);
                            delete potion;
                            revived = true;
                            break;
                        }
                    }

                    if (!revived)
                    {
                        stopMp3();
                        getPGame()->cleanup(getPGame()->getVWin()->txtmacs.deathScreen(getPGame()));
                        return false;
                    }
                    else
                    {
                        player->setHp(0);
                        vwin->txtmacs.displayHealthBars(enemy, player);
                        vwin->txtmacs.outputBattleInfo(playerTimer, playerWeaponSpeed, enemyTimer, enemyWeaponSpeed);

                        soundEffect("MagicalPotion.wav", false, false);
                        soundEffect("RefillHealth.wav", true, true);
                        for (int i = 0; i < static_cast<int>(player->getMaxhp() * .75); i++)
                        {
                            player->increaseHealth(1);
                            vwin->txtmacs.displayHealthBars(enemy, player);
                            Sleep(40);
                        }
                        stopSound();
                    }
                }
                break;
            }

            if (keypress('I'))
            {
                player->inventoryMenu();
                getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                if (player->getPrimary().getAttSpeed() != playerTimer)
                {
                    playerWeaponSpeed = player->getPrimary().getAttSpeed();
                    playerTimer = 0;
                }
                break;
            }
        }
    }
}

void Creature::setClass(dngutil::ClassType classType)
{
    this->classType = classType;
}

std::string Creature::getClassName()
{
    switch (classType)
    {
    case dngutil::ClassType::KNIGHT:
        return "Knight";
        break;
    case dngutil::ClassType::WIZARD:
        return "Wizard";
        break;
    case dngutil::ClassType::RANGER:
        return "Ranger";
        break;
    case dngutil::ClassType::ADVENTURER:
        return "Adventurer";
        break;
    default:
        return "INVALID CLASS";
        break;
    }
}

bool Creature::adjustPosition(dngutil::Movement movement)
{
    int newX, newY;
    lastMovement = movement;
    switch (movement)
    {
    case dngutil::Movement::UP:
        newX = getCoord().x;
        newY = getCoord().y - 1;
        break;

    case dngutil::Movement::DOWN:
        newX = getCoord().x;
        newY = getCoord().y + 1;
        break;

    case dngutil::Movement::LEFT:
        newX = getCoord().x - 1;
        newY = getCoord().y;
        break;

    case dngutil::Movement::RIGHT:
        newX = getCoord().x + 1;
        newY = getCoord().y;
        break;
    }

    Coordinate newCoord(newX, newY);
    
    // Setting this to the current map coord in the case it needs it.
    Coordinate newMapCoord(getPGame()->getActiveRoom()->getRoomInfo().mapCoord);

    switch (getPGame()->getActiveRoom()->checkMovement(newCoord, this))
    {
    case dngutil::MovementTypes::VALID:
        lastMoveTime = GetTickCount();
        if (this == getPGame()->getPlayer())
        {
            getPGame()->getPlayer()->minusStep();
        }

        getPGame()->getActiveRoom()->addCoordToList(getCoord());
        getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);

        setPosition(newCoord);

        getPGame()->getActiveRoom()->addCoordToList(getCoord());

        sortPriority(getPGame()->getActiveRoom()->getObjects(getCoord()), this);


        for (auto it = getPGame()->getActiveRoom()->getObjects(getCoord()).begin();
            it != getPGame()->getActiveRoom()->getObjects(getCoord()).end(); )
        {
            Collision c = (*it)->mapAction(this, it);
            if (c.exitCollisions)
            {
                if (c.returnTrue)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }
            if (c.iterate)
            {
                it++;
            }
        }
        break;
    case dngutil::MovementTypes::NEW_ROOM:
    {
        switch (movement)
        {
        case dngutil::Movement::UP:
            newMapCoord.y--;
            break;

        case dngutil::Movement::DOWN:
            newMapCoord.y++;
            break;

        case dngutil::Movement::LEFT:
            newMapCoord.x--;
            break;

        case dngutil::Movement::RIGHT:
            newMapCoord.x++;
            break;
        }

        if (getPGame()->getActiveFloor().count(newMapCoord) != 1)
        {
            errorMessage("There is not a floor where you are trying to go.", __LINE__, __FILE__);
        }

        Coordinate* coordToTest = nullptr;
        switch (movement)
        {
        case dngutil::Movement::UP:
            coordToTest = new Coordinate(getCoord().x, getPGame()->getActiveFloor()[newMapCoord]->getRoomY() - 1);
            break;
        case dngutil::Movement::DOWN:
            coordToTest = new Coordinate(getCoord().x, 0);
            break;
        case dngutil::Movement::LEFT:
            coordToTest = new Coordinate(getPGame()->getActiveFloor()[newMapCoord]->getRoomX() - 1, getCoord().y);
            break;
        case dngutil::Movement::RIGHT:
            coordToTest = new Coordinate(0, getCoord().y);
            break;
        }
        if (coordToTest == nullptr)
        {
            errorMessage("coordToTest is a nullptr.", __LINE__, __FILE__);
        }

        if (getPGame()->getActiveFloor()[newMapCoord]->checkMovement(*coordToTest, this) == dngutil::MovementTypes::VALID)
        {
            getPGame()->getActiveRoom()->getObjects(getCoord()).remove(this);
            Room* tempOldRoom = getPGame()->getActiveRoom();
            getPGame()->setActiveRoom(getPGame()->getActiveFloor()[newMapCoord]);

            setPosition(*coordToTest);

            getPGame()->getActiveRoom()->getObjects(getCoord()).push_back(this);
            getPGame()->getVWin()->txtmacs.screenScroll(movement, tempOldRoom, getPGame()->getActiveRoom(), getPGame());

            getPGame()->clearDeletionList();
            return true;
        }

        delete coordToTest;
        break;
    }

    case dngutil::MovementTypes::RETURN_TRUE:
        return true;
        break; // just in case...
        
        // invalid movement does nothinG
    }

    lastMoveTime = GetTickCount();
    return false;
}

Damage Creature::getDamageDealt(Creature* defender)
{
    Damage damage(0, 0);
    bool deflect = false;
    double attack = getAtt();
    double defense = defender->getDef();

    attack *= getPrimary().getDmgMultiplier();
    defense *= defender->getSecondary().getDefenseBoost();
    if (this == getPGame()->getPlayer())
    {
        defense *= .8;
        attack *= getPGame()->getDifficulty().damageMultiplier;
    }

    attack += (random(static_cast<int>(attack / 3.0), static_cast<int>(attack / 2.0)));
    defense += (random(static_cast<int>(defense / 3.0), static_cast<int>(defense / 2.0)));

    bool miss = !primary->hit() && canMiss;

    if (defender == getPGame()->getPlayer() && !miss)
    {
        bool canDeflect = true;

        // Prevent holding down the key
        if (keypress(VK_SPACE))
        {
            canDeflect = false;
        }

        auto start = GetTickCount();
        int timeWindow = defender->getSecondary().getDeflectTime();

        getPGame()->getVWin()->putcen(ColorString("!", dngutil::CYAN), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);

        while ((start + timeWindow) > GetTickCount() && canDeflect)
        {
            if (keypress(VK_SPACE))
            {
                soundEffect("ShieldDeflect.wav", false, false);
                deflect = true;
            }
        }
    }

    soundEffect(getPrimary().getHitsound(), false, false);

    if (!miss)
    {
        canMiss = true;
        int critChance = dngutil::MAX_LCK;
        critChance -= getLck();
        if (critChance < 0)
        {
            critChance = 0;
        }

        bool crit = (random(critChance) == 0);

        if (crit && this == getPGame()->getPlayer())
        {
            attack *= 2;
            soundEffect("CriticalHit.wav", false, false);
        }
        else
        {
            if (getTypeId() == dngutil::TID::Player)
            {
                soundEffect("EnemyHit.wav", false, true);
            }
            else
            {
                soundEffect("PlayerHit.wav", false, true);
            }
        }
    }
    else
    {
        soundEffect("WeaponMiss.wav", false, true);
        canMiss = false;
    }

    if (deflect)
    {
        damage.damageDeflected = static_cast<int>(attack - defense) - static_cast<int>(attack - (defense * 1.3));
        if (damage.damageDeflected < 0)
        {
            damage.damageDeflected = 0;
        }
        damage.damage = static_cast<int>(attack - defense) - damage.damageDeflected;
    }
    else
    {
        damage.damage = static_cast<int>(attack - defense);
    }

    int min = random(2, 3);
    if (damage.damage < min)
    {
        damage.damage = min;
    }

    if (miss)
    {
        damage.damage = 0;
    }

    return damage;
}

void Creature::levelUpStats()
{
    int healthIncrease = random(4, 8);
    increaseMaxhp(healthIncrease);
    increaseHealth(healthIncrease);

    increaseAtt(random(3, 4));

    increaseDef(random(2, 4));

    increaseLck(random(2, 6));

    increaseSpd(random(4, 8));
}

dngutil::Movement Creature::getLastMovement()
{
    return lastMovement;
}
//------------------------------------------------------------