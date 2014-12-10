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
#ifndef BUTTON_H
#define BUTTON_H

#include "image.h"

/*
    @class Button
    @brief Custom button class.
    @detail The Button class is an Image with the ability to have an active and passive image.
    This allows me to create a hover effect when the Button is active.
*/
class Button : public Image
{
public:
    //Constructor
    Button(QString filePath, QString h_filePath, qreal scale);
    Button(Image* passive, Image* active);
    ~Button();

    //Setter for the active property
    inline void setActive(bool a){ active = a; }

    //Getter for the active property
    inline bool isActive() const { return active; }

    //getter for the active image
    inline QImage getActiveImage() const { return *activeImage; }
private:
    //Hover properties
    QImage* activeImage;
    bool active;
};

#endif // BUTTON_H
