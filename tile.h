#ifndef TILE_H
#define TILE_H

#include "gameobject.h"

class Tile : public GameObject
{
public:
    Tile(QString fileName, bool path=false) : GameObject(fileName) , path(path) , occupied(false) , active(false){}
    bool isActive() const { return active; }
    bool isOccupied() const { return occupied; }
    bool isPath() const { return path; }

    void setActive(bool b) { active = b; }
    void setOccupied(bool b)  { occupied = b; }
private:
    bool path;
    bool occupied;
    bool active;
};

#endif // TILE_H
