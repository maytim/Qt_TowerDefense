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
#include <deque>
#include <QTimer>
#include <random>

//Media file paths for ToolTip objects
namespace TOOLTIP{
    const QString BASE = "C:/Qt/Projects/GameProject/tooltip_base.png";
}

//Enum for all of the different game states
enum State {MENU, INGAME, CLEARED, PAUSED, HELP};

//Character types to use in mergeChars
enum Chars {NORMAL, ACTIVE, RED};

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
public slots:
    void moveDecals(){for(auto& d : damageDisplays)d->getRect()->translate(0,-1); if(state==INGAME)QTimer::singleShot(150,this,SLOT(moveDecals()));}
    void removeDecal(){Image* front = damageDisplays.front(); damageDisplays.pop_front(); delete front;}
    void moveEvent(){cleanEnemyList(); moveEnemies(); if(state==INGAME)QTimer::singleShot(30,this,SLOT(moveEvent()));}
    void collisionEvent(){raycast(); if(state == INGAME)QTimer::singleShot(30,this,SLOT(collisionEvent()));}
private:
    //Functions to setup the game states' componenets
    void fillCharReferences();
    void loadMenu();
    void loadHelp();
    void loadPause();
    void loadInGame();
    void buildMap();
    void createNavigationPath();

    //Functons to delete the game states' components
    void cleanCharReferences();
    void cleanMenu();
    void cleanHelp();
    void cleanPause();
    void cleanInGame();

    //Event functions
    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    //Helper functions to control the game
    void newGame();
    void clearGame();
    void selectTile(Tile*);
    void raycast();
    void moveEnemies();
    void cleanEnemyList();
    void spawner();
    void newWave();
    void startTimers();

    //Score and wave getters
    inline int getWave() const { return wave_value; }
    inline int getScore() const { return score_value; }

    //Updates for wave and score
    inline void updateWave(){ wave_value++; }
    inline void updateScore(int v) { score_value += v; }

    //A helper function to dyanically draw Images
    void paintChar(std::string,double,QPainter&,int,int,bool);
    void printChar(Image* character, double scale, QPainter& p, int& x, int& y);
    void appendChar(Image* character, double scale, Image* i);
    Image* mergeChars(std::string,double,Chars);

    //Game Properties
    int wave_value;
    int score_value;
    State state;
    QPointF navPath[CONSTANTS::PATH_TILE_COUNT];

    //The QTimer identifier
    int paintTimer;
    int spawnTimer;

    //Count for spawnTimer
    int enemyCount;

    //Object to create new waves
   WaveGenerator wave_generator;

    //Containers for the waypoints and enemies
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> spawnList;
    std::vector<Tile*> map;
    std::vector<Tower*> towers;

    //Random generator variables for the damage number objects
    DEFAULT generator;
    std::uniform_int_distribution<int> damageDisplayOffset;
    std::deque<Image*> damageDisplays; //Deque of the damage number objects

    //Menu components
    Image* title_line1;
    Image* title_line2;
    Button* start_button;
    Button* help_button;
    Button* quit_button;

    //Ingame components
    Image* wave_title;
    Image* score_title;
    Image* tileHighlight;
    Button* continue_button;
    std::vector<Image*> letterChars;
    std::vector<Image*> letterCharsAct;
    std::vector<Image*> letterCharsRed;
    std::vector<Image*> specialChars;
    std::vector<Image*> towerOptions;
    int curTowerOpt;
    Type curTowerType;
    Image* towerOptHighlight;
    std::vector<Image*> fire_upgrade;
    std::vector<Image*> upgrade_icon;
    std::vector<Image*> ice_upgrade;
    std::vector<Image*> earth_upgrade;

    //Pause components
    std::vector<Button*> pauseButtons;

    //Help components
    std::vector<Button*> arrows;
    std::vector<Image*> helpImages;

    //Index to navigate the help images
    int helpIndex;

    class ToolTip; //forward declaration of the ToolTip nested class
    //Tooltip obejcts for each set of menus
    ToolTip* tooltip;

    /*
        @class ToolTip
        A nested class to create ToolTip objects to describe buttons
    */
    class ToolTip{
    public:
        //Constructors
        ToolTip(Image* s, Image* s_u, Image*, Image* c_a);
        ToolTip(Image* c, Image* c_a);
        ~ToolTip();

        void moveTo(QPointF position); //Function to reposition the tooltip
        void paint(QPainter* p); //Function to paint the tooltip
    private:
        bool upgrade;
        Image* cost;
        Image* cost_amount;
        Image* background;
        Image* stat;
        Image* stat_upgrade;

        void resizeBackground(); //function to resize the background image for the contents
    };
};

#endif // GAME_H
