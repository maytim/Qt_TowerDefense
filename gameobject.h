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
    const int WAYPOINT_COUNT = 8;
    const int WAYPOINTS[2*WAYPOINT_COUNT] = {258,50,258,114,82,114,98,242,210,226,194,146,274,162,258,306};
    
    //GUI constants
    const QString TITLE_PATH_1 = "C:/Qt/Projects/GameProject/tower.png";
    const QString TITLE_PATH_2 = "C:/Qt/Projects/GameProject/defense.png";
    const QString START_PATH = "C:/Qt/Projects/GameProject/start.png";
    const QString START_H_PATH = "C:/Qt/Projects/GameProject/start_hover.png";
    const QString HELP_PATH = "C:/Qt/Projects/GameProject/help.png";
    const QString HELP_H_PATH = "C:/Qt/Projects/GameProject/help_hover.png";
    const QString SCORE_PATH = "C:/Qt/Projects/GameProject/score.png";
    const QString MAINMENU_PATH = "C:/Qt/Projects/GameProject/mainmenu.png";
    const QString MAINMENU_H_PATH = "C:/Qt/Projects/GameProject/mainmenu_hover.png";
    const QString QUIT_PATH = "C:/Qt/Projects/GameProject/quit.png";
    const QString QUIT_H_PATH = "C:/Qt/Projects/GameProject/quit_hover.png";
    const QString RESUME_PATH = "C:/Qt/Projects/GameProject/resume.png";
    const QString RESUME_H_PATH = "C:/Qt/Projects/GameProject/resume_hover.png";
    const QString RIGHT_PATH = "C:/Qt/Projects/GameProject/rightarrow.png";
    const QString RIGHT_H_PATH = "C:/Qt/Projects/GameProject/rightarrow_hover.png";
    const QString LEFT_PATH = "C:/Qt/Projects/GameProject/leftarrow.png";
    const QString LEFT_H_PATH = "C:/Qt/Projects/GameProject/leftarrow_hover.png";
    const QString BACK_PATH = "C:/Qt/Projects/GameProject/back.png";
    const QString BACK_H_PATH = "C:/Qt/Projects/GameProject/back_hover.png";
    const QString SCORE_TITLE_PATH = "C:/Qt/Projects/GameProject/score_title.png";
    const QString WAVE_TITLE_PATH = "C:/Qt/Projects/GameProject/wave_title.png";
    const QString HELP_IMAGE_1 = "C:/Qt/Projects/GameProject/help1.png";
    const QString HELP_IMAGE_2 = "C:/Qt/Projects/GameProject/help2.png";
    const QString HELP_IMAGE_3 = "C:/Qt/Projects/GameProject/help3.png";
    const QString HELP_IMAGE_4 = "C:/Qt/Projects/GameProject/help4.png";
    const QString HELP_IMAGE_5 = "C:/Qt/Projects/GameProject/help5.png";
    const QString HELP_IMAGE_6 = "C:/Qt/Projects/GameProject/help6.png";
    const QString DIRT_TILE = "C:/Qt/Projects/GameProject/dirt_tile.png";
    const QString GRASS_TILE = "C:/Qt/Projects/GameProject/grass_tile.png";
    const QString HIGHLIGHT_TILE = "C:/Qt/Projects/GameProject/tile_highlight.png";
    const QString TOWER_FIRE = "C:/Qt/Projects/GameProject/fire.png";
    const QString TOWER_ICE = "C:/Qt/Projects/GameProject/ice.png";
    const QString TOWER_EARTH = "C:/Qt/Projects/GameProject/rock.png";
    const QString TOWEROPT_H = "C:/Qt/Projects/GameProject/toweroption_h.png";
    const QString CHAR_0 = "C:/Qt/Projects/GameProject/characters/0.png";
    const QString CHAR_1 = "C:/Qt/Projects/GameProject/characters/1.png";
    const QString CHAR_2 = "C:/Qt/Projects/GameProject/characters/2.png";
    const QString CHAR_3 = "C:/Qt/Projects/GameProject/characters/3.png";
    const QString CHAR_4 = "C:/Qt/Projects/GameProject/characters/4.png";
    const QString CHAR_5 = "C:/Qt/Projects/GameProject/characters/5.png";
    const QString CHAR_6 = "C:/Qt/Projects/GameProject/characters/6.png";
    const QString CHAR_7 = "C:/Qt/Projects/GameProject/characters/7.png";
    const QString CHAR_8 = "C:/Qt/Projects/GameProject/characters/8.png";
    const QString CHAR_9 = "C:/Qt/Projects/GameProject/characters/9.png";
    const int MARGIN_TOP = 64;

    //enemy constants
    const QString ENEMY_PATH = "C:/Qt/Projects/GameProject/enemy.png";

    //map constants
    const int TILE_ROW = 8;
    const int TILE_COL = 8;
    const int PATH_TILE_COUNT = 22;
    const int MAP[TILE_ROW*TILE_COL] = { 0, 0, 0, 0, 0, 0, 1, 0,
                                         0, 7, 6, 5, 4, 3, 2, 0,
                                         0, 8, 0, 0, 0, 0, 0, 0,
                                         0, 9, 0, 0,16,17,18, 0,
                                         0,10, 0, 0,15, 0,19, 0,
                                         0,11,12,13,14, 0,20, 0,
                                         0, 0, 0, 0, 0, 0,21, 0,
                                         0, 0, 0, 0, 0, 0,22, 0};
    const int TOWER_COST = 10;
}

/*
    @class GameObject
    @brief The basic building block for the games components.
    @detail A base class for all components in the game world. It uses a QImage and QRect
    to represent the objects within the game.
*/
class GameObject
{
public:
    //Constructors and Destructors
    GameObject();
    GameObject(QString, qreal=1);
    virtual ~GameObject();

    //rect and image getter functions
    QRect* getRect(){ return rect; }
    QImage getImage() const { return *image; }
private:
    //properties
    QImage* image;
    QRect* rect;
};

#endif // GAMEOBJECT_H
