#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>
#include <memory>

/*
changes:
1. make controls more clear
2. adjust speed
3. dyamic leveling
4. altars
5. fix keys (done)
6. revival potion 
7. stats menu (done)
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