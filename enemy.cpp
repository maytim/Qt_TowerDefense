/*
    @mainpage HW9
    @author Tim Maytom (104016902)
    @date 12/10/2014
    @section DESCRIPTION

    This is my last update for the Tower Defense Game.

    Feature List:
        -Dynamically generated Text Images
        -Dynamically generated tile map
        -Random enemy spawner
        -Tower class upgrades
        -Formula based costs and stats for towers
        -Infinite waves with increasing difficulty
*/

#include "enemy.h"

/*
    The Enemy constructor. That spawns the enemy at a given location
    @param QPointF p the spawn coordinates
*/
Enemy::Enemy(Enemy_Type type, QPointF p) : GameObject(), currentWaypoint(0),
     dead(false), spawnDelay(2000), faceRight(false)
{
    //set unique variables depending on the enemy type
    if(type == Enemy_Type::NORMAL){
        leftAnimation = new Image(ENEMY::NORMAL_L);
        rightAnimation = new Image(ENEMY::NORMAL_R);
        health = 3;
        score = 10;
    }
    else if(type == Enemy_Type::BADASS){
        leftAnimation = new Image(ENEMY::BADASS_L);
        rightAnimation = new Image(ENEMY::BADASS_R);
        health = 10;
        score = 15;
    }
    else if(type == Enemy_Type::BAT){
        leftAnimation = new Image(ENEMY::BAT_L);
        rightAnimation = new Image(ENEMY::BAT_R);
        health = 15;
        score = 20;
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

/*
    Helper Function for moving the enemy left that also changes the Enemy image.
    @param int& x the variable that is used to update the enemy's position.
*/
void Enemy::moveLeft(int& x){
    x=-1;
    faceRight = false;
    setImage(*leftAnimation->getImage());
}

/*
    Helper Function for moving the enemy right that also changes the Enemy image.
    @param int& x the variable that is used to update the enemy's position.
*/
void Enemy::moveRight(int& x){
    x=1;
    faceRight = true;
    setImage(*rightAnimation->getImage());
}

/*
    Animation getter that returns either the left or right image of the enemy
*/
Image Enemy::getAnimation() const{
    if(faceRight)
        return *rightAnimation;
    else
        return *leftAnimation;
}
