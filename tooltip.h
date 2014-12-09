#ifndef TOOLTIP_H
#define TOOLTIP_H

#include "image.h"
#include <QPainter>

/*namespace TOOLTIP{
    const QString BASE = "C:/Qt/Projects/GameProject/tooltip_base.png";
}*/

class ToolTip
{
public:
    ToolTip(int cost, std::string stat, int upgrade);
    ~ToolTip();
    void moveTo(double x, double y);
    void paint(QPainter* p);
private:
    Image* cost;
    Image* cost_amount;
    Image* background;
    Image* stat;
    Image* stat_upgrade;

    void resizeBackground();
};

#endif // TOOLTIP_H
