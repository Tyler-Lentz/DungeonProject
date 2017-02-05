#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>
#include <memory>

/*
changes:
1. make controls more clear (done)
2. adjust speed (done)
3. dyamic leveling (done)
4. altars 
5. fix keys (done)
6. revival potion (done)
7. stats menu (done)
8. white staircases (done)
9. fixed hit sounds (done)
10. slightly nerfed mage's staffs (done)
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