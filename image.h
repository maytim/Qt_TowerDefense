#ifndef IMAGE_H
#define IMAGE_H

#include "gameobject.h"

class Image : public GameObject
{
public:
    Image(QString filePath) : GameObject(filePath) {}
    Image(QString filePath, qreal scale) : GameObject(filePath, scale) {}

    virtual bool isActive() const { return false; }
    virtual void setActive(bool) {}
    //virtual QImage getActiveImage() const =0;
};

#endif // IMAGE_H
