#include "tower.h"

#include <QRect>

Tower::Tower(QString fileName, QRect tile) : GameObject(fileName) , damage(1) , range(40){
    getRect()->moveTo(tile.topLeft());
}
