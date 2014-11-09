#ifndef TOWER_H
#define TOWER_H

#include "gameobject.h"

class Tower : public GameObject
{
public:
    Tower(QString fileName, QRect tile);

};

#endif // TOWER_H
