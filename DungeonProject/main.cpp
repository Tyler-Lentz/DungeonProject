#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

// TODO when everything else is done: implement menu class

int main()
{
    VirtualWindow* vwin = new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY);
    vwin->getConsole().setTitle("Dungeon RPG 2");

    Game* game = new Game(vwin);
    bool exit = false;
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

    delete vwin;

    return returnValue;
}