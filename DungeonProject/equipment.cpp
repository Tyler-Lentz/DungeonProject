#include "equipment.h"
#include "item.h"
#include "mapobject.h"
#include "game.h"
#include "player.h"
#include "virtualwindow.h"
#include "colorstring.h"

void Equipment::action(Player* player, unsigned int inventoryIndex)
{
    std::string output;

    if (esid == EquipSlotId::ARMOR)
    {
        player->getArmorMemory()->unequipAction();
        if (player->swapArmor(player->getInventoryNotConst()[inventoryIndex]))
        {
            output = "Swapped active armor.";
        }
        else
        {
            output = "An error occured trying to swap armor - please report this bug";
        }
        player->getArmorMemory()->equipAction();
    }
    else
    {
        player->getBootsMemory()->unequipAction();
        if (player->swapBoots(player->getInventoryNotConst()[inventoryIndex]))
        {
            output = "Swapped active boots.";
        }
        else
        {
            output = "An error occured trying to swap boots - please report this bug";
        }
        player->getBootsMemory()->equipAction();
    }
    

    getPGame()->getVWin()->putcen(ColorString(output, dngutil::LIGHTGRAY), getPGame()->getVWin()->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
}

EquipSlotId Equipment::getSlotId() const
{
    return esid;
}

void Equipment::equipAction()
{

}

void Equipment::unequipAction()
{

}

void Speedboots::equipAction()
{
    getPGame()->getPlayer()->setMaxSpeedMultiplier(getPGame()->getPlayer()->startingMaxSpeedMultiplier - 0.25);
}

void Speedboots::unequipAction()
{
    getPGame()->getPlayer()->setMaxSpeedMultiplier(getPGame()->getPlayer()->startingMaxSpeedMultiplier);
}

void BlueTunic::equipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::LIGHTCYAN));
}

void BlueTunic::unequipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::noArmorPlayerColor));
}

void HerosTunic::equipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::YELLOW));
    getPGame()->getPlayer()->increaseDef(3);
    getPGame()->getPlayer()->increaseMaxhp(10);
}

void HerosTunic::unequipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::noArmorPlayerColor));
    getPGame()->getPlayer()->setDef(getPGame()->getPlayer()->getDef() - 3);
    getPGame()->getPlayer()->setMaxhp(getPGame()->getPlayer()->getMaxhp() - 10);
    if (getPGame()->getPlayer()->getHp() > static_cast<int>(getPGame()->getPlayer()->getMaxhp()))
    {
        getPGame()->getPlayer()->setHp(getPGame()->getPlayer()->getMaxhp());
    }
}

void Bubblecharm::equipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::LIGHTGREEN));
}

void Bubblecharm::unequipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::noArmorPlayerColor));
}

void ReinforcedBoots::equipAction()
{
    getPGame()->getPlayer()->increaseDef(2);
}

void ReinforcedBoots::unequipAction()
{
    getPGame()->getPlayer()->setDef(getPGame()->getPlayer()->getDef() - 2);
}