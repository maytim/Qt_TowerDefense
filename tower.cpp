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
#include "tower.h"

#include <QRect>
#include <QApplication>

//Initialize all of the static members
Tower::TowerStats Tower::fire = Tower::TowerStats();
Tower::TowerStats Tower::ice = Tower::TowerStats();
Tower::TowerStats Tower::earth = Tower::TowerStats();
int Tower::fireCount = 0;
int Tower::iceCount = 0;
int Tower::earthCount = 0;

/*
    Constructor
    @param fileName the file path to the tower image
    @param tile the location to build the tower
*/
Tower::Tower(QString fileName, QRect tile) : GameObject(fileName) , coolDown(false){
    //Update the tower type counters
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

    getRect()->moveTo(tile.topLeft()); //Move the tower to the tile location
}

/*
    Function to reset the tower type counts
*/
void Tower::resetUpgrades(){
    fireCount = 0;
    iceCount = 0;
    earthCount = 0;

    fire.d_count = 0;
    fire.r_count = 0;
    fire.s_count = 0;

    ice.d_count = 0;
    ice.r_count = 0;
    ice.s_count = 0;

    earth.d_count = 0;
    earth.r_count = 0;
    earth.s_count = 0;
}

/*
    Tower type damage getter
*/
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

/*
    Tower type range getter
*/
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

/*
    Tower type cool down getter
*/
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

/*
    Tower type cost getter
*/
int Tower::getCost(Type t){
    switch(t){
        case FIRE:
            return (10 + Tower::fireCount*5);
            break;
        case ICE:
            return (15 + Tower::iceCount*5);
            break;
        case EARTH:
            return (20 + Tower::earthCount*5);
            break;
    }
}

/*
    Tower type damage upgrade cost getter
*/
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

/*
    Tower type range upgrade cost getter
*/
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

/*
    Tower type cool down upgrade cost getter
*/
int Tower::getCoolDownCost(Type t){
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

/*
    Tower type damage upgrade function
*/
void Tower::upgradeDamage(Type t){
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

/*
    Tower type range upgrade function
*/
void Tower::upgradeRange(Type t){
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

/*
    Tower type damage cool down function
*/
void Tower::upgradeCoolDown(Type t){
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
