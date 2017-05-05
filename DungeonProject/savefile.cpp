#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iterator>

#include "game.h"
#include "player.h"
#include "room.h"
#include "item.h"
#include "savefile.h"
#include "equipment.h"
#include "utilities.h"
void xorFile()
{
    std::fstream file("save.txt", std::ios::in);

    std::string str;
    std::getline(file, str, std::string::traits_type::to_char_type(
        std::string::traits_type::eof()));

    file.close();

    int key = 30;
    for (auto& i : str)
    {
        i ^= key;
    }

    file.open("save.txt", std::ios::out | std::ios::trunc);

    file << str;

    file.close();
}

void saveGame(Player* player, Game* game)
{
    std::ofstream file("save.txt", std::ios::trunc);

    Coordinate mapCoord(game->getActiveRoom()->getRoomInfo().mapCoord);

    player->getArmorMemory()->unequipAction();
    player->getBootsMemory()->unequipAction();

    file << player->getName() << std::endl
        << game->getRawFloor() << std::endl
        << player->getGold() << std::endl
        << player->getStartingDungeonRoomCoord().x << std::endl
        << player->getStartingDungeonRoomCoord().y << std::endl
        << player->getStartingDungeonMapCoord().x << std::endl
        << player->getStartingDungeonMapCoord().y << std::endl
        << mapCoord.x << std::endl
        << mapCoord.y << std::endl
        << player->getCoord().x << std::endl
        << player->getCoord().y << std::endl
        << std::to_string(player->hasHarpPiece(1)) << std::endl
        << std::to_string(player->hasHarpPiece(2)) << std::endl
        << std::to_string(player->hasHarpPiece(3)) << std::endl
        << std::to_string(player->hasHarpPiece(4)) << std::endl
        << player->getMaxhp() << std::endl
        << player->getHp() << std::endl
        << player->getAtt() << std::endl
        << player->getDef() << std::endl
        << player->getLck() << std::endl
        << player->getSpd() << std::endl
        << player->getLvl() << std::endl
        << player->getExp() << std::endl
        << player->getPlayersExpToLevel() << std::endl
        << static_cast<int>(player->getClass()) << std::endl
        << getPrimarySaveText(*player->getPrimaryMemory()) << std::endl
        << getSecondarySaveText(*player->getSecondaryMemory()) << std::endl
        << getInventoryItemText(*player->getArmorMemory()) << std::endl
        << getInventoryItemText(*player->getBootsMemory()) << std::endl;

    for (auto& i : player->getInventoryNotConst())
    {
        file << getInventoryItemText(*i) << std::endl;
    }

    file << "DONE";

    player->getArmorMemory()->equipAction();
    player->getBootsMemory()->equipAction();

    file.close();
    xorFile();

    file.open("backupsave.txt", std::ios::out | std::ios::trunc);
    std::ifstream in("save.txt");
    std::string str;
    std::getline(in, str, std::string::traits_type::to_char_type(
        std::string::traits_type::eof()));
    in.close();
    file << str;
    file.close();
}

std::string getInventoryItemText(Item& i)
{
    switch (i.getTypeId())
    {
    case dngutil::TID::Primary: return getPrimarySaveText(dynamic_cast<Primary&>(i));
    case dngutil::TID::Secondary: return getSecondarySaveText(dynamic_cast<Secondary&>(i));
    default: return std::to_string(static_cast<int>(i.getTypeId()));
    }
}

std::string getPrimarySaveText(Primary& p)
{
    std::string text;

    text += std::to_string(static_cast<int>(p.getTypeId())) + "~";
    text += std::to_string(static_cast<int>(p.getClass())) + "~";
    text += p.getName() + "~";
    text += p.getDescription() + "~";
    text += std::to_string(p.getMapRep().character) + "~";
    text += std::to_string(p.getMapRep().color) + "~";
    text += std::to_string(p.isRawoutput()) + "~";
    text += std::to_string(p.getDmgMultiplier()) + "~";
    text += std::to_string(p.getAttSpeed()) + "~";
    text += std::to_string(p.getAccuracy()) + "~";
    text += std::to_string(p.getStartReady()) + "~";
    text += p.getHitsound().getFilename();

    return text;
}

std::string getSecondarySaveText(Secondary& s)
{
    std::string text;

    text += std::to_string(static_cast<int>(s.getTypeId())) + "~";
    text += s.getName() + "~";
    text += s.getDescription() + "~";
    text += std::to_string(s.getMapRep().character) + "~";
    text += std::to_string(s.getMapRep().color) + "~";
    text += std::to_string(s.isRawoutput()) + "~";
    text += std::to_string(s.getDeflectTime()) + "~";
    text += std::to_string(s.getDefenseBoost()) + "~";

    return text;
}

bool loadGame(Game* game)
{
    xorFile();
    Player* p = game->getPlayer();
    std::ifstream file("save.txt");
    std::string s;

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return false;
    }
    
    std::getline(file, s);
    p->setName(s);

    std::getline(file, s);
    game->setActiveFloor(stoi(s));

    std::getline(file, s);
    p->setGold(stoi(s));

    Coordinate coord;
    Coordinate coord2;
    std::getline(file, s);
    coord.x = stoi(s);
    std::getline(file, s);
    coord.y = stoi(s);
    std::getline(file, s);
    coord2.x = stoi(s);
    std::getline(file, s);
    coord2.y = stoi(s);
    game->getPlayer()->setDungeonStart(coord, coord2);

    std::getline(file, s);
    coord.x = stoi(s);
    std::getline(file, s);
    coord.y = stoi(s);
    game->setActiveRoomFromLoadSave(coord);

    std::getline(file, s);
    coord.x = stoi(s);
    std::getline(file, s);
    coord.y = stoi(s);
    p->setPosition(coord);

    std::getline(file, s);
    p->setHarp(1, static_cast<bool>(stoi(s)));

    std::getline(file, s);
    p->setHarp(2, static_cast<bool>(stoi(s)));

    std::getline(file, s);
    p->setHarp(3, static_cast<bool>(stoi(s)));

    std::getline(file, s);
    p->setHarp(4, static_cast<bool>(stoi(s)));

    std::getline(file, s);
    p->setMaxhp(stoi(s));

    std::getline(file, s);
    p->setHp(stoi(s));

    std::getline(file, s);
    p->setAtt(stoi(s));

    std::getline(file, s);
    p->setDef(stoi(s));

    std::getline(file, s);
    p->setLck(stoi(s));

    std::getline(file, s);
    p->setSpd(stoi(s));

    std::getline(file, s);
    p->setLvl(stoi(s));

    std::getline(file, s);
    p->setExp(stoi(s));

    std::getline(file, s);
    p->setExpToLevel(stoi(s));

    std::getline(file, s);
    p->setClass(static_cast<dngutil::ClassType>(stoi(s)));

    std::getline(file, s);
    delete p->getPrimaryMemory();
    p->setPrimary(getPrimaryFromSaveString(s, game,true));

    std::getline(file, s);
    delete p->getSecondaryMemory();
    p->setSecondary(getSecondaryFromSaveString(s, game,true));

    std::getline(file, s);
    p->getArmorMemory()->unequipAction();
    delete p->getArmorMemory();
    p->setArmor(dynamic_cast<Equipment*>(getItemFromId(static_cast<dngutil::TID>(stoi(s)), game,true)));
    p->getArmorMemory()->equipAction();

    std::getline(file, s);
    p->getBootsMemory()->unequipAction();
    delete p->getBootsMemory();
    p->setBoots(dynamic_cast<Equipment*>(getItemFromId(static_cast<dngutil::TID>(stoi(s)), game, true)));
    p->getBootsMemory()->equipAction();

    for (auto& i : p->getInventoryNotConst())
    {
        delete i;
    }
    p->getInventoryNotConst().resize(0);

    std::getline(file, s);
    while (s != "DONE")
    {
        std::stringstream ss(s);
        std::string buff;
        ss >> buff; // get first token
        dngutil::TID type = static_cast<dngutil::TID>(stoi(buff));
        if (type == dngutil::TID::Primary)
        {
            p->addToInventory(getPrimaryFromSaveString(s, game, true));
        }
        else if (type == dngutil::TID::Secondary)
        {
            p->addToInventory(getSecondaryFromSaveString(s, game, true));
        }
        else
        {
            p->addToInventory(getItemFromId(type, game, true));
        }

        std::getline(file, s);
    }

    file.close();
    xorFile();
    return true;
}

Primary* getPrimaryFromSaveString(std::string str, Game* game, bool saving)
{
    std::stringstream ss(str);
    std::string buff;
    std::vector<std::string> tokens;

    while (std::getline(ss, buff, '~')) 
    {
        tokens.push_back(buff);
    }
    // size should be 12 since its primary
    if (tokens.size() != 12)
    {
        errorMessage("tokens.size() was not 12, it was " + tokens.size(), __LINE__, __FILE__);
    }

    // if this changes change it in the plan text file too
    // id-classtype-name-description-char-color-rawoutput-dmgMultiplier-attSpeed-accuracy-startReady-hitsound
    // 0    1         2    3          4     5       6           7           8        9         10        11
    return new Primary(
        game,
        ColorChar(stoi(tokens[4]), stoi(tokens[5])), // the extra [0] is so it takes the first character of the string (it will only be 1 character)
        Coordinate(-1, -1), // already in inventory so doesnt need coord
        tokens[2],
        static_cast<bool>(stoi(tokens[6])),
        stod(tokens[7]),
        stoi(tokens[8]),
        stoi(tokens[9]),
        static_cast<bool>(stoi(tokens[10])),
        tokens[3],
        WavFile(tokens[11], false, false),
        static_cast<dngutil::ClassType>(stoi(tokens[1]))
    );
}

Secondary* getSecondaryFromSaveString(std::string str, Game* game, bool saving)
{
    std::stringstream ss(str);
    std::string buff;
    std::vector<std::string> tokens;
    while (std::getline(ss, buff, '~'))
    {
        tokens.push_back(buff);
    }
    // size should be 8 since its secondary
    if (tokens.size() != 8)
    {
        errorMessage("tokens.size() was not 8, it was " + tokens.size(), __LINE__, __FILE__);
    }

    // if this changes change it in the plan text file too
    // id - name - description - char - color - rawoutput - deflectTime - defense
    // 0    1         2           3       4       5           6             7
    return new Secondary(
        game,
        ColorChar(stoi(tokens[3]), stoi(tokens[4])),
        Coordinate(-1, -1),
        tokens[1],
        static_cast<bool>(stoi(tokens[5])),
        dngutil::TID::Secondary,
        stoi(tokens[6]),
        stoi(tokens[7]),
        tokens[2]
    );
}

Item* getItemFromId(dngutil::TID tid, Game* game, bool saving)
{
    switch (tid)
    {
    case dngutil::TID::Flute: return new Flute(game, Coordinate(-1, -1));
    case dngutil::TID::HerosClaim: return new HerosClaim(game, Coordinate(-1, -1));
    case dngutil::TID::Key: return new Key(game, Coordinate(-1, -1));
    case dngutil::TID::MagicalPotion: return new MagicalPotion(game, Coordinate(-1, -1));
    case dngutil::TID::Potion: return new Potion(game, Coordinate(-1, -1), dngutil::POTION_HEAL);
    case dngutil::TID::Waterboots: return new Waterboots(game, Coordinate(-1, -1));
    case dngutil::TID::Speedboots: return new Speedboots(game, Coordinate(-1, -1));
    case dngutil::TID::BlueTunic: return new BlueTunic(game, Coordinate(-1, -1));
    case dngutil::TID::StandardBoots: return new StandardBoots(game, Coordinate(-1, -1));
    case dngutil::TID::HerosTunic: return new HerosTunic(game, Coordinate(-1, -1));
    case dngutil::TID::HerosBlade: return new HerosBlade(game, Coordinate(-1, -1));
    case dngutil::TID::Bubblecharm: return new Bubblecharm(game, Coordinate(-1, -1));
    case dngutil::TID::ReinforcedBoots: return new ReinforcedBoots(game, Coordinate(-1, -1));
    case dngutil::TID::PowerBoots: return new PowerBoots(game, Coordinate(-1, -1));
    case dngutil::TID::BasiliskHorn: return new BasiliskHorn(game, Coordinate(-1, -1));
    }

    if (saving)
    {
        xorFile();
    }
    errorMessage("invalid tid passed to getItemFromId()", __LINE__, __FILE__);

    // so i stop getting a warning
    return nullptr;
}