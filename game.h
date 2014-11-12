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
#ifndef GAME_H
#define GAME_H

#include "waypoint.h"
#include "enemy.h"
#include "tile.h"
#include "image.h"
#include "button.h"
#include "tower.h"
#include <QWidget>

//Enum for all of the different game states
enum State {MENU, INGAME, PAUSED, HELP};

/*
    @class Game
    @brief Class that manages the entire game.
    @detail This class will controll and manage all of the components that make up the game.
*/
class Game : public QWidget
{
    Q_OBJECT

public:
    //Constructor and destructor
    Game(QWidget *parent = 0);
    ~Game();

    //Event functions
    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    //Helper functions to control the game
    void newGame();
    void clearGame();
    void loadWaypoints();
    void generateEnemy();
    void moveEnemies();
    void buildMap();
    void selectTile(Tile*);
    void raycast();
    void cleanEnemyList();

    //Score and wave getters
    int getWave() const { return wave_value; }
    int getScore() const { return score_value; }

    //Updates for wave and score
    void updateWave(){ wave_value++; }
    void updateScore(int v) { score_value += v; }

private:
    //Functions to setup the game states' componenets
    void loadMenu();
    void loadHelp();
    void loadPause();
    void loadInGame();

    //Functons to delete the game states' components
    void cleanMenu();
    void cleanHelp();
    void cleanPause();
    void cleanInGame();

    void paintNum(int,QPainter&,int,int);

    //Game Properties
    int wave_value;
    int score_value;

    //The QTimer identifier
    int timerId;
    int collisionTimer;
    int moveTimer;

    //Current game state
    State state;

    //Containers for the waypoints and enemies
    std::vector<Waypoint*> waypoints;
    std::vector<Enemy*> enemies;
    std::vector<Tile*> map;
    std::vector<Tower*> towers;

    //Menu components
    Image* title_line1;
    Image* title_line2;
    Button* start_button;
    Button* help_button;
    Button* quit_button;

    //Ingame components
    Image* wave_visual;
    Image* wave_title;
    Image* score_visual;
    Image* score_title;
    Image* tileHighlight;
    std::vector<Image*> numChars;
    std::vector<Image*> towerOptions;
    int curTowerOpt;
    Image* towerOptHighlight;

    //Pause components
    std::vector<Button*> pauseButtons;

    //Help components
    std::vector<Button*> arrows;
    std::vector<Image*> helpImages;

    //Index to navigate the help images
    int helpIndex;
};

#endif // GAME_H
