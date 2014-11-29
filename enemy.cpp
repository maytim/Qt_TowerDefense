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
#include "enemy.h"

/*
    The Enemy constructor. That spawns the enemy at a given location
    @param QPointF p the spawn coordinates
*/
Enemy::Enemy(QPointF p) : GameObject(CONSTANTS::ENEMY_PATH), currentWaypoint(0),
    health(3), dead(false), score(10), spawnDelay(2000)
{
    //move the enemy object to that 'spawn' site
    getRect()->translate(p.toPoint());
}

/*
    Function to move the enemy 1 pixel at a time.
    @param QPointF w the waypoint that the enemy is moving towards
*/
void Enemy::move(QPointF w){
    //temporary direction variables
    int x=0, y=0;
    //if the waypoint is to the left/right of the enemy then change its x direction
    (w.rx() > getRect()->center().rx()) ? x=1 : x=-1;
    //if the waypoint is above/below the enemy then change its y direction
    (w.ry() > getRect()->center().ry()) ? y=1 : y=-1;
    //finally move the enemy one step towards its target waypoint
    getRect()->translate(x, y);
}
