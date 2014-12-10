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


/*
    GameObject destructor
*/
GameObject::~GameObject(){
    delete image;
    delete rect;
}
