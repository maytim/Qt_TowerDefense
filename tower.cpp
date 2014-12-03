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
#include "tower.h"

#include <QRect>
#include <QApplication>

//Create a tower with default stats for it's damage and range. Move it to the tile that it was created over
Tower::Tower(QString fileName, QRect tile) : GameObject(fileName) , cost(10), damage(1) , range(40) , coolDown(false) , coolDownTime(1000){
    getRect()->moveTo(tile.topLeft());
    animation.addFrame(new Image(TOWER::FIRE_FRAME_1,0.5));
    animation.addFrame(new Image(TOWER::FIRE_FRAME_2,0.5));
}

Tower::~Tower(){
}
