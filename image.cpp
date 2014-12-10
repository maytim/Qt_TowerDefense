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
#include "image.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

/*
    Copy Constructor
    @param i Image to be copied
*/
Image::Image(const Image& i) : fpath(i.fpath) {
    setRect(i.getRectV());
    setImage(i.getImageV());
}

/*
    Function to rescale an existing Image
    @param scale the desired scale
*/
Image* Image::scaledCopy(double scale){
    Image* copy = new Image(fpath, scale);
    return copy;
}

/*
    Function to append another Image to the current one
    @param i the image that will be appended
*/
void Image::append(Image *i){
    //Check if the Image is empty
    if(getRect()->width() == 0){
        //Set the inputed Image to this image
        setImage(*i->getImage());
        setRect(getImage()->rect());
    }
    else{
        //Create a resize Image
        QImage* image = new QImage(getImage()->width()+i->getImage()->width(),
                                 getImage()->height(),
                                 QImage::Format_ARGB32_Premultiplied);
        image->fill(qRgba(0,0,0,0)); //Set the Image background to transparent
        QPainter painter;
        painter.begin(image);
        painter.drawImage(0,0,*getImage()); //Add the current image to this new image
        painter.drawImage(getImage()->width(),0,*i->getImage()); //Add the second Image on the right of the first
        painter.end();
        delete getImage(); //delete the old image
        setImage(*image); //replace it with the new image
        setRect(image->rect());
    }
}
