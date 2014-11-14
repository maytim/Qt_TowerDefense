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
#include "button.h"
#include <QImage>

/*
    The Button constructor
    @param filePath is the file path to the passive button image
    @param h_filePath is the file path to the active button image
    @param scale is the factor that the image will be scaled by
*/
Button::Button(QString filePath, QString h_filePath, qreal scale) : Image(filePath, scale) , active(false)
{
    //Mimic the scaling of an image used in GameObject for the 2nd image
    QImage* raw = new QImage(h_filePath);
    activeImage = new QImage(raw->scaled(raw->width()/scale, raw->height()/scale, Qt::KeepAspectRatio));
}
