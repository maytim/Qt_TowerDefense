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
#ifndef IMAGE_H
#define IMAGE_H

#include "gameobject.h"

/*
    @class Image
    @brief The basic building block for the game's GUI components.
    @detail A class that is used to display images in the GUI.
*/
class Image : public GameObject
{
public:
    //Constructors
    Image(QString filePath) : GameObject(filePath) , fpath(filePath) {}
    Image(QString filePath, qreal scale) : GameObject(filePath, scale) {}
    Image(const Image& i);
    Image(){ setRect(QRect(0,0,0,11));}

    //Function to rescale the image
    Image* scaledCopy(double scale);

    //function to append another image to the current image
    void append(Image* i);
private:
    //The file path of the image
    QString fpath;
};

#endif // IMAGE_H
