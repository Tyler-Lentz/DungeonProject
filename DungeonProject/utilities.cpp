#include "utilities.h"
#include "mapobject.h"
#include "virtualwindow.h"
#include "game.h"
#include "item.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <list>

void errorMessage(std::string error, int line, std::string file)
{
    std::cerr << "Error on line " << line << " in the file " << file << "\n- " << error;
    Sleep(100000);
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
    if (!(GetAsyncKeyState(key) < 0)) // if its not pressed
    {
        return false;
    }

    if (GetConsoleWindow() != GetForegroundWindow()) // if the window is not active (i.e. looking at something else)
    {
        return false;
    }
    return true;
}

bool keyrelease(int key)
{
    if (GetAsyncKeyState(key) < 0) // if the key is down
    {
        return false;
    }
    if (GetConsoleWindow() != GetForegroundWindow())
    {
        return false;
    }
    return true;
}

void sortPriority(std::list<MapObject*>& list, MapObject* objectToAdd)
{
    int p = objectToAdd->getPriority();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        if ((*it)->getPriority() > p)
        {
            list.insert(it, objectToAdd);
            return;
        }
    }

    list.push_back(objectToAdd);
}

void pressEnter(Coordinate cursorpos, VirtualWindow* vwin, int color)
{
    vwin->putcen(ColorString("Press enter to continue", color), cursorpos.y);

    while (!keyrelease(VK_RETURN));

    while (!keypress(VK_RETURN));

    while (!keyrelease(VK_RETURN));
}

int getExpToLevel(unsigned int level)
{
    int exp = static_cast<int>(((0.15 * (level * level)) + 50)* 1.25);
    if (exp > 170)
    {
        exp = 170;
    }
    return exp;
}



int getHealthbarSleepTime(int damageDealt)
{
    return (200 / damageDealt);
}

void credits(dngutil::CreditType c, Game* pgame)
{
    VirtualWindow* v = pgame->getVWin();

    Coordinate vcursor(0, 5);

    int color;
    int color2;
    int sleepTime;
    int clearScreenColor;
    if (c == dngutil::CreditType::TITLESCREEN)
    {
        v->clearScreen(dngutil::BLACK);

        color = dngutil::WHITE;
        color2 = dngutil::GREEN;
        sleepTime = 200;
        clearScreenColor = dngutil::BLACK;
    }
    else
    {
        v->clearScreen(dngutil::WHITE);
        color = getColor(dngutil::BLACK, dngutil::WHITE);
        color2 = getColor(dngutil::GREEN, dngutil::WHITE);
        sleepTime = 450;
        clearScreenColor = dngutil::WHITE;
    }

    std::vector<ColorString> credits;

    auto addEmptySpace = [&](int amount) {
        for (int i = 0; i < amount; i++)
        {
            credits.push_back(ColorString("", color));
        }
    };

    addEmptySpace(35);
    credits.push_back(ColorString("Harp of the Gods", color2));
    if (c == dngutil::CreditType::VICTORY)
    {
        addEmptySpace(5);
        credits.push_back(ColorString(R"(         ____                   )", color));
        credits.push_back(ColorString(R"(         SSSS____.              )", color));
        credits.push_back(ColorString(R"(         (WW);;;;;\             )", color));
        credits.push_back(ColorString(R"(         `WW'____ |     ,_____  )", color));
        credits.push_back(ColorString(R"(          UU ||||\ \___/,---. ) )", color));
        credits.push_back(ColorString(R"(          UU |||||\____/||| //  )", color));
        credits.push_back(ColorString(R"(          UU ||||||||||||" //   )", color));
        credits.push_back(ColorString(R"(          UU |||||||||||' //    )", color));
        credits.push_back(ColorString(R"(          UU |||||||||"  //     )", color));
        credits.push_back(ColorString(R"(          UU ||||||||'  //      )", color));
        credits.push_back(ColorString(R"(          UU |||||||"  //       )", color));
        credits.push_back(ColorString(R"(          UU ||||||'  //        )", color));
        credits.push_back(ColorString(R"(          UU ||||"   //         )", color));
        credits.push_back(ColorString(R"(          UU |||"   //          )", color));
        credits.push_back(ColorString(R"(          UU ||'   //           )", color));
        credits.push_back(ColorString(R"(          UU |"   //            )", color));
        credits.push_back(ColorString(R"(         ,UU,'   ||             )", color));
        credits.push_back(ColorString(R"(       (~~~~~~~~~~~~]""'        )", color));
        credits.push_back(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- The Harp -", color));
    }
    addEmptySpace(20);
    credits.push_back(ColorString("Programming: Tyler Lentz", color)); 
    if (c == dngutil::CreditType::VICTORY)
    {
        addEmptySpace(5);
        credits.push_back(ColorString("                      . . . .", color));
        credits.push_back(ColorString("                      ,`,`,`,`,", color));
        credits.push_back(ColorString(". . . .               `\\`\\`\\`\\;", color));
        credits.push_back(ColorString("`\\`\\`\\`\\`,            ~|;!;!;\\!", color));
        credits.push_back(ColorString(" ~\\;\\;\\;\\|\\          (--,!!!~`!       .", color));
        credits.push_back(ColorString("(--,\\\\\\===~\\         (--,|||~`!     ./", color));
        credits.push_back(ColorString(" (--,\\\\\\===~\\         `,-,~,=,:. _,//", color));
        credits.push_back(ColorString("  (--,\\\\\\==~`\\        ~-=~-.---|\\;/J,", color));
        credits.push_back(ColorString("   (--,\\\\\\((```==.    ~'`~/       a |", color));
        credits.push_back(ColorString("     (-,.\\\\('('(`\\\\.  ~'=~|     \\_.  \\", color));
        credits.push_back(ColorString("        (,--(,(,(,'\\\\. ~'=|       \\\\_;>", color));
        credits.push_back(ColorString("          (,-( ,(,(,;\\\\ ~=/        \\", color));
        credits.push_back(ColorString("          (,-/ (.(.(,;\\\\,/          )", color));
        credits.push_back(ColorString("           (,--/,;,;,;,\\\\         ./------.", color));
        credits.push_back(ColorString("             (==,-;-'`;'         /_,----`. \\", color));
        credits.push_back(ColorString("     ,.--_,__.-'                    `--.  ` \\", color));
        credits.push_back(ColorString("    (='~-_,--/        ,       ,!,___--. \\  \\_)", color));
        credits.push_back(ColorString("   (-/~(     |         \\   ,_-         | ) /_|", color));
        credits.push_back(ColorString("   (~/((\\    )\\._,      |-'         _,/ /", color));
        credits.push_back(ColorString("    \\\\))))  /   ./~.    |           \\_\\;", color));
        credits.push_back(ColorString(" ,__/////  /   /    )  /", color));
        credits.push_back(ColorString("  '===~'   |  |    (, <.", color));
        credits.push_back(ColorString("           / /       \\. \\", color));
        credits.push_back(ColorString("         _/ /          \\_\\", color));
        credits.push_back(ColorString("        /_!/            >_\\", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- The 3 Elemental Horses -", color));
        addEmptySpace(8);
        credits.push_back(ColorString("    _,_", color));
        credits.push_back(ColorString("  /7/Y/^\\", color));
        credits.push_back(ColorString("  vuVV|C)|            ", color) + ColorString("             __ _", color));
        credits.push_back(ColorString("    \\|^ /          ", color) + ColorString("             .'  Y '>,", color));
        credits.push_back(ColorString("    )| \\)       ", color) + ColorString("               / _   _   \\", color));
        credits.push_back(ColorString("   //)|\\\\     ", color) + ColorString("                 )(_) (_)(|}", color));
        credits.push_back(ColorString("  / ^| \\ \\    ", color) + ColorString("                 {  4A   } /", color));
        credits.push_back(ColorString(" //^| || \\\\   ", color) + ColorString("                  \\uLuJJ/\\l", color));
        credits.push_back(ColorString(">//||| |\\\\\\|   ", color) + ColorString("                 |3    p)/", color));
        credits.push_back(ColorString("| \"\"\"\"\"  7/>l ", color) + ColorString("__ _____ ____      /nnm_n//", color));
        credits.push_back(ColorString("L>_   _-< 7/|", color) + ColorString("_-__,__-)\\,__)(\".  \\_>-<_/D", color));
        credits.push_back(ColorString(")D\" Y \"c)  9)  ", color) + ColorString("     //V     \\_\"-._.__G<D     __", color));
        credits.push_back(ColorString("\\  ||    . _/ ", color) + ColorString("      (       \\ \"-._\"> G<D-_.-\"_=)_", color));
        credits.push_back(ColorString(" ) __   / /  ", color) + ColorString("       \"      _/.__ />_c__.-__<\"/ ( \\", color));
        credits.push_back(ColorString(" | | |  |(|  ", color) + ColorString("             < \"-._\"> _.G_.___)\\   \\7\\", color));
        credits.push_back(ColorString("  \\\"=\" // |   ", color) + ColorString("           (,\"-.__.|\\ \\<.__.-\" )   \\ \\", color));
        credits.push_back(ColorString("   '---'  |     ", color) + ColorString("         |,\"-.__\"| \\!\"-.__.-\".\\   \\ \\", color));
        credits.push_back(ColorString("     |_;._/    ", color) + ColorString("          (_\"-.__\"'\\ \\\"-.__.-\".|    \\_\\", color));
        credits.push_back(ColorString("     )(\" V     ", color) + ColorString("           \\\"-.__\"'|\\ \\-.__.-\".)     \\ \\", color));
        credits.push_back(ColorString("        (     ", color) + ColorString("             \"-.__'\"\\_\\ \\.__.-\"./      \\ l", color));
        credits.push_back(ColorString("         )          ", color) + ColorString("        \".__\"\">>G\\ \\__.-\">        V )", color));
        credits.push_back(ColorString("                                \"\"  G<\\ \\_.-\"        ./7", color));
        credits.push_back(ColorString("                                     G<\\ \\          ///", color));
        credits.push_back(ColorString("                                ___  G<D\\ \\        /7^", color));
        credits.push_back(ColorString("                              /\".-_\"-\"G\"<\\ \\__    ///))", color));
        credits.push_back(ColorString("                              ||,-_ \"     '.\\_\"- /// (\\", color));
        credits.push_back(ColorString("                              | (_)  _   \"/ __7 |X/   V", color));
        credits.push_back(ColorString("                             /\\__\"\"-/ \"\\ _\"\"c_\"GBV", color));
        credits.push_back(ColorString("                            _)(  \"\"'    \"--.c__>D'", color)); addEmptySpace(2);
        credits.push_back(ColorString("- Skeleton King -", color));
    }
    addEmptySpace(8);
    credits.push_back(ColorString("Story: Tyler Lentz", color));
    if (c == dngutil::CreditType::VICTORY)
    {
        addEmptySpace(5);
        credits.push_back(ColorString(R"(        _)", color));
        credits.push_back(ColorString(R"(       (_))", color));
        credits.push_back(ColorString(R"(       |=|)", color));
        credits.push_back(ColorString(R"(       |=|)", color));
        credits.push_back(ColorString(R"(   /|__|_|__|\)", color));
        credits.push_back(ColorString(R"(  (    ( )    ))", color));
        credits.push_back(ColorString(R"(   \|\/\"/\/|/)", color));
        credits.push_back(ColorString(R"(     |  Y  |)", color));
        credits.push_back(ColorString(R"(     |  |  |)", color));
        credits.push_back(ColorString(R"(     |  |  |)", color));
        credits.push_back(ColorString(R"(    _|  |  |)", color));
        credits.push_back(ColorString(R"( __/ |  |  |\)", color));
        credits.push_back(ColorString(R"(/  \ |  |  |  \)", color));
        credits.push_back(ColorString(R"(   __|  |  |   |)", color));
        credits.push_back(ColorString(R"(/\/  |  |  |   |\)", color));
        credits.push_back(ColorString(R"( <   +\ |  |\ />  \)", color));
        credits.push_back(ColorString(R"(  >   + \  | LJ    |)", color));
        credits.push_back(ColorString(R"(        + \|+  \  < \)", color));
        credits.push_back(ColorString(R"(  (O)      +    |    ))", color));
        credits.push_back(ColorString(R"(   |             \  /\ )", color));
        credits.push_back(ColorString(R"( ( | )   (o)      \/  ))", color));
        credits.push_back(ColorString(R"(_\\|//__( | )______)_/ )", color));
        credits.push_back(ColorString(R"(        \\|//        )", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- The Hero's Blade -", color));
        addEmptySpace(8);
        credits.push_back(ColorString(R"( _________________________ )", color));
        credits.push_back(ColorString(R"(|<><><>     |  |    <><><>|)", color));
        credits.push_back(ColorString(R"(|<>         |  |        <>|)", color));
        credits.push_back(ColorString(R"(|           |  |          |)", color));
        credits.push_back(ColorString(R"(|  (______ <\-/> ______)  |)", color));
        credits.push_back(ColorString(R"(|  /_.-=-.\| " |/.-=-._\  |)", color));
        credits.push_back(ColorString(R"(|   /_    \(o_o)/    _\   |)", color));
        credits.push_back(ColorString(R"(|    /_  /\/ ^ \/\  _\    |)", color));
        credits.push_back(ColorString(R"(|      \/ | / \ | \/      |)", color));
        credits.push_back(ColorString(R"(|_______ /((( )))\ _______|)", color));
        credits.push_back(ColorString(R"(|      __\ \___/ /__      |)", color));
        credits.push_back(ColorString(R"(|--- (((---'   '---))) ---|)", color));
        credits.push_back(ColorString(R"(|           |  |          |)", color));
        credits.push_back(ColorString(R"(|           |  |          |)", color));
        credits.push_back(ColorString(R"(:           |  |          :)", color));
        credits.push_back(ColorString(R"( \<>        |  |       <>/ )", color));
        credits.push_back(ColorString(R"(  \<>       |  |      <>/  )", color));
        credits.push_back(ColorString(R"(   \<>      |  |     <>/   )", color));
        credits.push_back(ColorString(R"(    `\<>    |  |   <>/'    )", color));
        credits.push_back(ColorString(R"(      `\<>  |  |  <>/'     )", color));
        credits.push_back(ColorString(R"(        `\<>|  |<>/'       )", color));
        credits.push_back(ColorString(R"(          `-.  .-`         )", color));
        credits.push_back(ColorString(R"(            '--'           )", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- The Hero's Shield -", color));
    }
    addEmptySpace(8);
    credits.push_back(ColorString("Playtesting: Tyler Lentz, Thomas Westenhofer, Collin Werth,", color));
    credits.push_back(ColorString("Kristian Rascon, Danny Hernandez and Zachary Fineberg", color));
    if (c == dngutil::CreditType::VICTORY)
    {
        addEmptySpace(5);
        credits.push_back(ColorString(R"(                         /\     /\)", color));
        credits.push_back(ColorString(R"(                        (_ \   (  \)", color));
        credits.push_back(ColorString(R"(                        (_ `\iRBNMMbn.)", color));
        credits.push_back(ColorString(R"(                         \_  \VAPRBNMMb)", color));
        credits.push_back(ColorString(R"(                           \o_)lI<= (=\)", color));
        credits.push_back(ColorString(R"(                          / `   ) , _\ ))", color));
        credits.push_back(ColorString(R"(                          )    /   (_(/)", color));
        credits.push_back(ColorString(R"(                         /_ __( \ __ _|)", color));
        credits.push_back(ColorString(R"(                            _|     |_)", color));
        credits.push_back(ColorString(R"(                   ,edMMNBRB)\ \_  /(PRBNMMbn.)", color));
        credits.push_back(ColorString(R"(                  dMMNBRPAV/o(_____)o\VAPRBNMMb)", color));
        credits.push_back(ColorString(R"(                 fMMWBRYIli\ o  o  o /rlIPRBWMMj)", color));
        credits.push_back(ColorString(R"(                 MMBRRSZIlti+._.-._.+ilIZSRRBMMM)", color));
        credits.push_back(ColorString(R"(                 /_o__o_/KZYI0lLRBNWMMMN\_o___o_\)", color));
        credits.push_back(ColorString(R"(                /_____/flZJlDYTPRKBBNWMM \ ___( \)", color));
        credits.push_back(ColorString(R"(          NNBR0PRRBNMMMMMMMlIPZSKRNWMMMV  \__._\ \)", color));
        credits.push_back(ColorString(R"(          NMBRR00PRBNNMMMMM_o___o___o__/   )_ __)o))", color));
        credits.push_back(ColorString(R"(          NMMBRR00PRRBNNMMM___o___o___/    /___/ /)", color));
        credits.push_back(ColorString(R"(          NMMBBRR00PRRBNNMM_/\_/\_/\_(  __/___x)/)", color));
        credits.push_back(ColorString(R"(          NMMBBRR00PRRBNNMM \/ \/ \/ __ )    /(`)", color));
        credits.push_back(ColorString(R"(        __NMMBBRR00PRRBNNMM_________|_ /   _'))", color));
        credits.push_back(ColorString(R"(       /__        ________        ___)(<  (====.)", color));
        credits.push_back(ColorString(R"(       \_____________________________)_\ __\---')", color));
        credits.push_back(ColorString(R"(            VMBBR00RBNMNV           |__|\/)", color));
        credits.push_back(ColorString(R"(             lMBRR00RNMV  )", color));
        credits.push_back(ColorString(R"(              VMB00BMNV   )", color));
        credits.push_back(ColorString(R"(               VMB0BMV   )", color));
        credits.push_back(ColorString(R"(                b'ger  )", color));
        credits.push_back(ColorString(R"(                 VMV     )", color));
        credits.push_back(ColorString(R"(                  V     )", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- The Hero (Aegeus) -", color));
    }
    addEmptySpace(8);
    credits.push_back(ColorString("Music: The Legend of Zelda, Mario, Final Fantasy, ", color));
    if (c == dngutil::CreditType::VICTORY)
    {
        addEmptySpace(5);
        credits.push_back(ColorString(R"(           _/          ,          .                                          )", color));
        credits.push_back(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", color));
        credits.push_back(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", color));
        credits.push_back(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", color));
        credits.push_back(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", color));
        credits.push_back(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", color));
        credits.push_back(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", color));
        credits.push_back(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", color));
        credits.push_back(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", color));
        credits.push_back(ColorString(R"( /,'     ||          \           \      /              \\                    )", color));
        credits.push_back(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", color));
        credits.push_back(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", color));
        credits.push_back(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", color));
        credits.push_back(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", color));
        credits.push_back(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", color));
        credits.push_back(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", color));
        credits.push_back(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", color));
        credits.push_back(ColorString(R"(        ' ;    /  ,'          ,'                                             )", color));
        credits.push_back(ColorString(R"(       /,'    /  /      '    /     , - --- .                                 )", color));
        credits.push_back(ColorString(R"(       \|    /  (          ,'   '           `.                               )", color));
        credits.push_back(ColorString(R"(       ('  ,'    `.    "  / ,'                \                              )", color));
        credits.push_back(ColorString(R"(         ,'        \    ,/,'        '`)   (_   )                             )", color));
        credits.push_back(ColorString(R"(        /           \ , /'          ,      /  /                              )", color));
        credits.push_back(ColorString(R"(       .             )  ,       ,         '  /                               )", color));
        credits.push_back(ColorString(R"(                      )      ,              /                                )", color));
        credits.push_back(ColorString(R"(       .            ' `|   ,'              /                                 )", color));
        credits.push_back(ColorString(R"(                    '  |  /              ,'                                  )", color));
        credits.push_back(ColorString(R"(        |\             | <    ______,---'                                    )", color));
        credits.push_back(ColorString(R"(        ` \            ','   (                                               )", color));
        credits.push_back(ColorString(R"(         \ '          /(____ ,`-._,-.                                        )", color));
        credits.push_back(ColorString(R"(          `.         /      `._, )---)                                       )", color));
        credits.push_back(ColorString(R"(            `-------'\         `/ \    )", color));
        addEmptySpace(2);
        credits.push_back(ColorString("- Zorlock -", color));
    }
    addEmptySpace(32);
    if (c == dngutil::CreditType::VICTORY)
    {
        credits.push_back(ColorString("Peace has been restored to Bora", color2));
        credits.push_back(ColorString("Your quest is over...", color2));
        addEmptySpace(10);
    }        
    else
    {
        credits.push_back(ColorString("Press enter to continue", color2));
    }
    addEmptySpace(16);

    for (unsigned int i = 0; i < credits.size() - 40; i++)
    {
        Sleep(sleepTime);
        v->clearScreen(clearScreenColor);
        for (unsigned int j = 0; j < 40; j++)
        {
            v->put(credits[i + j], Coordinate(3, j));
        }
        if (keypress(VK_RETURN) && c == dngutil::CreditType::TITLESCREEN)
        {
            goto skipCredits;
        }
    }

    if (c == dngutil::CreditType::TITLESCREEN)
    {
        while (!keypress(VK_RETURN));
        skipCredits:
        ;
    }
    else
    {
        stopSound(SoundType::MP3);
        v->clearScreen();
        Sleep(1500);
        v->putcen(ColorString("Thank you for playing", dngutil::WHITE), v->txtmacs.BOTTOM_DIVIDER_TEXT_LINE);
        Sleep(5000);
    }

    v->clearScreen();


    pgame->cleanup(dngutil::ReturnVal::RESTART);
}

void intro(VirtualWindow* vwin)
{
    VirtualWindow* t = vwin;
    Coordinate temp_cursor;
    temp_cursor.x = 8;
    temp_cursor.y = 5;
    t->put(ColorString(R"(                  [\)", dngutil::WHITE), temp_cursor); temp_cursor.y++;
    t->put(ColorString(R"(                  |\)                                ____)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(                  |                               __(_   )__)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(                  Y\          ___               _(          ))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(                 T  \       __)  )--.          (     )-----`)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(                J    \   ,-(         )_         `---')", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(               Y/T`-._\ (     (       _)                 __)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(               /[|   ]|  `-(__  ___)-`  |\          ,-(  __))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(               | |    |      (__)       J'         (     ))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(   _           | |  ] |    _           /;\          `-  ')", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(  (,,)        [| |    |    L'         /;  \)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(             /||.| /\ |   /\         /.,-._\        ___ _)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(            /_|||| || |  /  \        | |{  |       (._.'_))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(  L/\       | \| | '` |_ _ {|        | | U |   /\)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"( /v^v\/\   `|  Y | [  '-' '--''-''-"-'`'   | ,`^v\ /\,`\)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(/ ,'./  \.` |[   |       [     __   L    ] |      /^v\  \)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(,'     `    |    |           ,`##Y.   ]    |___Y Y____,_,,_,,_)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(--   -----.-(] [ |   ]     o/####U|o      ]|| /`-, Y   _   Y  Y)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(   Y Y  --;`~T   |      }   \####U|[\ _,.-(^) ,-'  _  (^)__  _)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(  Y  YY   ;'~~l  |   L     [|\###U'E'\  \ \Y-` _  (^) _Y  _)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"( Y  Y Y   ;\~~/\{| [      _,'-\`= = '.\_ ,`   (^)(^) (^) (^))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(     --   ;\~~~/\|  _,.-'`_  `.\_..-'"  _ . ,_ Y_ Y_ _Y  _Y__)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(    _    _; \~~( Y``   Y (^) / `,      (^)      _   (^) (^))", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(   (^)  (^)`._~ /  L \  _.Y'`  _  ` --  Y - - -(^) - Y - Y -)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(    Y    Y    `'--..,-'`      (^)   _  -    _   Y ____)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(      --           _    _ --   Y   (^)   _ (^)  ===   ----)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(          __   -  (^)  (^)      --- Y   (^) Y)", dngutil::WHITE), temp_cursor); temp_cursor.y++; 
    t->put(ColorString(R"(      _            Y    Y                Y             )", dngutil::WHITE), temp_cursor); temp_cursor.y++; 


    playSound(Mp3File("Intro"));
    bool keepGoing = true;
    for (int i = 0; i < 600; i++)
    {
        Sleep(10);
        if (keypress(VK_RETURN))
        {
            keepGoing = false;
            break;
        }
    }

    if (keepGoing)
    {
        int timesThrough = 0;
        for (int i = dngutil::CONSOLEY - 1; i > 2; i--)
        {
            Coordinate vcursor(5, i);
            t->clearScreen();
            int color = dngutil::YELLOW;
            int scolor = dngutil::WHITE;
            t->put(ColorString(R"(   __ __                     ___  __  __         _____        __  )", color), vcursor); vcursor.y++;
            if (timesThrough > 1)
            {
                t->put(ColorString(R"(  / // /__ ________    ___  / _/ / /_/ /  ___   / ___/__  ___/ /__)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 2)
            {
                t->put(ColorString(R"( / _  / _ `/ __/ _ \  / _ \/ _/ / __/ _ \/ -_) / (_ / _ \/ _  (_-<)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 3)
            {
                t->put(ColorString(R"(/_//_/\_,_/_/ / .__/  \___/_/   \__/_//_/\__/  \___/\___/\_,_/___/)", color), vcursor); vcursor.y++;
            }
            if (timesThrough > 4)
            {
                t->put(ColorString(R"(             /_/                                                  )", color), vcursor); vcursor.y++;
            }
            
            timesThrough++;
            
            if (timesThrough < 4)
            {
                Sleep(150);
            }
            else
            {   
                Sleep(125);
            }
            if (keypress(VK_RETURN))
            {
                keepGoing = false;
                break;
            }
        }
    }

    if (keepGoing)
    {
        for (int i = 0; i < 500; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }
    }

    stopSound(SoundType::MP3);

    vwin->putcen(ColorString("A game by Tyler Lentz", dngutil::WHITE), 35);
    if (keepGoing)
    {
        for (int i = 0; i < 200; i++)
        {
            Sleep(10);
            if (keypress(VK_RETURN))
            {
                break;
            }
        }
    }
    
    vwin->clearScreen();
    Sleep(333);
}


void statIncreaseDisplay(int change, int prevValue, int color, std::string text, VirtualWindow* vwin, Coordinate& vcursor)
{
    vwin->putcen(ColorString("*---------------------------------*", color), vcursor.y - 1);
    vwin->putcen(ColorString("*---------------------------------*", color), vcursor.y + 1);

    vwin->putcen(ColorString(text + " + " + std::to_string(change), color), vcursor.y);
    

    vcursor.y += 3;

    playSound(WavFile("PickupItem", false, false));

    Sleep(500);
}

std::string getClassName(dngutil::ClassType cl)
{
    switch (cl)
    {
    case dngutil::ClassType::ADVENTURER:
        return "Neutral";
        break;
    case dngutil::ClassType::WIZARD:
        return "Magic";
        break;
    case dngutil::ClassType::KNIGHT:
        return "Close-Combat";
        break;
    case dngutil::ClassType::RANGER:
        return "Ranged";
        break;
    }
    return "INVALID";
}

std::string safeInput(unsigned int maxSize)
{
    std::string name;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    std::cin >> name;
    std::cin.ignore();
    if (name.size() > maxSize)
    {
        name.resize(maxSize);
    }
    return name;
}

void printRandomTitlePic(Game* game)
{
    VirtualWindow* t = game->getVWin();
    Coordinate vcursor(0, t->txtmacs.DIVIDER_LINES[1] + 5);
    t->txtmacs.clearMapArea(false, 0);
    switch (random(3))
    {
    case 0:
        t->putcen(ColorString(R"(         ____                   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(         SSSS____.              )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(         (WW);;;;;\             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(         `WW'____ |     ,_____  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||||\ \___/,---. ) )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |||||\____/||| //  )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||||||||||||" //   )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |||||||||||' //    )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |||||||||"  //     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||||||||'  //      )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |||||||"  //       )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||||||'  //        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||||"   //         )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |||"   //          )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU ||'   //           )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(          UU |"   //            )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(         ,UU,'   ||             )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(       (~~~~~~~~~~~~]""'        )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        t->putcen(ColorString(R"(~~~~~~~~~~~~~~~~~~~~~~~~~~~     )", dngutil::YELLOW), vcursor.y); vcursor.y++;
        break;
    case 1:
        t->put(ColorString(R"(                  [\)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(                  |\)                                ____)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(                  |                               __(_   )__)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(                  Y\          ___               _(          ))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(                 T  \       __)  )--.          (     )-----`)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(                J    \   ,-(         )_         `---')",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(               Y/T`-._\ (     (       _)                 __)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(               /[|   ]|  `-(__  ___)-`  |\          ,-(  __))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(               | |    |      (__)       J'         (     ))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(   _           | |  ] |    _           /;\          `-  ')",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(  (,,)        [| |    |    L'         /;  \)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(             /||.| /\ |   /\         /.,-._\        ___ _)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(            /_|||| || |  /  \        | |{  |       (._.'_))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(  L/\       | \| | '` |_ _ {|        | | U |   /\)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"( /v^v\/\   `|  Y | [  '-' '--''-''-"-'`'   | ,`^v\ /\,`\)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(/ ,'./  \.` |[   |       [     __   L    ] |      /^v\  \)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(,'     `    |    |           ,`##Y.   ]    |___Y Y____,_,,_,,_)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(--   -----.-(] [ |   ]     o/####U|o      ]|| /`-, Y   _   Y  Y)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(   Y Y  --;`~T   |      }   \####U|[\ _,.-(^) ,-'  _  (^)__  _)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(  Y  YY   ;'~~l  |   L     [|\###U'E'\  \ \Y-` _  (^) _Y  _)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"( Y  Y Y   ;\~~/\{| [      _,'-\`= = '.\_ ,`   (^)(^) (^) (^))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(     --   ;\~~~/\|  _,.-'`_  `.\_..-'"  _ . ,_ Y_ Y_ _Y  _Y__)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(    _    _; \~~( Y``   Y (^) / `,      (^)      _   (^) (^))",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(   (^)  (^)`._~ /  L \  _.Y'`  _  ` --  Y - - -(^) - Y - Y -)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(    Y    Y    `'--..,-'`      (^)   _  -    _   Y ____)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(      --           _    _ --   Y   (^)   _ (^)  ===   ----)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(          __   -  (^)  (^)      --- Y   (^) Y)",  dngutil::DARKGRAY), vcursor); vcursor.y++;
        t->put(ColorString(R"(      _            Y    Y                Y             )", dngutil::DARKGRAY), vcursor); vcursor.y++;
        break;
    case 2:
        t->put(ColorString(R"(           _/          ,          .                                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(       , -' )         ( \-------.,')            (\_________________________  )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(     , ,-/ |          /\_) )     \/            ,' _.----------------------,\ )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(   ,',  /, |         /      >--. ,)           / /\\                          )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(  / ,  //|,'        /'     '\--'\\)          /,'  \\     `         `   ,     )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"( / ,  // ||       ,'     (.--^( `')         //     \\                \       )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(( ,  //  ||,___,-'    (___\\  '^^^'        //       \\              ,        )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"( \  //   ||--.__     (     \`^--)  _____.-'/         \\   `                  )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(  >'/    ||,        (       \|_(\-'      ,'           \\         \,          )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"( /,'     ||          \           \      /              \\                    )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"((/       ||           \           )  ,'(     `     `    \\      ,            )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"( `       ||\           \      ) ,'  /_  )                \\    \             )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(         || `.          `.    ,'   /( `.\  \ , \ \,       \\   ,             )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(   `     || (_`.          ` .'   .'  )  `)'            ,   \\                )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(         ||  (_ `-v-------  ^--v' , )                      '\\,              )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(         ||    (    , _,-  /  -./ )'                         `)              )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(     `   '|     ),  ,'    '     )'                                           )", dngutil::MAGENTA), vcursor); vcursor.y++;
        t->put(ColorString(R"(        ' ;    /  ,'          ,'                                             )", dngutil::MAGENTA), vcursor); vcursor.y++;
        break;
    case 3:
        int background = dngutil::DARKGRAY;
        int cross = dngutil::WHITE;
        t->putcen(ColorString(R"(	            .----------------------------._              )", background), vcursor.y++);
        t->putcen(ColorString(R"(           _.-'          '-        .           '-._         )", background), vcursor.y++);
        t->putcen(ColorString(R"(         .'      _|   .    . - .        ._         '.       )", background), vcursor.y++);
        t->putcen(ColorString(R"(      _.'    '           .'     '.               _| |       )", background), vcursor.y++);
        t->putcen(ColorString(R"(     /  _|        _|    ''       ''  |_     '    .  '.      )", background), vcursor.y++);
        t->putcen(ColorString(R"(    |      . -- .      ''         ''      . -- .     |      )", background), vcursor.y++);
        t->putcen(ColorString(R"(   .'    .'      '.   -||         ||    .'      '.   '.     )", background), vcursor.y++);
        t->putcen(ColorString(R"(   | '  ''        ''   ||   )", background) + ColorString(".-.", cross) + ColorString(R"(   ||_  ''        ''   |     )", background), vcursor.y++);
        t->putcen(ColorString(R"(   '.  ''          ''  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   ||  ''          ''  |     )", background), vcursor.y++);
        t->putcen(ColorString(R"(    | -||          ||- ')", background) + ColorString("____|!|____", cross) + ColorString(R"(' -||          ||- |     )", background), vcursor.y++);
        t->putcen(ColorString(R"(    |  ||          ||  )", background) + ColorString("|____-+-____|", cross) + ColorString(R"(  ||          ||  '.    )", background), vcursor.y++);
        t->putcen(ColorString(R"(   .' -||          ||_ ||   )", background) + ColorString("|!|", cross) + ColorString(R"(   ||  ||          ||  _|    )", background), vcursor.y++);
        t->putcen(ColorString(R"(   |_.-||          ||  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   || _||          ||-._|    )", background), vcursor.y++);
        t->putcen(ColorString(R"(_.-' |_||          ||  ||   )", background) + ColorString("| |", cross) + ColorString(R"(   ||  ||          ||_| '-._ )", background), vcursor.y++);
        t->putcen(ColorString(R"(_| |_  |:;;.,::;,.';|--|:;;.)", background) + ColorString("| |", cross) + ColorString(R"(,.';|--|:;;.,::;,.';|     |_ )", background), vcursor.y++);
        t->putcen(ColorString(R"(         :;;.,::;,.';   :;;.)", background) + ColorString("| |", cross) + ColorString(R"(,.';    :;;.,::;,.';  _|   -')", background), vcursor.y++);
        t->putcen(ColorString(R"(   |_                       )", background) + ColorString("| |", cross) + ColorString(R"(                         |_. )", background), vcursor.y++);
        t->putcen(ColorString(R"( _      _|                __)", background) + ColorString("| |", cross) + ColorString(R"(__              |_     _     )", background), vcursor.y++);
        t->putcen(ColorString(R"(|________________________/_______\___________________|______)", background), vcursor.y++);
        t->putcen(ColorString(R"(,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.,:.)", background), vcursor.y++);
        t->putcen(ColorString(R"(------------------------------------------------------------)", background), vcursor.y++);
        break;
    }
    
}