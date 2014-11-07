#include "waypoint.h"
#include <QDebug>

Waypoint::Waypoint(int x, int y)
{
    pos = new QPointF(x, y);
    qDebug() << "new waypoint at (" << pos->x() << ", " << pos->y() << ")";
}
