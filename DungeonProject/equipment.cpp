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
    getPGame()->getPlayer()->increaseSpd(50);
}

void Speedboots::unequipAction()
{
    getPGame()->getPlayer()->setSpd(getPGame()->getPlayer()->getSpd() - 50);
}

void BlueTunic::equipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::LIGHTCYAN));
}

void BlueTunic::unequipAction()
{
    getPGame()->getPlayer()->changeMapChar(ColorChar('A', dngutil::noArmorPlayerColor));
}