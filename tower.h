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
#ifndef TOWER_H
#define TOWER_H

#include "gameobject.h"
#include <QPointF>

/*
    @class Tower
    @brief Class that manages individual tower information in the game.
    @detail This class holds the towers Image and stats which are relevant to attacking events.
*/
class Tower : public GameObject
{
public:
    //Constructor
    Tower(QString fileName, QRect tile);
    //Getters
    int getDamage() const { return damage; }
    int getRange() const { return range; }
    int getTimer() const { return timerID; }
    bool isCoolDown() const { return coolDown; }
    int getCoolDown() const { return coolDownTime; }

    void setTimer(int id) { timerID = id; }
    void setCoolDown(bool c) { coolDown = c; }
private:
    //Attacking stats
    int damage;
    int range;
    int timerID;
    bool coolDown;
    int coolDownTime;
};

#endif // TOWER_H
