#include "virtualwindow.h"
#include "colorstring.h"
#include "utilities.h"
#include "coordinate.h"
#include "console.h"
#include "room.h"
#include "game.h"
#include "creature.h"
#include "player.h"
#include "item.h"

#include <iostream>
#include <cmath>
#include <string>
#include <mutex>

//-------------------------------------------------------------
// Text Macro Functions

void TextMacros::drawDividers()
{
    ColorString dividerString(std::string(dngutil::CONSOLEX, DIVIDER_CHARACTER), dngutil::LIGHTGRAY);

    for (int i = 0; i < 4; i++)
    {
        vwin->put(dividerString, Coordinate(0, DIVIDER_LINES[i]));
    }
}

void TextMacros::clearMapArea(bool rolling, int sleepTime)
{ 
    ColorString emptyString(std::string(dngutil::CONSOLEX, ' '), dngutil::LIGHTGRAY);

    for (int i = DIVIDER_LINES[1] + 1; i < DIVIDER_LINES[2]; i++)
    {
        vwin->put(emptyString, Coordinate(0, i));
        if (rolling)
        {
            Sleep(sleepTime);
        }
    }
}

void TextMacros::screenScroll(dngutil::Movement direction, Room* oldRoom, Room* newRoom, Game* game)
{ 
    std::vector<ColorString> oldMap;
    std::vector<ColorString> newMap;

    int xDiff = (newRoom->getRoomX() - oldRoom->getRoomX());
    for (Coordinate coord(0, 0); coord.y != oldRoom->getRoomY(); coord.y++)
    {
        ColorString cstr;
        oldMap.push_back(cstr);
        for (coord.x = 0; coord.x != oldRoom->getRoomX(); coord.x++)
        {
            int color;
            if (oldRoom->getObjects(coord).back()->isRawoutput())
            {
                color = oldRoom->getObjects(coord).back()->getMapRep().color;
            }
            else if (oldRoom->getObjects(coord).back()->getMapRep().color != dngutil::BACKGROUND_COLOR)
            {
                color = getColor(oldRoom->getObjects(coord).back()->getMapRep().color, oldRoom->getRoomInfo().backColor);
            }
            else
            {
                color = getColor(oldRoom->getRoomInfo().backColor, oldRoom->getRoomInfo().backColor);
            }
            oldMap[coord.y] += ColorChar(oldRoom->getObjects(coord).back()->getMapRep().character, color);
        }
    }

    for (Coordinate coord(0, 0); coord.y != newRoom->getRoomY(); coord.y++)
    {
        ColorString cstr;
        newMap.push_back(cstr);
        for (coord.x = 0; coord.x != newRoom->getRoomX(); coord.x++)
        {
            int color;
            if (newRoom->getObjects(coord).back()->isRawoutput())
            {
                color = newRoom->getObjects(coord).back()->getMapRep().color;
            }
            else if (newRoom->getObjects(coord).back()->getMapRep().color != dngutil::BACKGROUND_COLOR)
            {
                color = getColor(newRoom->getObjects(coord).back()->getMapRep().color, newRoom->getRoomInfo().backColor);
            }
            else
            {
                color = getColor(newRoom->getRoomInfo().backColor, newRoom->getRoomInfo().backColor);
            }
            newMap[coord.y] += (ColorChar(newRoom->getObjects(coord).back()->getMapRep().character, color));
        }
    }

    int yDiff = newRoom->getRoomY() - oldRoom->getRoomY();
    Coordinate vcursor(0, DIVIDER_LINES[1] + 1);
    switch (direction)
    {
    case dngutil::Movement::UP:
        for (int i = 0; i < newRoom->getRoomY(); i++)
        {
            vcursor.x = 0; vcursor.y = DIVIDER_LINES[1] + 1;
            for (int j = i; j >= 0; j--)
            {
                vwin->putcen(newMap[newMap.size() - (j + 1)], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int j = 0; j < oldRoom->getRoomY() - (i + 1) && j < (newRoom->getRoomY() - (i + 1)); j++)
            {
                vwin->putcen(oldMap[j], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            if (yDiff < 0)
            {
                for (int i = abs(yDiff); i > 0; i--)
                {
                    clearLine(vcursor.y);
                    vcursor.y++;
                }
            }
            Sleep(dngutil::SCROLL_TIME);
        }
        break;
    case dngutil::Movement::DOWN:
        for (int i = 0; i < oldRoom->getRoomY() || i < newRoom->getRoomY(); i++)
        {
            vcursor.x = 0; vcursor.y = DIVIDER_LINES[1] + 1;
            for (int j = i + 1; j < oldRoom->getRoomY(); j++)
            {
                vwin->putcen(oldMap[j], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int j = 0; j < (i + 1) && j < newRoom->getRoomY(); j++)
            {
                vwin->putcen(newMap[j], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int i = abs(yDiff); i > 0; i--)
            {
                clearLine(vcursor.y++);
            }
            Sleep(dngutil::SCROLL_TIME);
        }
        break;
    case dngutil::Movement::LEFT: // WHO EVEN KNOWS WHATS GOING ON HERE I DONT
        for (int i = 0; i < newRoom->getRoomX(); i++)
        {
            vcursor.x = 0; vcursor.y = DIVIDER_LINES[1] + 1;
            for (int j = 0; j < newRoom->getRoomY() || j < oldRoom->getRoomY(); j++)
            {
                ColorString cstr;
                for (int k = 0; k < i + 1 && j < newRoom->getRoomY(); k++)
                {
                    cstr += (newMap[j][newMap[j].size() - (k + 1)]);
                }
                std::reverse(cstr.begin(), cstr.end());
                for (int k = 0; k < newRoom->getRoomX() - (i + 1) && j < oldRoom->getRoomY() && k < oldRoom->getRoomX(); k++)
                {
                    cstr += (oldMap[j][k]);
                }            
                vwin->txtmacs.clearLine(vcursor.y);
                vwin->putcen(cstr, vcursor.y);
                vcursor.x = 0, vcursor.y++;
            }
            Sleep(dngutil::SCROLL_TIME);
        }
        break;
    case dngutil::Movement::RIGHT:
        for (int i = 0; i < newRoom->getRoomX(); i++)
        {
            vcursor.x = 0; vcursor.y = DIVIDER_LINES[1] + 1;
            for (int j = 0; j < newRoom->getRoomY() || j < oldRoom->getRoomY(); j++)
            {
                ColorString cstr;
                for (int k = i + 1; k < newRoom->getRoomX() && j < oldRoom->getRoomY() && k < oldRoom->getRoomX(); k++)
                {
                    cstr += (oldMap[j][k]);
                }
                for (int k = 0; k < i + 1 && j < newRoom->getRoomY(); k++)
                {
                    cstr += (newMap[j][k]);
                }
                vwin->txtmacs.clearLine(vcursor.y);
                vwin->putcen(cstr, vcursor.y);
                vcursor.x = 0, vcursor.y++;
            }
            Sleep(dngutil::SCROLL_TIME);
        }
        break;
    }
    
    writeMapName(game);

    if (newRoom->hasPuzzle())
    {
        soundEffect("Puzzle.wav", false, true);
    }
}

void TextMacros::writeMapName(Game* game)
{
    ColorString name(game->getActiveRoom()->getRoomInfo().name, dngutil::BROWN);
    vwin->txtmacs.clearLine(DIVIDER_LINES[0] + 1);
    vwin->putcen(name, DIVIDER_LINES[0] + 1);
}

void TextMacros::clearDivider(std::string divider)
{ 
    if (divider != "top" && divider != "bottom")
    {
        errorMessage("Invalid string passed to clearDivider. String was " + divider, __LINE__, __FILE__);
    }

    if (divider == "top")
    {
        clearLine(DIVIDER_LINES[0] + 1);
    }
    else // must be bottom
    {

        for (int i = DIVIDER_LINES[2] + 1; i < DIVIDER_LINES[3]; i++)
        {
            clearLine(i);
        }
    }
}

void TextMacros::displayGame(Game* game)
{ 
    clearMapArea(false, NULL);
    drawDividers();
    game->getActiveRoom()->setAll();
    writeMapName(game);
    clearDivider("bottom");
    displayOverworldInfo(game);
}

void TextMacros::displayHealthBars(Creature* top, Creature* player)
{ 
    Coordinate vcursor(0, DIVIDER_LINES[2] - 6);

    vwin->putcen(ColorString(top->getName(), dngutil::LIGHTGRAY), vcursor.y);

    vwin->putcen(top->getHealthBar(), ++vcursor.y);

    vwin->putcen(ColorString(player->getName(), dngutil::LIGHTGRAY), ++vcursor.y);

    vwin->putcen(player->getHealthBar(), ++vcursor.y);

    player->printStats(0, ++vcursor.y);

    vwin->putcen(ColorString("ENTER - attack, I - inventory, SPACE - deflect", dngutil::WHITE), BOTTOM_DIVIDER_TEXT_LINE);
}

void TextMacros::displayOverworldInfo(Game* game)
{
    Coordinate vcursor(0, DIVIDER_LINES[2] + 1);
    vwin->putcen(game->getPlayer()->getHealthBar(), vcursor.y);

    vwin->putcen(game->getPlayer()->getExperienceBar(), ++vcursor.y);

    game->getPlayer()->printStats(0, ++vcursor.y);

    vcursor.y = DIVIDER_LINES[2] - 1;
    vwin->putcen(ColorString("VALID INPUT {ARROWKEYS, I, U}", dngutil::CYAN), vcursor.y);
}

dngutil::ReturnVal TextMacros::deathScreen()
{ 
    vwin->clearScreen();

    ColorString line(std::string(dngutil::CONSOLEX, ' '), getColor(dngutil::RED, dngutil::RED));

    for (int i = 0; i < dngutil::CONSOLEY; i++)
    {
        vwin->putcen(line, i);
    }

    soundEffect("Death.wav", false, false);
    startMp3("Continue.mp3");
    int startingLine = static_cast<int>(dngutil::CONSOLEY / 2.0) + 1;
    vwin->putcen(ColorString("Oh dear, you have died...", getColor(dngutil::WHITE, dngutil::RED)), startingLine);
    startingLine += 2;
    vwin->putcen(ColorString("Press enter to continue", getColor(dngutil::WHITE, dngutil::RED)), startingLine);
    while (!keyrelease(VK_RETURN));

    while (!keypress(VK_RETURN));

    while (!keyrelease(VK_RETURN));
    dngutil::ReturnVal returnValue = dngutil::ReturnVal::RESTART;

    stopMp3();

    vwin->clearScreen();

    return returnValue;
}

void TextMacros::outputBattleInfo(int leftTimer, int leftMaxTimer, int rightTimer, int rightMaxTimer)
{ 
    Coordinate vcursor(0, DIVIDER_LINES[2] + 1);

    int circlecolor = dngutil::CYAN;
    int xcolor = dngutil::LIGHTCYAN;
    int fullcolor = dngutil::LIGHTGREEN;

    int circles = leftMaxTimer - leftTimer;
    ColorString leftCircles(std::string(circles, 'o'), circlecolor);
    ColorString leftXs(std::string(leftTimer, 'x'), xcolor);
    ColorString left = leftCircles + leftXs;

    if (leftTimer == leftMaxTimer)
    {
        left.setColor(fullcolor);
    }

    int rcircles = rightMaxTimer - rightTimer;
    ColorString rightCircles(std::string(rcircles, 'o'), circlecolor);
    ColorString rightXs(std::string(rightTimer, 'x'), xcolor);
    ColorString right = rightXs + rightCircles;

    if (rightTimer == rightMaxTimer)
    {
        right.setColor(fullcolor);
    }

    ColorString info = left + ColorString(" | ", dngutil::LIGHTGRAY) + right;

    vwin->putcen(info, vcursor.y);

    vcursor.y = BOTTOM_DIVIDER_TEXT_LINE + 1;

    clearLine(vcursor.y);
}

void TextMacros::clearLine(unsigned int line)
{ 
    vwin->putcen(ColorString(std::string(dngutil::CONSOLEX, ' '), dngutil::LIGHTGRAY), line);
}

void TextMacros::displayInventory(int positions[], Player* player)
{
    clearMapArea(false, NULL);
    Coordinate vcursor(0, DIVIDER_LINES[1] + 3);

    positions[0] = vcursor.y;
    for (auto& i : player->getInventory())
    {
        vwin->put(ColorString("   " + i->getName() + " - ", dngutil::LIGHTGRAY)
            + i->getMapRep()
            + ColorString(" - " + i->getDescription(), dngutil::LIGHTGRAY), Coordinate(0, vcursor.y));
        vcursor.y++;
    }

    positions[1] = --vcursor.y;
}



void TextMacros::displayLevelupStats(Coordinate cursor, Player* player)
{
    vwin->putcen(ColorString("**************************", dngutil::MAGENTA), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("Max Health " + std::to_string(player->getMaxhp()), dngutil::RED), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("Attack " + std::to_string(player->getAtt()), dngutil::GREEN), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("Defense " + std::to_string(player->getDef()), dngutil::BLUE), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("Luck " + std::to_string(player->getLck()), dngutil::YELLOW), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("Speed " + std::to_string(player->getSpd()), dngutil::CYAN), cursor.y);
    cursor.y += 2;
    vwin->putcen(ColorString("**************************", dngutil::MAGENTA), cursor.y);
}

void TextMacros::fallingScreen(Game* game)
{
    clearMapArea(false, NULL);
    soundEffect("Fall.wav", false, false);
    displayGame(game);
}
//-------------------------------------------------------------

//-------------------------------------------------------------
// VirtualWindow functions

VirtualWindow::VirtualWindow(unsigned int width, unsigned int height):
    console(width, height), txtmacs(this)
{
    vwin.resize(height, ColorString(std::string(width, ' '), getColor(dngutil::LIGHTGRAY, dngutil::BLACK)));
    this->width = width;
    this->height = height;
}

void VirtualWindow::put(ColorChar colchar, Coordinate coord)
{
    refreshMut.lock();
    if (vwin[coord.y][coord.x] != colchar)
    {
        vwin[coord.y][coord.x] = colchar;
        posToDraw.push_back(coord);
    }
    refreshMut.unlock();
}

void VirtualWindow::put(ColorString colstr, Coordinate coord)
{
    for (auto i : colstr)
    {
        put(i, coord);

        coord.x++;
        if (coord.x >= dngutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }
}

void VirtualWindow::putcen(ColorChar colchar, unsigned int line)
{
    put(colchar, Coordinate(static_cast<int>((width - 1) / 2), line));
}

void VirtualWindow::putcen(ColorString colstr, unsigned int line)
{
    Coordinate coord(static_cast<int>((width - colstr.size()) / 2), line);
    for (auto i : colstr)
    {
        put(i, coord);
        coord.x++;
        if (coord.x >= dngutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }
}

void VirtualWindow::refresh()
{
    refreshMut.lock();

    for (auto i : posToDraw)
    {
       ColorChar charToDraw = vwin[i.y][i.x];
       console.setCursorPos(i);
       console.setColor(charToDraw.color);
       std::cout << charToDraw.character;
    }

    posToDraw.clear();
    console.setColor(dngutil::LIGHTGRAY);

    refreshMut.unlock();
}

const Console& VirtualWindow::getConsole() const
{
    return console;
}

void VirtualWindow::clearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    console.setColor(dngutil::LIGHTGRAY);

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);

    int height = vwin.size();
    int width = vwin[0].size();

    vwin.resize(height, ColorString(std::string(width, ' '), getColor(dngutil::LIGHTGRAY, dngutil::BLACK)));
}

//-------------------------------------------------------------