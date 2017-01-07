#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include "colorstring.h"
#include "coordinate.h"
#include "console.h"

#include <vector>
#include <list>

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

    const Console& getConsole() const;
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