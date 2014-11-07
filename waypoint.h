#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPointF>

class Waypoint
{
public:
    Waypoint(int x, int y);
    QPointF getPos() const{ return *pos; }
private:
    QPointF* pos;
};

#endif // WAYPOINT_H
