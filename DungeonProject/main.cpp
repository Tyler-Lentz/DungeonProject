#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include <thread>
#include <Windows.h>
#include <memory>

/*
  add more spells
  fig out battle music (the 2 new tracks)
  play through and test whole game
X increase max health, att, def
X buff dark knights
X change mana bar
X nerf critical hits
  mana potions  
X scale health by x3
*/

int main()
{
    std::unique_ptr<VirtualWindow> vwin(new VirtualWindow(dngutil::CONSOLEX, dngutil::CONSOLEY));
    vwin->getConsole().setTitle("Harp of the Gods");

    bool exit = false;
    std::thread framerate([&vwin, &exit]()
    {
        while (!exit)
        {
            vwin->refresh();
            Sleep(33);
        }
    });

    intro(vwin.get());

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