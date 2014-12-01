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
#ifndef GAME_H
#define GAME_H

#include "waypoint.h"
#include "enemy.h"
#include "tile.h"
#include "image.h"
#include "button.h"
#include "tower.h"
#include "wavegenerator.h"
#include <QWidget>

//Enum for all of the different game states
enum State {MENU, INGAME, CLEARED, PAUSED, HELP};

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
    void createNavigationPath();

    //Score and wave getters
    int getWave() const { return wave_value; }
    int getScore() const { return score_value; }

    //Updates for wave and score
    void updateWave(){ wave_value++; }
    void updateScore(int v) { score_value += v; }

    QPointF navPath[CONSTANTS::PATH_TILE_COUNT];
private:
    //Functions to setup the game states' componenets
    void fillCharReferences();
    void loadMenu();
    void loadHelp();
    void loadPause();
    void loadInGame();

    //Functons to delete the game states' components
    void cleanCharReferences();
    void cleanMenu();
    void cleanHelp();
    void cleanPause();
    void cleanInGame();

    //A helper function to draw the scores using Images
    void paintChar(std::string,double,QPainter&,int,int,bool);
    void printChar(Image* character, double scale, QPainter& p, int& x, int& y);
    Image* mergeChars(std::string,double,bool);
    void appendChar(Image* character, double scale, Image* i);

    //Spawning function
    void spawner();

    void newWave();

    WaveGenerator wave_generator;

    //Game Properties
    int wave_value;
    int score_value;

    //The QTimer identifier
    int timerId;
    int collisionTimer;
    int moveTimer;
    int spawnTimer;

    //Count for spawnTimer
    int enemyCount;

    //Current game state
    State state;

    //Containers for the waypoints and enemies
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> spawnList;
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
    std::vector<Image*> letterChars;
    std::vector<Image*> letterCharsAct;
    std::vector<Image*> specialChars;
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


    //Temp
    Image* temp_start;
};

#endif // GAME_H
