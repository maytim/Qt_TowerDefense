/*
    The GameObject class is used to store the QImage and QRect data that represents the object within the game.
    All other objects within the game will build functionality off this framework.
*/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QImage>
#include <QRect>
#include <QString>

//Constants used throughout the application
namespace CONSTANTS{
    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 400;
    
    //waypoint constants
    const int WAYPOINT_COUNT = 4;
    const int WAYPOINTS[2*WAYPOINT_COUNT] = {1,1,200,1,200,200,1,200};
    
    //menu constants
    const QString TITLE_PATH = "C:/Qt/Projects/GameProject/title2.png";
    const QString START_PATH = "C:/Qt/Projects/GameProject/start.png";
    const QString START_H_PATH = "C:/Qt/Projects/GameProject/start_hover.png";
    const QString HELP_PATH = "C:/Qt/Projects/GameProject/help.png";
    const QString HELP_H_PATH = "C:/Qt/Projects/GameProject/help_hover.png";
    const int MARGIN_TOP = 64;

    //enemy constants
    const QString ENEMY_PATH = "C:/Qt/Projects/GameProject/enemy.png";
}

class GameObject
{
public:
    //Constructors and Destructors
    GameObject();
    GameObject(int x, int y, int w, int h);
    GameObject(QString, qreal=1);
    virtual ~GameObject();

    //rect and image get functions
    QRect* getRect(){ return rect; }
    QImage* getImage(){ return image; }
private:
    QImage* image;
    QRect* rect;
};

#endif // GAMEOBJECT_H
