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
