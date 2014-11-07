#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

class Enemy : public GameObject
{
public:
    Enemy(QPointF p);

    void move(QPointF w);

    int getCurWaypoint() const { return currentWaypoint; }
    void incrementCurWaypoint() { currentWaypoint++; }
private:
    int currentWaypoint;
};

#endif // ENEMY_H
