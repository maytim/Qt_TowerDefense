#include "image.h"

Image* Image::scaledCopy(double scale){
    Image* copy = new Image(fpath, scale);
    return copy;
}
