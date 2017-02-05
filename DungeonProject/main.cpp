#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>
#include <memory>

/*
beta 2 changes:
1.  make how to exit inventory more clear (done)
2.  make starting speed slower (done)
3.  dyamic stat leveling (done)
4.  fix keys disapearring (done)
5.  revival potion (done)
6.  stats menu (done)
7.  white staircases (done)
8.  fixed hit sounds (done)
9.  slightly nerfed mage's staff (done)
10. title screen variants (done)
11. full health on level (done)
12. added missing third key (done)
13. altars & beasts (done)
*/

int main()
{
    std::unique_ptr<VirtualWindow> vwin(new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY));
    vwin->getConsole().setTitle("Dungeon RPG - Dragon's Lair");
    bool exit = false;

    std::thread framerate([&vwin, &exit]()
    {
        while (!exit)
        {
            vwin->refresh();
            Sleep(33);
        }
    });

    std::unique_ptr<Game> game(new Game(vwin.get()));
    while (!exit)
    {
        switch (game->run())
        {
        case dngutil::ReturnVal::RESTART:
            game.reset(new Game(vwin.get()));
            break;

        case dngutil::ReturnVal::EXIT:
            exit = true;
            break;
        }
    }

    framerate.join();

    return 0;
}