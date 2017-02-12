#include "utilities.h"
#include "mapobject.h"
#include "virtualwindow.h"
#include "game.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <list>

#pragma comment(lib, "Winmm.lib")

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
    if (GetAsyncKeyState(key) < 0)
    {
        return true;
    }
    return false;
}

bool keyrelease(int key)
{
    if (!(GetAsyncKeyState(key) < 0))
    {
        return true;
    }
    return false;
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

void startMp3(std::string fileName)
{
    std::string file = "open \"" + fileName + "\" type mpegvideo alias mp3";
    mciSendString(file.c_str(), NULL, 0, NULL);
    mciSendString("play mp3 repeat", NULL, 0, NULL);
}

void stopMp3()
{
    mciSendString("stop mp3", NULL, 0, NULL);
    mciSendString("close mp3", NULL, 0, NULL);
}

void soundEffect(std::string fileName, bool loop, bool async)
{
    if (loop)
    {
        PlaySound(TEXT(fileName.c_str()), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    }
    else if (async)
    {
        PlaySound(TEXT(fileName.c_str()), NULL, SND_FILENAME | SND_ASYNC);
    }
    else
    {
        PlaySound(TEXT(fileName.c_str()), NULL, SND_FILENAME);
    }
}

void stopSound()
{
    PlaySound(NULL, 0, 0);
}

int getHealthbarSleepTime(int damageDealt)
{
    return (200 / damageDealt);
}

void credits(dngutil::CreditType c, Game* pgame)
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
        startMp3("Win.mp3");
        Sleep(5000);
        stopMp3();

        Sleep(100);
        startMp3("Ending.mp3");

        pgame->adjustScore(dngutil::BASE_SCORE_VICTORY);
    }

    int color = 0;
    if (c == dngutil::CreditType::SECRET_VICTORY)
    {
        color = getColor(dngutil::BLACK, dngutil::LIGHTMAGENTA);
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

    v->putcen(ColorString("DUNGEON RPG - DRAGON'S LAIR", color), vcursor.y++);
    if (c == dngutil::CreditType::SECRET_VICTORY)
    {
        v->putcen(ColorString("FULL ENDING", color), vcursor.y++);
    }
    else if (c == dngutil::CreditType::VICTORY)
    {
        v->putcen(ColorString("A dark energy still resides in the dungeon...", color), vcursor.y++);
    }
    vcursor.y += 4;

    v->putcen(ColorString("Programming: Tyler Lentz", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Story: Tyler Lentz and Thomas Westenhofer", color), vcursor.y++);
    vcursor.y++;
    v->putcen(ColorString("Play Testing: Tyler Lentz, Thomas Westenhoffer, Kristian Rascon,", color), vcursor.y++);
    v->putcen(ColorString("Daniel Hernandez and others", color), vcursor.y++);
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
        stopMp3();
    }

    v->clearScreen();


    pgame->cleanup(dngutil::ReturnVal::RESTART);
}