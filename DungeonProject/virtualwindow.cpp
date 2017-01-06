#include "virtualwindow.h"
#include "colorstring.h"
#include "utilities.h"
#include "coordinate.h"
#include "console.h"

#include <iostream>
#include <cmath>
#include <string>

//-------------------------------------------------------------
// VirtualWindow functions

VirtualWindow::VirtualWindow(size_t width, size_t height)
    :console(width, height)
{
    vwin.resize(height, ColorString(std::string(width, ' '), getColor(dngutil::LIGHTGRAY, dngutil::BLACK)));
}

void VirtualWindow::put(ColorChar colchar, Coordinate coord)
{
    if (vwin[coord.x][coord.y] != colchar)
    {
        vwin[coord.x][coord.y] = colchar;
        posToDraw.push_back(coord);
    }
}

void VirtualWindow::put(ColorString colstr, Coordinate coord)
{
    for (auto it = colstr.begin(); it != colstr.end(); it++)
    {
        put(*it, coord);
        coord.moveRight(width);
    }
}

void VirtualWindow::putcen(ColorChar colchar, size_t line)
{
    put(colchar, Coordinate(floor((width - 1) / 2), line));
}

void VirtualWindow::putcen(ColorString colstr, size_t line)
{
    Coordinate coord(floor((width - colstr.size()) / 2), line);
    for (auto it = colstr.begin(); it != colstr.end(); it++)
    {
        put(*it, coord);
        coord.moveRight(width);
    }
}

void VirtualWindow::refresh()
{
    for (auto it = posToDraw.begin(); it != posToDraw.end(); it++)
    {
        ColorChar charToDraw = vwin[it->x][it->y];
       console.setCursorPos(*it);
       console.setColor(charToDraw.color);
       std::cout << charToDraw.character;
    }

    posToDraw.clear();
    console.setColor(dngutil::LIGHTGRAY);
}

//-------------------------------------------------------------