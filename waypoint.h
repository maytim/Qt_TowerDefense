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
#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPointF>

/*
    @class Waypoint
    @brief A class to mark the path for enemy navigation.
    @detail A waypoint is the invisible gameobject that marks the path for the enemies to
    travel in game.
*/
class Waypoint
{
public:
    /*
        The constructor that I use to make a new waypoint. It accepts x and y coordinates and creates a new qpointf
        at those coordinates.
    */
    Waypoint(int x, int y) : pos(new QPointF(x, y)){}

    //A getter for the pos value
    inline QPointF getPos() const{ return *pos; }
private:
    //The position property
    QPointF* pos;
};

#endif // WAYPOINT_H
