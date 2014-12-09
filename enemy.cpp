/*
    @mainpage HW6
    @author Tim Maytom (104016902)
    @date 11/14/2014
    @section DESCRIPTION

    This is an update of my previous assignment. I have worked on adding game logic to the GUI
    that I had already constructed. The game draws a game map from array data. The enemies navigation
    coordinates have been manually updated to follow the new path. I have added an ingame GUI. This
    GUI includes the wave and score displays at the top of the screen. The number images for these displays
    are drawn from a parsed string with Images that I have created. I have also added a toggle menu on the right
    to select the tower type that you want to build. The towers target enemies by drawing QLine's to the enemy
    and then comparing the distance of that QLine to its range property. If the enemy is within range, the tower
    will reduce its health. When an enemy's health reaches 0, the enemy will be deleted from the game and the score
    will be updated by the appropriate value.

    Issues:
    -no end game event
    -only a single wave. Need to store wave data, and then create a system to load the waves
    -no attacking animations
    -building towers doesn't affect the player's score so the user can create as many towers as they like
    -no tower upgrade system
*/
#include "enemy.h"
#include <QPainter>
#include <QDebug>

/*
    The Enemy constructor. That spawns the enemy at a given location
    @param QPointF p the spawn coordinates
*/
Enemy::Enemy(QString type, QPointF p) : GameObject(type), currentWaypoint(0),
     dead(false), score(10), spawnDelay(2000), faceRight(false)
{

    //set animation Images
    if(type == ENEMY::NORMAL){
        leftAnimation = new Image(ENEMY::NORMAL_L);
        rightAnimation = new Image(ENEMY::NORMAL_R);
        health = 3;
    }
    else if(type == ENEMY::BADASS){
        leftAnimation = new Image(ENEMY::BADASS_L);
        rightAnimation = new Image(ENEMY::BADASS_R);
        health = 10;
    }

    //set the animation image to be left by default
    setImage(*leftAnimation->getImage());
    setRect(*leftAnimation->getRect());

    //move the enemy object to that 'spawn' site
    getRect()->translate(p.toPoint().rx()-getRect()->width()/2, p.toPoint().ry()-getRect()->height()/2);

}

Enemy::~Enemy(){
    delete leftAnimation;
    delete rightAnimation;
}

/*
    Function to move the enemy 1 pixel at a time.
    @param QPointF w the waypoint that the enemy is moving towards
*/
void Enemy::move(QPointF w){
    //temporary direction variables
    int x=0, y=0;

    //Try to get the enemy to move x or y
    if(std::abs(w.rx() - getRect()->center().rx()) > 0.1 &&
       std::abs(w.ry() - getRect()->center().ry()) > 0.1){
        (w.rx() > getRect()->center().rx()) ? moveRight(x) : moveLeft(x);
        (w.ry() > getRect()->center().ry()) ? y=1 : y=-1;
    }
    else if(std::abs(w.rx() - getRect()->center().rx()) > 0.1)
        (w.rx() > getRect()->center().rx()) ? moveRight(x) : moveLeft(x);
    else if(std::abs(w.ry() - getRect()->center().ry()) > 0.1)
        (w.ry() > getRect()->center().ry()) ? y=1 : y=-1;

    //finally move the enemy one step towards its target waypoint
    getRect()->translate(x, y);
}

void Enemy::moveLeft(int& x){
    x=-1;
    faceRight = false;
    setImage(*leftAnimation->getImage());
}

void Enemy::moveRight(int& x){
    x=1;
    faceRight = true;
    setImage(*rightAnimation->getImage());
}

Image Enemy::getAnimation() const{
    if(faceRight)
        return *rightAnimation;
    else
        return *leftAnimation;
}
