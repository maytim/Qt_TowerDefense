#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "image.h"


class Animation
{
public:
    Animation() : currentFrame(0), frames_count(0), finished(false), timerId(-1){

    }
    void addFrame(Image f, int l){frames.push_back(f); frameLengths.push_back(l); frames_count++;}
    Image getCurrentFrame() const { return frames[currentFrame]; }
    void moveCurrentFrame(QPointF p) {
        frames[currentFrame].getRect()->moveTo(p.rx()-frames[currentFrame].getRect()->width()/2,
                                              p.ry()-frames[currentFrame].getRect()->height()/2);
    }

    int getCurrentFrameLength() const { return frameLengths[currentFrame]; }
    bool increment() { (++currentFrame == frames_count) ? finished = true : finished = false; return finished; }
    bool isFinished() const { return finished; }
    bool checkTimerId(int id) const { return (id == timerId); }
    int getTimer() const { return timerId; }
    void setTimer(int timer) { timerId = timer; }
private:
    std::vector<Image> frames;
    std::vector<int> frameLengths;
    int currentFrame;
    int frames_count;
    int timerId;
    bool finished;
};

#endif // ANIMATION_H
