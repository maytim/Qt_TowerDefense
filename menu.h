#ifndef MENU_H
#define MENU_H

#include "image.h"
#include "button.h"
#include "game.h"

#include <QWidget>

class Menu : public QWidget
{
    Q_OBJECT
public:
    Menu(QWidget *parent = 0);
    ~Menu();

    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
private:
    Image* title;
    Button* start;
    Button* help;
    Game* game;
};

#endif // MENU_H
