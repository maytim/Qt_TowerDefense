/*
    @mainpage HW9
    @author Tim Maytom (104016902)
    @date 12/10/2014
    @section DESCRIPTION

    This is my last update for the Tower Defense Game.

    Feature List:
        -Dynamically generated Text Images
        -Dynamically generated tile map
        -Random enemy spawner
        -Tower class upgrades
        -Formula based costs and stats for towers
        -Infinite waves with increasing difficulty
*/
#include "game.h"
#include <QApplication>

namespace Temp{
    Game* g;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Temp::g = new Game();
    Temp::g->show();

    return a.exec();
}
