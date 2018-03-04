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
                clearLine(vcursor.y);
                vwin->putcen(newMap[newMap.size() - (j + 1)], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int j = 0; j < oldRoom->getRoomY() - (i + 1) && j < (newRoom->getRoomY() - (i + 1)); j++)
            {
                clearLine(vcursor.y);
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
                clearLine(vcursor.y);
                vwin->putcen(oldMap[j], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int j = 0; j < (i + 1) && j < newRoom->getRoomY(); j++)
            {
                clearLine(vcursor.y);
                vwin->putcen(newMap[j], vcursor.y);
                vcursor.x = 0; vcursor.y++;
            }
            for (int i = 0; i < abs(yDiff) && i < 10; i++)
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
        playSound(WavFile("Puzzle", false, true));
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

    vcursor.y = DIVIDER_LINES[2] - 2;
    if (game->shouldSpawnBeast())
    {
        vwin->putcen(ColorString("  Beast in ", dngutil::RED) + game->getPlayer()->getStepString() + ColorString(" steps...  ", dngutil::RED), vcursor.y++);
    }
    else
    {
        clearLine(vcursor.y++);
    }
    vwin->putcen(ColorString("VALID INPUT {ARROWKEYS, I, U, M}", dngutil::CYAN), vcursor.y);
}

dngutil::ReturnVal TextMacros::deathScreen(Game* game)
{ 
    vwin->clearScreen(dngutil::RED);

    playSound(WavFile("Death", false, false));
    playSound(Mp3File("Continue"));
    int startingLine = static_cast<int>(dngutil::CONSOLEY / 2.0) + 1;
    int textcolor = getColor(dngutil::WHITE, dngutil::RED);
    vwin->putcen(ColorString("Oh dear, you have died...", textcolor), startingLine++);
    startingLine++;
    vwin->putcen(ColorString("Press enter to continue", textcolor), startingLine);
    while (!keyrelease(VK_RETURN));

    while (!keypress(VK_RETURN));

    while (!keyrelease(VK_RETURN));
    dngutil::ReturnVal returnValue = dngutil::ReturnVal::RESTART;

    stopSound(SoundType::MP3);

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

    if (keypress(VK_RETURN))
    {
        left.setColor(dngutil::RED);
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

void TextMacros::displayInventory(std::vector<Item*> menu, Player* player)
{
    clearMapArea(false, NULL);
    Coordinate vcursor(0, DIVIDER_LINES[1] + 2);
    
    vwin->put(ColorString("   Gold: " + std::to_string(player->getGold()), dngutil::YELLOW), vcursor); vcursor.y++;
    // if top line changes change it in the player inventory function
    for (auto& i : menu)
    {
        vwin->put(ColorString("   " + i->getName() + " - ", dngutil::LIGHTGRAY)
            + i->getMapRep()
            + ColorString(" - " + i->getDescription(), dngutil::LIGHTGRAY), Coordinate(0, vcursor.y));
        vcursor.y++;
    }
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

void TextMacros::fallingScreen(Game* game, bool altSound)
{
    clearMapArea(false, NULL);
    playSound(WavFile("Fall", false, false));
    if (altSound)
    {
        Sleep(500);
        game->getOverworldMusic().stop();
        playSound(WavFile("WarpToDungeonStart", false, false));
    }
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

void VirtualWindow::put(ColorString colstr, Coordinate coord, bool scrolling)
{
    if (scrolling)
    {
        playSound(WavFile("LetterSound", true, true));
    }

    for (auto i : colstr)
    {
        put(i, coord);
        if (scrolling)
        {
            Sleep(dngutil::TEXT_SCROLL_TIME);
        }

        coord.x++;
        if (coord.x >= dngutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    if (scrolling)
    {
        stopSound(SoundType::WAV);
    }
}

void VirtualWindow::putcen(ColorChar colchar, unsigned int line)
{
    put(colchar, Coordinate(static_cast<int>((width - 1) / 2), line));
}

void VirtualWindow::putcen(ColorString colstr, unsigned int line, bool scrolling)
{
    Coordinate coord(static_cast<int>((width - colstr.size()) / 2), line);

    if (scrolling)
    {
        playSound(WavFile("LetterSound", true, true));
    }

    for (auto i : colstr)
    {
        put(i, coord);
        if (scrolling)
        {
            Sleep(dngutil::TEXT_SCROLL_TIME);
        }
        coord.x++;
        if (coord.x >= dngutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    if (scrolling)
    {
        stopSound(SoundType::WAV);
    }
}

void VirtualWindow::putcenSlowScroll(ColorString colstr, unsigned int line)
{
    Coordinate coord(static_cast<int>((width - colstr.size()) / 2), line);

    playSound(WavFile("LetterSound", true, true));

    for (auto i : colstr)
    {
        put(i, coord);
        Sleep(dngutil::TEXT_SCROLL_TIME * 2);
        coord.x++;
        if (coord.x >= dngutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    stopSound(SoundType::WAV);
}

ColorString VirtualWindow::getLine(unsigned int line) const
{
    return vwin[line];
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

void VirtualWindow::clearScreen(int color)
{
    ColorString line(std::string(dngutil::CONSOLEX, ' '), getColor(color, color));

    for (int i = 0; i < dngutil::CONSOLEY; i++)
    {
        putcen(line, i);
    }
}

//-------------------------------------------------------------