/*
    @mainpage HW5
    @author Tim Maytom (104016902)
    @date 11/7/2014
    @section DESCRIPTION

    This is the beginning of my tower defense game. I have implemented the game's GUI
    and started planning some of the game logic. The GUI is built off custom Images
    and Button's that are derived from the GameObject class. The game behavior is controlled
    by its state (MENU, INGAME, PAUSE, HELP). Each state  tells the game to paint a different
    interface. For the MENU it shows the title, start button, help button, and quit button.
    Clicking each of these buttons will take you to a new state or end the program. For
    the interface in the game I created placeholder images for the wave and score counters
    and labels for each of those counters. You can press the 'P' button while playing to
    enter the PAUSE state. While in the PAUSE state the game freezes which allows you to
    continue when you press resume or you can exit the game by pressing main menu. The
    help button starts the HELP state. This will provide the user the game instructions.
    The interface has a back button, main image, and navigation arrows. The back button
    returns you to the main menu. The navigation arrows update the image displayed. The
    images are placeholders for now but will eventually show the controls of the
    game.

    Note: the animation of the enemy in game is the beginnings of my waypoint system. I left
    it in to indicate that the user is in the INGAME state and to show that when you pause
    the game that the game is actually pausing.
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
    const bool MAP[TILE_ROW*TILE_COL] = {0,0,0,0,0,0,1,0,
                                         0,1,1,1,1,1,1,0,
                                         0,1,0,0,0,0,0,0,
                                         0,1,0,0,1,1,1,0,
                                         0,1,0,0,1,0,1,0,
                                         0,1,1,1,1,0,1,0,
                                         0,0,0,0,0,0,1,0,
                                         0,0,0,0,0,0,1,0};
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
