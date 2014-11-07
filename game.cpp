#include "game.h"
#include "waypoint.h"
#include "enemy.h"

#include <QPainter>
#include <QDebug>

Game::Game(QWidget *parent) : QWidget(parent)
{
    loadWaypoints();
    generateEnemy();
    timerId = startTimer(10);
}

Game::~Game()
{
    for(auto& w : waypoints)
        delete w;
    for(auto& e : enemies)
        delete e;
}

void Game::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    for(auto& e : enemies)
        painter.drawImage(*e->getRect(), *e->getImage());
}

void Game::timerEvent(QTimerEvent *event){
    //ball->autoMove();
    //checkCollision();
    moveEnemies();
    repaint();
}

void Game::loadWaypoints(){
    for(int i =0; i< CONSTANTS::WAYPOINT_COUNT*2; i+=2)
        waypoints.push_back(new Waypoint(CONSTANTS::WAYPOINTS[i],CONSTANTS::WAYPOINTS[i+1]));
}

void Game::generateEnemy(){
    enemies.push_back(new Enemy(waypoints[0]->getPos()));
}

void Game::moveEnemies(){
    for(auto& e : enemies){
        if(e->getRect()->contains(waypoints[CONSTANTS::WAYPOINT_COUNT - 1]->getPos().toPoint()))
            break;
        if(e->getRect()->contains(waypoints[e->getCurWaypoint()+1]->getPos().toPoint()))
        {
            e->incrementCurWaypoint();
            qDebug() << "current waypoint is now " << e->getCurWaypoint();
        }
        e->move(waypoints[e->getCurWaypoint()+1]->getPos());
    }
}
