#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>

// TODO when everything else is done: implement menu class

int main()
{
    VirtualWindow* vwin = new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY);
    vwin->getConsole().setTitle("Dungeon RPG 2");
    bool exit = false;

    std::thread framerate([vwin, &exit]()
    {
        while (!exit)
        {
            vwin->refresh();
            Sleep(33);
        }
    });

    Game* game = new Game(vwin);
    int returnValue;
    while (!exit)
    {
        switch (game->run())
        {
        case dngutil::ReturnVal::RESTART_SAVE:
            if (game->getLastSave() != nullptr)
            {
                Game* save = game->getLastSave();
                save->getLastSave() = save;
                delete game;
                game = new Game(*save);
            }
            else
            {
                delete game;
                game = new Game(vwin);
            }
            break;

        case dngutil::ReturnVal::RESTART:
            if (game->getLastSave() != nullptr)
            {
                delete game->getLastSave();
            }
            delete game;
            game = new Game(vwin);
            break;

        case dngutil::ReturnVal::EXIT:
            returnValue = 0;
            exit = true;
            if (game->getLastSave() != nullptr)
            {
                delete game->getLastSave();
            }
            delete game;
            break;
        }
    }

    framerate.join();

    delete vwin;

    return returnValue;
}