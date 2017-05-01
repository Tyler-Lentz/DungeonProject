#include "utilities.h"
#include "mapobject.h"
#include "virtualwindow.h"
#include "game.h"
#include "item.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <list>

void errorMessage(std::string error, int line, std::string file)
{
    std::cerr << "Error on line " << line << " in the file " << file << "\n- " << error;
    Sleep(100000);
    exit(1);
}

int getColor(int letterColor, int backgroundColor, int numberOfColors)
{
    return (letterColor + (backgroundColor * numberOfColors));
}

int random(int max)
{
    if (max == -1)
    {
        max = 1;
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(0, max);
    return distro(generator);
}

int random(int min, int max)
{
    if (max < min)
    {
        std::swap(max, min);
    }
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

bool keypress(int key)
{
    if (!(GetAsyncKeyState(key) < 0)) // if its not pressed
    {
        return false;
    }

    if (GetConsoleWindow() != GetForegroundWindow())
    {
        return false;
    }
    return true;
}

bool keyrelease(int key)
{
    if (GetAsyncKeyState(key) < 0) // if the key is down
    {
        return false;
    }
    if (GetConsoleWindow() != GetForegroundWindow())
    {
        return false;
    }
    return true;
}

void sortPriority(std::list<MapObject*>& list, MapObject* objectToAdd)
{
    int p = objectToAdd->getPriority();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        if ((*it)->getPriority() > p)
        {
            list.insert(it, objectToAdd);
            return;
        }
    }

    list.push_back(objectToAdd);
}

void pressEnter(Coordinate cursorpos, VirtualWindow* vwin, int color)
{
    vwin->putcen(ColorString("Press enter to continue", color), cursorpos.y);

    while (!keyrelease(VK_RETURN));

    while (!keypress(VK_RETURN));

    while (!keyrelease(VK_RETURN));
}

int getExpToLevel(unsigned int level)
{
    return static_cast<int>(((0.5 * (level * level)) + 50)* 1.25);
}



int getHealthbarSleepTime(int damageDealt)
{
    return (200 / damageDealt);
}

void credits(dngutil::CreditType c, Game* pgame)
{
    VirtualWindow* v = pgame->getVWin();

    Coordinate vcursor(0, 5);

    int color;
    int color2;
    int sleepTime;
    int clearScreenColor;
    if (c == dngutil::CreditType::TITLESCREEN)
    {
        v->clearScreen(dngutil::BLACK);

        color = dngutil::WHITE;
        color2 = dngutil::GREEN;
        sleepTime = 200;
        clearScreenColor = dngutil::BLACK;
    }
    else
    {
        v->clearScreen(dngutil::WHITE);
        color = getColor(dngutil::BLACK, dngutil::WHITE);
        color2 = getColor(dngutil::GREEN, dngutil::WHITE);
        sleepTime = 350;
        clearScreenColor = dngutil::WHITE;
    }

    std::vector<ColorString> credits;

    auto addEmptySpace = [&](int amount) {
        for (int i = 0; i < amount; i++)
        {
            credits.push_back(ColorString("", color));
        }
    };

    addEmptySpace(35);
    credits.push_back(ColorString("Harp of the Gods", color2));
    addEmptySpace(20);
    credits.push_back(ColorString("Programming: Tyler Lentz", color));
    addEmptySpace(8);
    credits.push_back(ColorString("Story: Tyler Lentz", color));
    addEmptySpace(8);
    credits.push_back(ColorString("Playtesting: Tyler Lentz, Thomas Westenhofer, Collin Werth,", color));
    credits.push_back(ColorString("Kristian Rascon and Danny Hernandez", color));
    addEmptySpace(8);
    credits.push_back(ColorString("Music: The Legend of Zelda 1, The Legend of Zelda 2, ", color));
    credits.push_back(ColorString("Link to the Past, Link's Awakening and Olcarina of Time", color));
    addEmptySpace(32);
    if (c == dngutil::CreditType::VICTORY)
    {
        credits.push_back(ColorString("Peace has been restored to Bora", color2));
        credits.push_back(ColorString("Your quest is over...", color2));
        addEmptySpace(16);
    }        
    credits.push_back(ColorString("Press enter to continue", color2));
    addEmptySpace(16);

    for (unsigned int i = 0; i < credits.size() - 40; i++)
    {
        Sleep(sleepTime);
        v->clearScreen(clearScreenColor);
        for (unsigned int j = 0; j < 40; j++)
        {
            v->putcen(credits[i + j], j);
        }
        if (keypress(VK_RETURN))
        {
            goto skipCredits;
        }
    }

    while (!keypress(VK_RETURN));
    skipCredits:

    if (c != dngutil::CreditType::TITLESCREEN)
    {
        stopSound(SoundType::MP3);
    }

    v->clearScreen();


    pgame->cleanup(dngutil::ReturnVal::RESTART);
}

void intro(VirtualWindow* vwin)
{
    playSound(Mp3File("Intro"));
    bool keepGoing = true;
    for (int i = 0; i < 600; i++)
    {
        Sleep(10);
        if (keypress(VK_RETURN))
        {
            keepGoing = false;
            break;
        }
    }

    if (keepGoing)
    {
        int timesThrough = 0;
        for (int i = dngutil::CONSOLEY - 1; i > 2; i--)
        {
            Coordinate vcursor(5, i);
            VirtualWindow* t = vwin;
            t->clearScreen();
            int color = dngutil::YELLOW;
            int scolor = dngutil::WHITE;
            t->put(ColorString(R"(   __ __                     ___  __  __         _____        __  )", color), vcursor); vcursor.y++;
            if (timesThrough > 1)
            {
                t->put(ColorString(R"(  / // /__ ________    ___  / _/ / /_/ /  ___   / ___/__  ___/ /__)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 2)
            {
                t->put(ColorString(R"( / _  / _ `/ __/ _ \  / _ \/ _/ / __/ _ \/ -_) / (_ / _ \/ _  (_-<)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 3)
            {
                t->put(ColorString(R"(/_//_/\_,_/_/ / .__/  \___/_/   \__/_//_/\__/  \___/\___/\_,_/___/)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 4)
            {
                t->put(ColorString(R"(             /_/                                                  )", color), vcursor); vcursor.y++;
            }
            
            timesThrough++;
            
            if (timesThrough < 4)
            {
                Sleep(150);
            }
            else
            {   
                Sleep(125);
            }
            if (keypress(VK_RETURN))
            {
                keepGoing = false;
                break;
            }
        }
    }

    if (keepGoing)
    {
        for (int i = 0; i < 500; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }
    }

    stopSound(SoundType::MP3);

    vwin->putcen(ColorString("A game by Tyler Lentz", dngutil::WHITE), 35);
    if (keepGoing)
    {
        for (int i = 0; i < 200; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }
    }
    
    vwin->clearScreen();
    Sleep(333);
}


void statIncreaseDisplay(int change, int prevValue, int color, std::string text, VirtualWindow* vwin, Coordinate& vcursor)
{
    playSound(WavFile("Experience", true, true));
    vwin->putcen(ColorString("*---------------------------------*", color), vcursor.y - 1);
    vwin->putcen(ColorString("*---------------------------------*", color), vcursor.y + 1);
    for (int i = 0; i <= change; i++)
    {
        vwin->putcen(ColorString(text + std::to_string(prevValue + i), color), vcursor.y);
        if (!keypress(VK_RETURN))
        {
            Sleep(250);
        }
    }
    stopSound(SoundType::WAV);

    vcursor.y += 3;

    if (!keypress(VK_RETURN))
    {
        playSound(WavFile("PickupItem", false, false));
        Sleep(350);
    }
}