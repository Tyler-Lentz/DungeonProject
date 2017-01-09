#include "creature.h"
#include "mapobject.h"
#include "utilities.h"
#include "item.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "virtualwindow.h"
#include "room.h"

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
    size_t att,
    size_t def,
    size_t lck,
    size_t spd,
    size_t lvl,
    Primary* primary,
    Secondary* secondary,
    int priority
)
    :MapObject(pgame, mapRep, coord, name, moveable, rawoutput, aggressive, typeId, priority, dngutil::BTID::Creature)
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
}

// Save Constructor
Creature::Creature(const Creature& other, Game* game)
    :MapObject(other, game)
{
    this->primary = new Primary(*primary, game);
    this->secondary = new Secondary(*secondary, game);

    this->maxhp = other.hp;
    this->att = other.att;
    this->def = other.def;
    this->lck = other.lck;
    this->spd = other.spd;
    this->lvl = other.lvl;
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

const bool& Creature::isDead() const
{
    return (hp < 1);
}

const size_t& Creature::getMaxhp() const
{
    return maxhp;
}

const int& Creature::getHp() const
{
    return hp;
}

const size_t& Creature::getAtt() const
{
    return att;
}

const size_t& Creature::getDef() const
{
    return def;
}

const size_t& Creature::getLck() const
{
    return lck;
}

const size_t& Creature::getSpd() const
{
    return spd;
}

const size_t& Creature::getLvl() const
{
    return lvl;
}

const Primary& Creature::getPrimary() const
{
    return *primary;
}

const Secondary& Creature::getSecondary() const
{
    return *secondary;
}

const size_t& Creature::increaseHealth(size_t amount)
{
    size_t tempHealth = hp;
    hp += amount;
    if (hp > maxhp)
    {
        hp = maxhp;
    }
    return hp - tempHealth;
}

const size_t& Creature::decreaseHealth(size_t amount)
{
    hp -= amount;
    return amount;
}

const ColorString& Creature::getHealthBar() const
{
    const int MAXIMUM_CHARACTERS = static_cast<int>(dngutil::CONSOLEX / 2.0);

    double scaleFactor = (MAXIMUM_CHARACTERS / (double)maxhp);

    int numOfCircles = (int)(scaleFactor * hp);

    std::string temp = std::string(MAXIMUM_CHARACTERS - numOfCircles, '-');
    std::string healthbar = temp + (std::string((size_t)numOfCircles, '='));

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

    // TODO: play enemies battle music

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

                // TODO: implement getDamageDealt
                int damage;
                //damage = getDamageDealt(player, enemy, musicPlayer, textOutput, game_pointer);
                enemy->decreaseHealth(damage);

                vwin->putcen(ColorString("-" + std::to_string(damage), dngutil::GREEN), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);
                Sleep(300);

                if (enemy->isDead())
                {
                    // TODO: this musicPlayer->stopMp3();

                    // TODO: put death sound based on the string stored in enemy

                    enemy->deathSequence();

                    getPGame()->getActiveRoom()->setAll();
                    getPGame()->getVWin()->txtmacs.clearMapArea(true, 20);
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                    pressEnter(Coordinate(0, getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE), getPGame()->getVWin());
                    getPGame()->getVWin()->txtmacs.clearDivider("bottom");
                    // TODO: play put this in
                    // musicPlayer->startMp3("Overworld.mp3");
                    return true;
                }
                break;
            }

            if (enemyTimer >= enemyWeaponSpeed)
            {
                enemyTimer = 0;
                int damage;
                // TODO: put this in when getDamageDealt exists
                // damage = getDamageDealt(enemy, player, musicPlayer, textOutput, game_pointer);
                player->decreaseHealth(damage);

                vwin->putcen(ColorString("-" + std::to_string(damage), dngutil::RED), vwin->txtmacs.BOTTOM_DIVIDER_TEXT_LINE + 1);
                Sleep(300);

                // TODO: musicPlayer->soundEffect("PlayerHit.wav", false, true);
                if (player->isDead())
                {
                    // TODO: musicPlayer->stopMp3();
                    getPGame()->cleanup(getPGame()->getVWin()->txtmacs.deathScreen());
                    return false;
                }
                break;
            }

            if (keypress('I'))
            {
                // TODO: when inventory menu is implemented, put this in
                // getPGame()->inventoryMenu();
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

//------------------------------------------------------------