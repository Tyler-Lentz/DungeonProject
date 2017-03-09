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
    return static_cast<int>((0.5 * (level * level)) + 50);
}



int getHealthbarSleepTime(int damageDealt)
{
    return (200 / damageDealt);
}

void credits(dngutil::CreditType c, Game* pgame, dngutil::DungeonType dungeon)
{
    VirtualWindow* v = pgame->getVWin();

    if (c == dngutil::CreditType::SECRET_VICTORY)
    {
        v->clearScreen(dngutil::LIGHTBLUE);
    }
    else if (c == dngutil::CreditType::VICTORY)
    {
        v->clearScreen(dngutil::WHITE);
    }

    if (c != dngutil::CreditType::TITLESCREEN)
    {
        Sleep(4000);
        playSound(Mp3File("Win"));
        Sleep(5000);
        stopSound(SoundType::MP3);

        Sleep(100);
        if (c == dngutil::CreditType::VICTORY)
        {
            playSound(Mp3File("Ending"));
        }
        else
        {
            playSound(Mp3File("alternate"));
        }

        pgame->adjustScore(dngutil::BASE_SCORE_VICTORY);
    }

    int color = 0;
    if (c == dngutil::CreditType::SECRET_VICTORY)
    {
        color = getColor(dngutil::WHITE, dngutil::LIGHTBLUE);
    }
    else if (c == dngutil::CreditType::VICTORY)
    {
        color = getColor(dngutil::BLACK, dngutil::WHITE);
    }
    else
    {
        color = getColor(dngutil::WHITE, dngutil::BLACK);

    }
    Coordinate vcursor(0, 5);
    if (c == dngutil::CreditType::TITLESCREEN)
    {
        v->txtmacs.clearMapArea(false, NULL);
    }

    if (dungeon == dngutil::DungeonType::DRAGONS_LAIR)
    {
        v->putcen(ColorString("DUNGEON RPG - DRAGON'S LAIR", color), vcursor.y++);
        if (c == dngutil::CreditType::SECRET_VICTORY)
        {
            v->putcen(ColorString("You avenge your village and", color), vcursor.y++);
            v->putcen(ColorString("you have vanquished all evil from the dungeon!", color), vcursor.y++);
        }
        else if (c == dngutil::CreditType::VICTORY)
        {
            v->putcen(ColorString("You avenge your village but", color), vcursor.y++);
            v->putcen(ColorString("a dark energy still resides in the dungeon...", color), vcursor.y++);
        }
    }
    else if (dungeon == dngutil::DungeonType::GRYPHONS_TOWER)
    {
        v->putcen(ColorString("DUNGEON RPG - GRYPHON'S TOWER", color), vcursor.y++);
        if (c == dngutil::CreditType::SECRET_VICTORY)
        {
            v->putcen(ColorString("You save your family and", color), vcursor.y++);
            v->putcen(ColorString("you have vanquished all evil from the dungeon!", color), vcursor.y++);
        }
        else if (c == dngutil::CreditType::VICTORY)
        {
            v->putcen(ColorString("You save your family but", color), vcursor.y++);
            v->putcen(ColorString("a dark energy still resides in the dungeon...", color), vcursor.y++);
        }
    }
    else if (dungeon == dngutil::DungeonType::FIFTYTRIALS)
    {
        v->putcen(ColorString("DUNGEON RPG - PIT OF 50 TRIALS", color), vcursor.y++);
        v->putcen(ColorString("You conquer the dungeon and", color), vcursor.y++);
        v->putcen(ColorString("have escaped with great treasures!", color), vcursor.y++);
    }
    else if (dungeon == dngutil::DungeonType::UNDERWATER_DUNGEON)
    {
        v->putcen(ColorString("DUNGEON RPG - UNDERWATER TEMPLE", color), vcursor.y++);
        if (c == dngutil::CreditType::SECRET_VICTORY)
        {
            v->putcen(ColorString("You kill the Beast and", color), vcursor.y++);
            v->putcen(ColorString("have vanquished all evil from the temple!", color), vcursor.y++);
        }
        else if (c == dngutil::CreditType::VICTORY)
        {
            v->putcen(ColorString("You rid the ocean of Bloodjaw but", color), vcursor.y++);
            v->putcen(ColorString("a dark energy still resides in the temple...", color), vcursor.y++);
        }
    }
    else
    {
        v->putcen(ColorString("DUNGEON RPG", color), vcursor.y++);
    }

    
    vcursor.y += 4;

    v->putcen(ColorString("Programming: Tyler Lentz", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Story: Tyler Lentz and Thomas Westenhofer", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Play Testing: Tyler Lentz, Thomas Westenhofer, Kristian Rascon,", color), vcursor.y++);
    v->putcen(ColorString("Danny Hernandez and others", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Music/Sound effects: Legend of Zelda 1, 2 and 3", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Special Thanks to: Evan \"Little Fella\" Maich,", color), vcursor.y++);
    v->putcen(ColorString("Niko \"Mile Stretch Mile\" Fernandez and", color), vcursor.y++);
    v->putcen(ColorString("Anthony \"Duganator 3000\" Dugan", color), vcursor.y++);
    vcursor.y += 5;
    if (c != dngutil::CreditType::TITLESCREEN)
    {
        v->putcen(
            ColorString("Your score was: ", color) +
            ColorString(std::to_string(pgame->getScore()), color),
            vcursor.y++
        ); 
        vcursor.y += 2;
        v->putcen(ColorString("Your adventure is over", color), vcursor.y++);
        vcursor.y += 3;
    }
    
    pressEnter(vcursor, v, color);

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
            Coordinate vcursor(0, i);
            VirtualWindow* t = vwin;
            t->clearScreen();
            int color = dngutil::GREEN;
            int scolor = dngutil::WHITE;
            t->put(ColorString(R"( ________  ___  ___  ________   ________  _______   ________  ________      )", color), vcursor); vcursor.y++;
            if (timesThrough > 1)
            {
                t->put(ColorString(R"(|\   ___ \|\  \|\  \|\   ___  \|\   ____\|\  ___ \ |\   __  \|\   ___  \    )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 2)
            {
                t->put(ColorString(R"(\ \  \_|\ \ \  \\\  \ \  \\ \  \ \  \___|\ \   __/|\ \  \|\  \ \  \\ \  \   )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 3)
            {
                t->put(ColorString(R"( \ \  \ \\ \ \  \\\  \ \  \\ \  \ \  \  __\ \  \_|/_\ \  \\\  \ \  \\ \  \  )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 4)
            {
                t->put(ColorString(R"(  \ \  \_\\ \ \  \\\  \ \  \\ \  \ \  \|\  \ \  \_|\ \ \  \\\  \ \  \\ \  \ )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 5)
            {
                t->put(ColorString(R"(   \ \_______\ \_______\ \__\\ \__\ \_______\ \_______\ \_______\ \__\\ \__\)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 6)
            {
                t->put(ColorString(R"(    \|_______|\|_______|\|__| \|__|\|_______|\|_______|\|_______|\|__| \|__|)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 7)
            {
                t->put(ColorString(R"(                       ________  ________  ________                         )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 8)
            {
                t->put(ColorString(R"(                      |\   __  \|\   __  \|\   ____\                        )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 9)
            {
                t->put(ColorString(R"(                      \ \  \|\  \ \  \|\  \ \  \___|                        )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 10)
            {
                t->put(ColorString(R"(                       \ \   _  _\ \   ____\ \  \  ___                      )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 11)
            {
                t->put(ColorString(R"(                        \ \  \\  \\ \  \___|\ \  \|\  \                     )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 12)
            {
                t->put(ColorString(R"(                         \ \__\\ _\\ \__\    \ \_______\                    )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 13)
            {
                t->put(ColorString(R"(                          \|__|\|__|\|__|     \|_______|                    )", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 14)
            {
                t->putcen(ColorString(R"(           /\                                                 /\           )", scolor), vcursor.y); vcursor.y++;
            }
            if (timesThrough > 15)
            {
                t->putcen(ColorString(R"( _         )( ______________________   ______________________ )(         _ )", scolor), vcursor.y); vcursor.y++;
            }
            if (timesThrough > 16)
            {
                t->putcen(ColorString(R"((_)///////(**)______________________> <______________________(**)\\\\\\\(_))", scolor), vcursor.y); vcursor.y++;
            }
            if (timesThrough > 17)
            {
                t->putcen(ColorString(R"(           )(                                                 )(           )", scolor), vcursor.y); vcursor.y++;
            }
            if (timesThrough > 18)
            {
                t->putcen(ColorString(R"(           \/                                                 \/           )", scolor), vcursor.y); vcursor.y++;
            }
            
            timesThrough++;
            
            if (timesThrough < 18)
            {
                Sleep(75);
            }
            else
            {   
                Sleep(50);
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