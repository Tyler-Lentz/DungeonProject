#include <fstream>

#include "game.h"
#include "player.h"
#include "room.h"
#include "item.h"
#include "savefile.h"

void saveGame(Player* player, Game* game)
{
    std::ofstream file("save.dat", std::ios::trunc);

    Coordinate mapCoord(game->getActiveRoom()->getRoomX(), game->getActiveRoom()->getRoomY());

    file << player->getName() << std::endl
        << game->getRawFloor() << std::endl
        << mapCoord.x << std::endl
        << mapCoord.y << std::endl
        << player->getCoord().x << std::endl
        << player->getCoord().y << std::endl
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
        << getSecondarySaveText(*player->getSecondaryMemory()) << std::endl;

    for (auto& i : player->getInventoryNotConst())
    {
        file << getInventoryItemText(*i) << std::endl;
    }

    // obfusicate
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

    text += std::to_string(static_cast<int>(p.getTypeId())) + " ";
    text += std::to_string(static_cast<int>(p.getClass())) + " ";
    text += p.getName() + " ";
    text += p.getDescription() + " ";
    text += std::to_string(p.getMapRep().character) + " ";
    text += std::to_string(p.getMapRep().color) + " ";
    text += std::to_string(p.isRawoutput()) + " ";
    text += std::to_string(p.getDmgMultiplier()) + " ";
    text += std::to_string(p.getAttSpeed()) + " ";
    text += std::to_string(p.getAccuracy()) + " ";
    text += std::to_string(p.getStartReady()) + " ";
    text += p.getHitsound().getFilename();

    return text;
}

std::string getSecondarySaveText(Secondary& s)
{
    std::string text;

    text += std::to_string(static_cast<int>(s.getTypeId())) + " ";
    text += s.getName() + " ";
    text += s.getDescription() + " ";
    text += std::to_string(s.getMapRep().character) + " ";
    text += std::to_string(s.getMapRep().color) + " ";
    text += std::to_string(s.isRawoutput()) + " ";
    text += std::to_string(s.getDeflectTime()) + " ";
    text += std::to_string(s.getDefenseBoost()) + " ";

    return text;
}

void loadGame(Game* game)
{
    
}