#include "tooltip.h"
#include "image.h"
#include "game.h"
#include <QPainter>

ToolTip::ToolTip(int price, std::string stat_name, int upgrade)
{
    /*cost = Game::mergeChars("cost", 1, NORMAL);
    cost_amount = Game::mergeChars(std::to_string(price), 1, ACTIVE);
    stat = Game::mergeChars(stat_name, 1, NORMAL);
    stat_upgrade = Game::mergeChars(std::to_string(upgrade), 1, ACTIVE);
    background = new Image(TOOLTIP::BASE);*/
}

ToolTip::~ToolTip(){
    delete cost;
    delete cost_amount;
    delete background;
    delete stat;
    delete stat_upgrade;
}

void ToolTip::moveTo(double x, double y){
    //x and y correspond to the mosue coordinates so need to draw a rect to the bottom left of the coordinates
    resizeBackground();
    background->getRect()->moveTo(x-background->getRect()->width(), y);
    stat->getRect()->moveTo(background->getRect()->x()+2, background->getRect()->y()+2);
    stat_upgrade->getRect()->moveTo(stat->getRect()->x()+3, stat->getRect()->y());
    cost->getRect()->moveTo(stat_upgrade->getRect()->x()+5, stat_upgrade->getRect()->y());
    cost_amount->getRect()->moveTo(cost->getRect()->x()+3, cost->getRect()->y());
}

void ToolTip::paint(QPainter *p){
    p->drawImage(*background->getRect(), *background->getImage());
    p->drawImage(*stat->getRect(), *stat->getImage());
    p->drawImage(*stat_upgrade->getRect(), *stat_upgrade->getImage());
    p->drawImage(*cost->getRect(), *cost->getImage());
    p->drawImage(*cost_amount->getRect(), *cost_amount->getImage());
}

void ToolTip::resizeBackground(){
    int width = 2 + stat->getRect()->width() + 3 + stat_upgrade->getRect()->width() + 5 +
            cost->getRect()->width() + 3 + cost_amount->getRect()->width() + 2;
    int height = cost->getRect()->height() + 4;
    background->getImage()->scaled(width, height);
    background->setRect(background->getImage()->rect());
}
