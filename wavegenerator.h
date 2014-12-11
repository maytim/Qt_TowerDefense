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
#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include<vector>
#include "enemy.h"
#include<chrono>
#include<random>

//Random generator macros
#define DEFAULT std::default_random_engine
#define SEED (unsigned int)std::chrono::system_clock::now().time_since_epoch().count()

/*
 * @class WaveGenerator
 * @description
    The wave generator is an object that the Game class will have a singe
    instance of. The Game class can request for a new wave of random enemies
    by passing its wave value and then the wave generator will return a
    spawnList. Then the Game class will send this spawnList to a spawning
    function that will handle spawn timing and Enemy type.
*/
class WaveGenerator
{
public:
    //Cosntructor
    WaveGenerator() : generator(SEED) {}

    //Function to create a new spawn list
    std::vector<Enemy*> generateSpawnList(int wave, QPointF spawnLocation);
private:
    DEFAULT generator; //random engine
    std::vector<Enemy*> spawnList; //spawn list

    void clearSpawnList(); //Function to clear the current spawn list
};

#endif // WAVEGENERATOR_H
