#include "virtualwindow.h"
#include "utilities.h"
#include "game.h"

#include "enemy.h"
#include <thread>
#include <Windows.h>
#include <memory>

/*
X all bosses more hp
X bosses drop mana pots too
X make puzzle sound upon entering room
X add more spells
X   make lower level healing spell         
X make dragons breath spell better & add lower level heal ^
X scale up potions slightly ^
X desert town vender sells mana potions instead of magical potions
X make statsIncreaseDisplay easier to read
X better scale potions with new hp system
X put spirit roller spell on map
X reduce number of enemies in tower of gods & demon fort if needed
X change weapon sterngths to be triangle // no crazy benefits only a little
X fix tower room in demon tower that looks dumb
  add room to left of bridge in demon temple and upper room
? make it so cant get same spell more than once
X make final dungeon giant demon temple thing ,
  & put save point before and a way to leave
X fix hp on the cheat save file so it matches up
  put mana potions on map
X fig out battle music (the 2 new tracks)
  play through and test whole game
X increase max health, att, def
X buff dark knights
X change mana bar
X nerf critical hits
X    REBUFF THEM (x2) ^
X           RENERF THEM TO 1.5
X mana potions  
X scale health by x3
X   nerf magical potions to only heal to 25% hp
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