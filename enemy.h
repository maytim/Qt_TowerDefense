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
#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"
#include "image.h"
#include "animation.h"
#include <QDebug>

/*
    @class Enemy
    @brief Class for the enemy objects.
    @detail This class will hold the properties and functions for the Enemy objects. It currently
    has basic movement functions implemented.
*/

//Types of enemies
namespace ENEMY {
    //enemy constants
    const QString NORMAL = "C:/Qt/Projects/GameProject/enemy.png";
    const QString BADASS = "C:/Qt/Projects/GameProject/enemy2.png";

    const QString NORMAL_L = "C:/Qt/Projects/GameProject/white ghost left.png";
    const QString NORMAL_R = "C:/Qt/Projects/GameProject/white ghost right.png";
    const QString BADASS_L = "C:/Qt/Projects/GameProject/red ghost left.png";
    const QString BADASS_R = "C:/Qt/Projects/GameProject/red ghost right.png";
}

class Enemy : public GameObject
{
public:
    Enemy(QString type, QPointF p);
    ~Enemy();

    void move(QPointF w);

    //Getter for the currentWaypoint
    int getCurWaypoint() const { return currentWaypoint; }
    //Function to increment the currentWaypoint
    void incrementCurWaypoint() { currentWaypoint++; }
    void inflictDamage(int d) { health -= d; }
    bool isDead() const { return dead; }
    int getHealth() const { return health; }
    void setDead(bool b) { dead = b; }
    int getScore() const { return score; }
    int getSpawnDelay() const { return spawnDelay; }
    Image getAnimation() const;
    Animation* getDmgAnimation() const { return damageAnimation; }
    void addDamageAnimation( Animation* a){ damageAnimation = new Animation(*a); hasAnimation = true; }
    void paintDamageAnimation(QPainter& p);
    void damageEvent(int dmg_amount,  Animation* dmg_anim) { addDamageAnimation(dmg_anim); inflictDamage(dmg_amount); }
    bool hasAnimation;
    void centerDmgAnimation(){damageAnimation->moveCurrentFrame(getRect()->center());}
    void resetDmgAnimation() {delete damageAnimation; damageAnimation = nullptr; hasAnimation = false;}
private:
    //index of current or last waypoint that it has touched to use for navigating the waypoints
    int currentWaypoint;
    int health;
    bool dead;
    int score;
    int spawnDelay;

    //Animation variables
    bool faceRight;
    Image* rightAnimation;
    Image* leftAnimation;

    //animation functions
    void moveLeft(int& x);
    void moveRight(int& x);

    Animation* damageAnimation;
};

#endif // ENEMY_H
