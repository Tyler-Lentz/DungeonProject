#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace dngutil
{
    enum colors
    {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE
    };

    enum class TID
    {
        Player,
        Potion
    };

    enum class Movement
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    const int CONSOLEX = 80;
    const int CONSOLEY = 45;
}

void errorMessage(std::string error, int line, std::string file);

int getColor(int letterColor, int backgroundColor, int numberOfColors = 16);

#endif