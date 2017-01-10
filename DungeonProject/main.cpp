#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

// TODO when everything else is done: implement menu class

int main()
{
    VirtualWindow* vwin = new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY);
    
    Game* game = new Game(vwin);
    bool exit = false;
    int returnValue;

    while (!exit)
    {
        switch (game->run())
        {
            // TODO: this
        case dngutil::ReturnVal::EXIT:

            break;

        case dngutil::ReturnVal::RESTART:

            break;

        case dngutil::ReturnVal::RESTART_SAVE:

            break;
        }
    }
}