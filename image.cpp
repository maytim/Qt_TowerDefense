#include "image.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

Image::Image(const Image& i) : fpath(i.fpath) {
    setRect(i.getRectV());
    setImage(i.getImageV());
}

Image* Image::scaledCopy(double scale){
    Image* copy = new Image(fpath, scale);
    return copy;
}

void Image::append(Image *i){
    //Resize as a new image
    if(getRect()->width() == 0){
        setImage(*i->getImage());
        setRect(getImage()->rect());
    }
    else{
        QImage* image = new QImage(getImage()->width()+i->getImage()->width(),
                                 getImage()->height(),
                                 QImage::Format_ARGB32_Premultiplied);
        image->fill(qRgba(0,0,0,0));
        QPainter painter;
        painter.begin(image);
        painter.drawImage(0,0,*getImage());
        painter.drawImage(getImage()->width(),0,*i->getImage());
        painter.end();
        delete getImage();
        setImage(*image);
        setRect(image->rect());
    }
}
