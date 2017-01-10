#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include "colorstring.h"
#include "coordinate.h"
#include "console.h"
#include "utilities.h"

#include <vector>
#include <list>

class Creature;
class Room;
class VirtualWindow;
class Game;
class Player;

struct TextMacros
{
    TextMacros(VirtualWindow* vwin)
    {
        this->vwin = vwin;
    }

    VirtualWindow* vwin;

    const int DIVIDER_LINES[4] = { 0, 2, dngutil::CONSOLEX - 7, dngutil::CONSOLEY - 2 };
    const char DIVIDER_CHARACTER = '-';

    const int POSITION_FOR_TOP_DIVIDER_TEXT = 0 + 1;
    const int POSITION_FOR_BOTTOM_DIVIDER_TEXT = dngutil::CONSOLEY - 4;

    const int BOTTOM_DIVIDER_TEXT_LINE = DIVIDER_LINES[2] + 2;

    // Draws the textboxes at the top and bottom of the screen
    void drawDividers();

    // Clears the area between the dividers. If rolling equals true, than the program
    // will sleep for sleepTime miliseconds after each line is cleared
    void clearMapArea(bool rolling, int sleepTime);

    // Screen scrolling effect when moving between rooms
    void screenScroll(dngutil::Movement direction, Room* oldG, Room* newG, Game* game);

    // Clears a divider, the string can either be "top" or "bottom"
    void clearDivider(std::string divider);

    // Does all neccessary actions needed to redraw the map after coming back from
    // a menu or battle
    void displayGame(Game* game);

    // outputs the health bars for two creatures
    void displayHealthBars(Creature* top, Creature* player);

    // displays information the game displays while the player is on the map
    void displayOverworldInfo(Game* game);

    // screen when the player dies
    dngutil::ReturnVal deathScreen();

    // outputs information displayed at the bottom divider during a battle
    void outputBattleInfo(int leftTimer, int leftMaxTimer, int rightTimer, int rightMaxTimer);

    // clears the line specified
    void clearLine(size_t line);

    // Writes the map name to the top of the screen
    void writeMapName(Game* game);

    // Displays the players stats when leveling up
    void displayLevelupStats(Coordinate cursor, Player* player);

};

typedef std::vector<ColorString> VWIN;

class VirtualWindow
{
public:
    // Sets the VWIN to the size of width,height
    VirtualWindow(size_t width, size_t height);

    // Outputs the updated information to the console
    void refresh();

    // Puts the char/string to the specified location in the VWIN, for strings
    // it is the starting location
    void put(ColorChar colchar, Coordinate coord);
    void put(ColorString colstr, Coordinate coord);
    // These center the string/char on the specified line
    void putcen(ColorChar colchar, size_t line);
    void putcen(ColorString colstr, size_t line);

    // clears the entire screen
    void clearScreen();

    const Console& getConsole() const;

    TextMacros txtmacs;
private:
    // Vector that holds the updated information to draw to the console
    VWIN vwin;

    Console console;

    // List that holds all of the updated positions in the virtual window
    std::list<Coordinate> posToDraw;

    // width and height of the virtual window
    size_t width;
    size_t height;
};

#endif