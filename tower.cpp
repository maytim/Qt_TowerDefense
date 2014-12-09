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
#include "tower.h"

#include <QRect>
#include <QApplication>

//Create a tower with default stats for it's damage and range. Move it to the tile that it was created over
Tower::Tower(QString fileName, QRect tile) : GameObject(fileName) , coolDown(false){
    if(fileName == TOWER::TOWER_FIRE)
        type = FIRE;
    else if(fileName == TOWER::TOWER_ICE)
        type = ICE;
    else if(fileName == TOWER::TOWER_EARTH)
        type = EARTH;

    getRect()->moveTo(tile.topLeft());
}

Tower::~Tower(){
}

Tower::TowerStats Tower::fire = Tower::TowerStats(500,1,40,10);
Tower::TowerStats Tower::ice = Tower::TowerStats(1000,3,40,10);
Tower::TowerStats Tower::earth = Tower::TowerStats(3000,5,60,10);

int Tower::getDamage() const{
    switch(type){
        case FIRE:
            return fire.strength;
            break;
        case ICE:
            return ice.strength;
            break;
        case EARTH:
            return earth.strength;
            break;
    }
}

int Tower::getRange() const{
    switch(type){
        case FIRE:
            return fire.range;
            break;
        case ICE:
            return ice.range;
            break;
        case EARTH:
            return earth.range;
            break;
    }
}

int Tower::getCoolDown() const{
    switch(type){
        case FIRE:
            return fire.speed;
            break;
        case ICE:
            return ice.speed;
            break;
        case EARTH:
            return earth.speed;
            break;
    }
}

int Tower::getCost() const{
    switch(type){
        case FIRE:
            return fire.cost;
            break;
        case ICE:
            return ice.cost;
            break;
        case EARTH:
            return earth.cost;
            break;
    }
}

void Tower::upgradeDamage(Type t, int change){
    switch(t){
        case FIRE:
            fire.strength += change;
            break;
        case ICE:
            ice.strength += change;
            break;
        case EARTH:
            earth.strength += change;
            break;
    }
}

void Tower::upgradeRange(Type t, int change){
    switch(t){
        case FIRE:
            fire.range += change;
            break;
        case ICE:
            ice.range += change;
            break;
        case EARTH:
            earth.range += change;
            break;
    }
}

void Tower::upgradeSpeed(Type t, int change){
    switch(t){
        case FIRE:
            fire.speed -= change;
            break;
        case ICE:
            ice.speed -= change;
            break;
        case EARTH:
            earth.speed -= change;
            break;
    }
}

int Tower::readDamage(Type t){
    switch(t){
        case FIRE:
            return fire.strength;
            break;
        case ICE:
            return ice.strength;
            break;
        case EARTH:
            return earth.strength;
            break;
    }
}

int Tower::readRange(Type t){
    switch(t){
        case FIRE:
            return fire.range;
            break;
        case ICE:
            return ice.range;
            break;
        case EARTH:
            return earth.range;
            break;
    }
}

int Tower::readRate(Type t){
    switch(t){
        case FIRE:
            return fire.speed;
            break;
        case ICE:
            return ice.speed;
            break;
        case EARTH:
            return earth.speed;
            break;
    }
}
