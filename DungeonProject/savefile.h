#ifndef SAVEFILE_H
#define SAVEFILE_h

#include "utilities.h"
#include <string>

class Player;
class Game;
class Item;
class Primary;
class Secondary;

void xorFile();

void saveGame(Player* player, Game* game);

std::string getInventoryItemText(Item& i);

std::string getPrimarySaveText(Primary& p);

std::string getSecondarySaveText(Secondary& s);

void loadGame(Game* game);

Primary* getPrimaryFromSaveString(std::string, Game* game);
Secondary* getSecondaryFromSaveString(std::string, Game* game);
Item* getItemFromId(dngutil::TID, Game* game);
#endif