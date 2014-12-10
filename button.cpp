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

#include "button.h"
#include <QImage>

/*
    The Button constructor
    @param filePath is the file path to the passive button image
    @param h_filePath is the file path to the active button image
    @param scale is the factor that the image will be scaled by
*/
Button::Button(QString filePath, QString h_filePath, qreal scale): Image(filePath, scale), active(false)
{
    //Mimic the scaling of an image used in GameObject for the 2nd image
    QImage* raw = new QImage(h_filePath);
    activeImage = new QImage(raw->scaled(raw->width()/scale, raw->height()/scale, Qt::KeepAspectRatio));
}

/*
    The Button constructor
    @param passive the Image object for the passive attribute
    @param active the Image object for the active attribute
*/
Button::Button(Image* passive, Image* active) : Image(*passive), activeImage(active->getImage()), active(false){}

/*
    The Button Destructor
*/
Button::~Button(){
    delete activeImage;
}
