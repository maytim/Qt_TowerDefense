/*
    @mainpage HW6
    @author Tim Maytom (104016902)
    @date 11/14/2014
    @section DESCRIPTION

    This is an update of my previous assignment. I have worked on adding game logic to the GUI
    that I had already constructed. The game draws a game map from array data. The enemies navigation
    coordinates have been manually updated to follow the new path. I have added an ingame GUI. This
    GUI includes the wave and score displays at the top of the screen. The number images for these displays
    are drawn from a parsed string with Images that I have created. I have also added a toggle menu on the right
    to select the tower type that you want to build. The towers target enemies by drawing QLine's to the enemy
    and then comparing the distance of that QLine to its range property. If the enemy is within range, the tower
    will reduce its health. When an enemy's health reaches 0, the enemy will be deleted from the game and the score
    will be updated by the appropriate value.

    Issues:
    -no end game event
    -only a single wave. Need to store wave data, and then create a system to load the waves
    -no attacking animations
    -building towers doesn't affect the player's score so the user can create as many towers as they like
    -no tower upgrade system
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
    QPointF getPos() const{ return *pos; }
private:
    //The position property
    QPointF* pos;
};

#endif // WAYPOINT_H
