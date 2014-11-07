#include "menu.h"
#include "image.h"
#include "button.h"
#include "gameobject.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Menu::Menu(QWidget *parent) : QWidget(parent) , game(NULL)
{
    setMouseTracking(true);
    //load images
    title = new Image(CONSTANTS::TITLE_PATH, 0.125);
    start = new Button(CONSTANTS::START_PATH, CONSTANTS::START_H_PATH, 0.25);
    help = new Button(CONSTANTS::HELP_PATH, CONSTANTS::HELP_H_PATH, 0.25);

    //position images
    title->getRect()->translate( (width()-title->getRect()->width())/2 , CONSTANTS::MARGIN_TOP );
    start->getRect()->translate( (width()-start->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title->getRect()->height());
    help->getRect()->translate( (width()-help->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title->getRect()->height() + start->getRect()->height());
}

Menu::~Menu(){
    delete title;
    delete start;
    delete help;
}

void Menu::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.drawImage(*title->getRect(), *title->getImage());
    if(start->isActive())
        painter.drawImage(*start->getRect(), start->getActiveImage());
    else
        painter.drawImage(*start->getRect(), *start->getImage());

    if(help->isActive())
        painter.drawImage(*help->getRect(), help->getActiveImage());
    else
        painter.drawImage(*help->getRect(), *help->getImage());
}

void Menu::mouseMoveEvent(QMouseEvent *event){
    if(start->getRect()->contains(event->pos())){
        start->setActive(true);
        help->setActive(false);
    }
    else if(help->getRect()->contains(event->pos())){
        help->setActive(true);
        start->setActive(false);
    }
    else{
        start->setActive(false);
        help->setActive(false);
    }
    repaint();
}

void Menu::mousePressEvent(QMouseEvent *event){
    if(start->getRect()->contains(event->pos())){
        //Start game
        game = new Game();
        this->close();
        game->show();
    }
    else if(help->getRect()->contains(event->pos())){
        //Open Help Window
    }
}
