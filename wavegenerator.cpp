#include "wavegenerator.h"
#include <random>
#include <chrono>
#include <cmath>
#include <QDebug>

void WaveGenerator::clearSpawnList(){
    //Don't delete the objects because they will be passed over to the game
    //enemy list
    spawnList.clear();
}

std::vector<Enemy*> WaveGenerator::generateSpawnList(int wave, QPointF spawnLocation){
    //Start by clearing spawnList
    clearSpawnList();

    //Allocate spawn tokens based on current wave. It increases every 5 waves
    int spawnTokens = std::ceil(wave * 0.2) * 10;

    //Generate an int distribution for each of the enemy types
    std::uniform_int_distribution<int> unif(0,1);

    //randomly generate enemies
    do{
        int token = unif(generator);
        switch( token ){
            case ENEMY::Normal:
                if(spawnTokens >= 1){
                    qDebug() << "Normal Spawn";
                    spawnList.push_back(new Enemy(spawnLocation));
                    spawnTokens -= 1;
                }
                break;
            case ENEMY::Badass:
                if(spawnTokens >= 3){
                    qDebug() << "Badass Spawn";
                    spawnList.push_back(new Enemy(spawnLocation));
                    spawnTokens -= 3;
                }
                break;
        }
    }while(spawnTokens > 0);
    return spawnList;
}
