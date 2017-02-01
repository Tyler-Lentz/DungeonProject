#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>
#include <memory>

int main()
{
    std::unique_ptr<VirtualWindow> vwin(new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY));
    vwin->getConsole().setTitle("Dungeon RPG 2");
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
                game.reset(save);
            }
            else
            {
                game.reset(new Game(vwin.get()));
            }
            break;

        case dngutil::ReturnVal::RESTART:
            if (game->getLastSave() != nullptr)
            {
                delete game->getLastSave();
            }
            game.reset(new Game(vwin.get()));
            break;

        case dngutil::ReturnVal::EXIT:
            returnValue = 0;
            exit = true;
            break;
        }
    }

    framerate.join();

    return returnValue;
}