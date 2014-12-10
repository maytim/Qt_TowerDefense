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
    if(fileName == TOWER::TOWER_FIRE){
        type = FIRE;
        fireCount++;
    }
    else if(fileName == TOWER::TOWER_ICE){
        type = ICE;
        iceCount++;
    }
    else if(fileName == TOWER::TOWER_EARTH){
        type = EARTH;
        earthCount++;
    }

    getRect()->moveTo(tile.topLeft());
}

Tower::~Tower(){
}

Tower::TowerStats Tower::fire = Tower::TowerStats();
Tower::TowerStats Tower::ice = Tower::TowerStats();
Tower::TowerStats Tower::earth = Tower::TowerStats();
int Tower::fireCount = 0;
int Tower::iceCount = 0;
int Tower::earthCount = 0;

void Tower::resetUpgrades(){
    fireCount = 0;
    iceCount = 0;
    earthCount = 0;
}

int Tower::getDamage(Type t){
    switch(t){
        case FIRE:
            return (1+fire.d_count);
            break;
        case ICE:
            return (3+ice.d_count);
            break;
        case EARTH:
            return (5+earth.d_count);
            break;
    }
}

int Tower::getRange(Type t){
    switch(t){
        case FIRE:
            return (40+fire.r_count*10);
            break;
        case ICE:
            return (40+ice.r_count*10);
            break;
        case EARTH:
            return (60+earth.r_count*10);
            break;
    }
}

int Tower::getCoolDown(Type t){
    switch(t){
        case FIRE:
            return (500-fire.s_count*10);
            break;
        case ICE:
            return (1000-ice.s_count*10);
            break;
        case EARTH:
            return (2500-earth.s_count*10);
            break;
    }
}

int Tower::getDamageCost(Type t){
    switch(t){
        case FIRE:
            return (25 + Tower::fireCount*5 + fire.d_count*10);
            break;
        case ICE:
            return (25 + Tower::iceCount*5 + ice.d_count*10);
            break;
        case EARTH:
            return (25 + Tower::earthCount*5 + earth.d_count*10);
            break;
    }
}

int Tower::getRangeCost(Type t){
    switch(t){
        case FIRE:
            return (10 + Tower::fireCount*5 + fire.r_count*10);
            break;
        case ICE:
            return (10 + Tower::iceCount*5 + ice.r_count*10);
            break;
        case EARTH:
            return (10 + Tower::earthCount*5 + earth.r_count*10);
            break;
    }
}

int Tower::getRateCost(Type t){
    switch(t){
        case FIRE:
            return (50 + Tower::fireCount*5 + fire.d_count*5);
            break;
        case ICE:
            return (50 + Tower::iceCount*5 + ice.d_count*5);
            break;
        case EARTH:
            return (50 + Tower::earthCount*5 + earth.d_count*5);
            break;
    }
}

void Tower::upgradeDamage(Type t, int change){
    switch(t){
        case FIRE:
            fire.d_count++;
            break;
        case ICE:
            ice.d_count++;
            break;
        case EARTH:
            earth.d_count++;
            break;
    }
}

void Tower::upgradeRange(Type t, int change){
    switch(t){
        case FIRE:
            fire.r_count++;
            break;
        case ICE:
            ice.r_count++;
            break;
        case EARTH:
            earth.r_count++;
            break;
    }
}

void Tower::upgradeSpeed(Type t, int change){
    switch(t){
        case FIRE:
            fire.s_count++;
            break;
        case ICE:
            ice.s_count++;
            break;
        case EARTH:
            earth.s_count++;
            break;
    }
}
