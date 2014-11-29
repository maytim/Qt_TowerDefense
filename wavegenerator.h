#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include<vector>
#include "enemy.h"
#include<chrono>
#include<random>

#define DEFAULT std::default_random_engine
#define SEED (unsigned int)std::chrono::system_clock::now().time_since_epoch().count()

/*
    The wave generator is an object that the Game class will have a singe
    instance of. The Game class can request for a new wave of random enemies
    by passing its wave value and then the wave generator will return a
    spawnList. Then the Game class will send this spawnList to a spawning
    function that will handle spawn timing and Enemy type.
*/
class WaveGenerator
{
public:
    WaveGenerator() : generator(SEED) {}
    std::vector<Enemy*> generateSpawnList(int wave, QPointF spawnLocation);
private:
    DEFAULT generator;
    std::vector<Enemy*> spawnList;

    void clearSpawnList();
};

#endif // WAVEGENERATOR_H
