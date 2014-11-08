/*
    @mainpage HW5
    @author Tim Maytom (104016902)
    @date 11/7/2014
    @section DESCRIPTION

    This is the beginning of my tower defense game. I have implemented the game's GUI
    and started planning some of the game logic. The GUI is built off custom Images
    and Button's that are derived from the GameObject class. The game behavior is controlled
    by its state (MENU, INGAME, PAUSE, HELP). Each state  tells the game to paint a different
    interface. For the MENU it shows the title, start button, help button, and quit button.
    Clicking each of these buttons will take you to a new state or end the program. For
    the interface in the game I created placeholder images for the wave and score counters
    and labels for each of those counters. You can press the 'P' button while playing to
    enter the PAUSE state. While in the PAUSE state the game freezes which allows you to
    continue when you press resume or you can exit the game by pressing main menu. The
    help button starts the HELP state. This will provide the user the game instructions.
    The interface has a back button, main image, and navigation arrows. The back button
    returns you to the main menu. The navigation arrows update the image displayed. The
    images are placeholders for now but will eventually show the controls of the
    game.

    Note: the animation of the enemy in game is the beginnings of my waypoint system. I left
    it in to indicate that the user is in the INGAME state and to show that when you pause
    the game that the game is actually pausing.
*/
#include "enemy.h"

/*
    The Enemy constructor. That spawns the enemy at a given location
    @param QPointF p the spawn coordinates
*/
Enemy::Enemy(QPointF p) : GameObject(CONSTANTS::ENEMY_PATH)
{
    //set the currentWaypoint to the first waypoint or the 'spawn' site
    currentWaypoint = 0;
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
    (w.rx() > getRect()->topLeft().rx()) ? x=1 : x=-1;
    //if the waypoint is above/below the enemy then change its y direction
    (w.ry() > getRect()->topLeft().ry()) ? y=1 : y=-1;
    //finally move the enemy one step towards its target waypoint
    getRect()->translate(x, y);
}