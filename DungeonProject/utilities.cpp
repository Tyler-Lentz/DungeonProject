#include "utilities.h"
#include "mapobject.h"
#include "virtualwindow.h"

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
    Sleep(10000);
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
    for (std::list<MapObject*>::const_iterator it = list.begin(); it != list.end(); it++)
    {
        if ((*it)->getPriority() > p)
        {
            list.insert(it, objectToAdd);
            return;
        }
    }

    list.push_back(objectToAdd);
}

void pressEnter(Coordinate cursorpos, VirtualWindow* vwin)
{
    vwin->putcen(ColorString("Press enter to continue", dngutil::CYAN), cursorpos.y);

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