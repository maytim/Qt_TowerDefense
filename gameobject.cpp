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
#include "gameobject.h"

//By default set the pointers of the QRect and QImage to NULL
GameObject::GameObject(){
    image = NULL;
    rect = NULL;
}

/*
    This is the constructor that I use in all the classes that are derived from GameObject. It accepts a QString
    that is a const value of the filePath for the applicable image. It also allows you to scale down the image.
    The scale value is the factor that the image is being scaled down by, so if scale==3 then the image will be 1/3
    of its original size.
*/
GameObject::GameObject(QString filePath, qreal scale)
{
    QImage* raw = new QImage(filePath);
    image = new QImage(raw->scaled(raw->width()/scale, raw->height()/scale, Qt::KeepAspectRatio));

    rect = new QRect(image->rect());
}


//Manage heap memory in the destructor
GameObject::~GameObject(){
    delete image;
    delete rect;
}
