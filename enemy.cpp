#include "enemy.h"
#include <QDebug>

Enemy::Enemy(QPointF p) : GameObject(CONSTANTS::ENEMY_PATH)
{
    currentWaypoint = 0;
    getRect()->translate(p.toPoint());
}

void Enemy::move(QPointF w){
    int x=0, y=0;
    (w.rx() > getRect()->topLeft().rx()) ? x=1 : x=-1;
    (w.ry() > getRect()->topLeft().ry()) ? y=1 : y=-1;
    getRect()->translate(x, y);
}
