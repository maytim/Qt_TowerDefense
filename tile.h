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
#ifndef TILE_H
#define TILE_H

#include "gameobject.h"

/*
    @class Tile
    @brief Class that stores individual tile information.
    @detail This class is used to hold the Images that represent the game tiles of the map and has additional
    properties to indicate if the tile is a path tile, occupied by a tower, or currently active.
*/
class Tile : public GameObject
{
public:
    //Constructor
    Tile(QString fileName, int id=0) : GameObject(fileName) , occupied(false) , active(false) , path_id(id){ (path_id==0) ? path = false : path = true; }
    //Getters
    bool isActive() const { return active; }
    bool isOccupied() const { return occupied; }
    bool isPath() const { return path; }
    int getPathID() const { return path_id; }

    //Setters
    void setActive(bool b) { active = b; }
    void setOccupied(bool b)  { occupied = b; }
private:
    //Properties
    bool path;
    bool occupied;
    bool active;
    int path_id;
};

#endif // TILE_H
