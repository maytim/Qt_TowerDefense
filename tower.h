#ifndef TOWER_H
#define TOWER_H

#include "gameobject.h"

class Tower : public GameObject
{
public:
    Tower(QString fileName, QRect tile);
    int getDamage() const { return damage; }
    int getRange() const { return range; }
private:
    int damage;
    int range;
};

#endif // TOWER_H
