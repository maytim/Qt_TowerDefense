#include "game.h"
#include "menu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*Game w;
    w.setWindowTitle("Tower Defense");
    w.show();*/

    Menu m;
    m.setWindowTitle("Tower Defense");
    m.show();

    return a.exec();
}
