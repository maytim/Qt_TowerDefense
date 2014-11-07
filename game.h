#ifndef GAME_H
#define GAME_H

#include "waypoint.h"
#include "enemy.h"

#include <QWidget>

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = 0);
    ~Game();

    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);

    void loadWaypoints();
    void generateEnemy();

    void moveEnemies();
private:
    int timerId;
    std::vector<Waypoint*> waypoints;
    std::vector<Enemy*> enemies;
};

#endif // GAME_H
