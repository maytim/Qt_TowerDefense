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
#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

/*
    @class Enemy
    @brief Class for the enemy objects.
    @detail This class will hold the properties and functions for the Enemy objects. It currently
    has basic movement functions implemented.
*/

//Types of enemies
enum ENEMY{Normal, Badass};

class Enemy : public GameObject
{
public:
    Enemy(QPointF p);

    void move(QPointF w);

    //Getter for the currentWaypoint
    int getCurWaypoint() const { return currentWaypoint; }
    //Function to increment the currentWaypoint
    void incrementCurWaypoint() { currentWaypoint++; }
    void inflictDamage(int d) { health -= d; }
    bool isDead() const { return dead; }
    int getHealth() const { return health; }
    void setDead(bool b) { dead = b; }
    int getScore() const { return score; }
    int getSpawnDelay() const { return spawnDelay; }
private:
    //index of current or last waypoint that it has touched to use for navigating the waypoints
    int currentWaypoint;
    int health;
    bool dead;
    int score;
    int spawnDelay;
};

#endif // ENEMY_H
