#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "item.h"
#include "mapobject.h"
#include "colorstring.h"
#include "coordinate.h"
#include "utilities.h"
#include <string>

enum class EquipSlotId
{
    ARMOR, BOOTS
};

class Equipment : public RItem
{
public:
    Equipment(
        Game* pgame,
        ColorChar mapRep,
        Coordinate coord,
        std::string name,
        bool rawoutput,
        std::string description,
        EquipSlotId esid,
        dngutil::TID tid
    ) :RItem(pgame, mapRep, coord, name, true, rawoutput, false, tid, false, description)
    {
        this->esid = esid;
    }

    virtual void equipAction();
    virtual void unequipAction();
    void action(Player* player, unsigned int inventoryIndex) override;

    EquipSlotId getSlotId() const;
private:
    EquipSlotId esid;
};

class BlueTunic : public Equipment
{
public:
    BlueTunic(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('0', dngutil::LIGHTCYAN),
        coord,
        "Blue Tunic",
        false,
        "A standard, blue tunic",
        EquipSlotId::ARMOR,
        dngutil::TID::BlueTunic
    )
    {

    }

private:
    void equipAction() override;
    void unequipAction() override;
};

class StandardBoots : public Equipment
{
public:
    StandardBoots(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('*', dngutil::BLACK),
        coord,
        "Standard Boots",
        false,
        "Regular old boots",
        EquipSlotId::BOOTS,
        dngutil::TID::StandardBoots
    )
    {

    }
};

class Waterboots : public Equipment
{
public:
    Waterboots(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('*', dngutil::BLUE),
        coord,
        "Waterboots",
        false,
        "If worn, the user can walk on shallow water",
        EquipSlotId::BOOTS,
        dngutil::TID::Waterboots
    )
    {

    }
};

class Speedboots : public Equipment
{
public:
    Speedboots(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('*', dngutil::YELLOW),
        coord,
        "Speedboots",
        false,
        "Increases speed greatly when worn",
        EquipSlotId::BOOTS,
        dngutil::TID::Speedboots
    )
    {

    }

private:
    void equipAction() override;
    void unequipAction() override;
};

class HerosTunic : public Equipment
{
public:
    HerosTunic(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('0', dngutil::YELLOW),
        coord,
        "Hero's Tunic",
        false,
        "Tunic passed down from the hero himself",
        EquipSlotId::ARMOR,
        dngutil::TID::HerosTunic
    )
    {

    }

private:
    void equipAction() override;
    void unequipAction() override;
};

class Bubblecharm : public Equipment
{
public:
    Bubblecharm(
        Game* pgame,
        Coordinate coord
    ) :Equipment(
        pgame,
        ColorChar('o', dngutil::LIGHTCYAN),
        coord,
        "Bubble Charm",
        false,
        "Allows the user to dive in certain water",
        EquipSlotId::ARMOR,
        dngutil::TID::Bubblecharm
    )
    {

    }

private:
    void equipAction() override;
    void unequipAction() override;
};
#endif