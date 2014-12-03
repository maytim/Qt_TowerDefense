#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "image.h"

class Animation
{
public:
    Animation();
    ~Animation(){for(auto& f : frames) delete f;}
    void addFrame(Image* f){frames.push_back(f);}
private:
    std::vector<Image*> frames;
};

#endif // ANIMATION_H
