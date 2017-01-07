#ifndef CONSOLE_H
#define CONSOLE_H

#include "coordinate.h"

#include <Windows.h>
#include <stdexcept>

struct Console
{
    Console(unsigned width, unsigned height)
    {
        SMALL_RECT r;
        COORD      c;
        hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

        r.Left =
            r.Top = 0;
        r.Right = width - 1;
        r.Bottom = height - 1;
        SetConsoleWindowInfo(hConOut, TRUE, &r);

        c.X = width;
        c.Y = height;
        SetConsoleScreenBufferSize(hConOut, c);

        CONSOLE_CURSOR_INFO     cursorInfo;

        GetConsoleCursorInfo(hConOut, &cursorInfo);
        cursorInfo.bVisible = false;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(hConOut, &cursorInfo);
    }

    ~Console()
    {
        SetConsoleTextAttribute(hConOut, csbi.wAttributes);
        SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
        SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
    }

    void setTitle(std::string title)
    {
        SetConsoleTitle(title.c_str());
    }

    void setColor(int color)
    {
        SetConsoleTextAttribute(hConOut, color);
    }

    void setCursorPos(Coordinate coord)
    {
        COORD cord = {
            static_cast<SHORT>(coord.x),
            static_cast<SHORT>(coord.y)
        };
        SetConsoleCursorPosition(hConOut, cord);
    }

    HANDLE                     hConOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
};

#endif