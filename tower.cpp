#include "tower.h"

#include <QRect>

Tower::Tower(QString fileName, QRect tile) : GameObject(fileName){
    getRect()->moveTo(tile.topLeft());
}
