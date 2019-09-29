#ifndef SAVEFILE_H
#define SAVEFILE_h

#include "utilities.h"
#include <string>

class Player;
class Game;
class Item;
class Primary;
class Secondary;
class Spellbook;

void xorFile();

void saveGame(Player* player, Game* game);

std::string getInventoryItemText(Item& i);

std::string getPrimarySaveText(Primary& p);

std::string getSecondarySaveText(Secondary& s);

std::string getSpellbookSaveText(Spellbook& s);

bool loadGame(Game* game);

Primary* getPrimaryFromSaveString(std::string, Game* game, bool saving);
Secondary* getSecondaryFromSaveString(std::string, Game* game, bool saving);
Item* getItemFromId(dngutil::TID, Game* game, bool saving);
Spellbook* getSpellbookFromSaveString(std::string, Game* game, bool saving);
#endif