#ifndef SAVEFILE_H
#define SAVEFILE_h

#include <string>

class Player;
class Game;
class Item;
class Primary;
class Secondary;

void saveGame(Player* player, Game* game);

std::string getInventoryItemText(Item& i);

std::string getPrimarySaveText(Primary& p);

std::string getSecondarySaveText(Secondary& s);

void loadGame(Game* game);
#endif