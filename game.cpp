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
#include "game.h"
#include "waypoint.h"
#include "enemy.h"
#include "wavegenerator.h"

#include <QApplication>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QDebug>

/*
    Default constructor.
    @brief It sets up all of the game GUI components and starts the game in the MENU state.
*/
Game::Game(QWidget *parent) : QWidget(parent) , state(MENU), helpIndex(0) , curTowerOpt(0), wave_value(1), score_value(10) , enemyCount(0)
{
    //Since we start in the Menu portion of the game we need mouse tracking
    setMouseTracking(true);

    //Load up the components for each game state
    loadMenu();
    loadHelp();
    loadPause();
    loadInGame();
}

/*
    Destructor
    @brief Manages all of the objects that I have created on the heap.
*/
Game::~Game()
{
    cleanMenu();
    cleanHelp();
    cleanPause();
    cleanInGame();
    for(auto& e : enemies)
        delete e;
}

/*
    The overridden paintEvent
    @brief for each paint event the game checks which state it's in and then draws the
    corresponding GUI components.
*/
void Game::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    switch(state){
        case MENU:{
            //Draw the title Images
            painter.drawImage(*title_line1->getRect(), title_line1->getImage());
            painter.drawImage(*title_line2->getRect(), title_line2->getImage());

            //Then for each of the buttons check to display active or passive image
            if(start_button->isActive())
                painter.drawImage(*start_button->getRect(), start_button->getActiveImage());
            else
                painter.drawImage(*start_button->getRect(), start_button->getImage());

            if(help_button->isActive())
                painter.drawImage(*help_button->getRect(), help_button->getActiveImage());
            else
                painter.drawImage(*help_button->getRect(), help_button->getImage());
            if(quit_button->isActive())
                painter.drawImage(*quit_button->getRect(), quit_button->getActiveImage());
            else
                painter.drawImage(*quit_button->getRect(), quit_button->getImage());
            break;
        }
        case INGAME:{
            //Draw the score and wave Images
            paintNum(getWave(),painter,10,10+wave_title->getRect()->height());
            painter.drawImage(*wave_title->getRect(), wave_title->getImage());
            paintNum(getScore(),painter,width()-10-score_visual->getRect()->width(), 10+score_title->getRect()->height());
            painter.drawImage(*score_title->getRect(), score_title->getImage());

            //Tower Builder Menu
            for(const auto o : towerOptions)
                painter.drawImage(*o->getRect(), o->getImage());
            painter.drawImage(*towerOptions[curTowerOpt]->getRect(), towerOptHighlight->getImage());


            //Draw the map tiles
            for(auto& t : map){
                painter.drawImage(*t->getRect(), t->getImage());
                if(t->isActive())
                    painter.drawImage(*tileHighlight->getRect(), tileHighlight->getImage());
            }

            //Draw each of the enemies
            for(auto& e : enemies){
                if(!e->isDead())
                    painter.drawImage(*e->getRect(), e->getImage());
            }

            //Draw the towers
            for(const auto t : towers)
                painter.drawImage(*t->getRect(), t->getImage());
            break;
        }
        case PAUSED:{
            //For each of the buttons check to display active or passive image
            for(const auto b : pauseButtons){
                if(b->isActive())
                    painter.drawImage(*b->getRect(), b->getActiveImage());
                else
                    painter.drawImage(*b->getRect(), b->getImage());
            }
            break;
        }
        case HELP:{
            //Draw the appropriate help image depending on the users current index
            painter.drawImage(*helpImages[helpIndex]->getRect(), helpImages[helpIndex]->getImage());

            //For each of the arrows check to display active or passive image
            for(const auto b : arrows){
                if(b->isActive())
                    painter.drawImage(*b->getRect(), b->getActiveImage());
                else
                    painter.drawImage(*b->getRect(), b->getImage());
            }
            break;
        }
    }
}

/*
    timerEvent
    @brief The timerEvent is treated as an update loop. It is called every 10
    milliseconds and will update enemy positions if ingame or repaint the game.
*/
void Game::timerEvent(QTimerEvent *event){
    //If the game is active then update the enemy positions, collision events, and spawn enemies
    if(state == INGAME){
        if(event->timerId() == moveTimer)
            moveEnemies();
        for(auto& t : towers){
            if(event->timerId()==t->getTimer()){
                killTimer(t->getTimer());
                t->setCoolDown(false);
            }
        }
        if(event->timerId() == collisionTimer){
            raycast();
            cleanEnemyList();
        }
        if(event->timerId() == spawnTimer){
            //generateEnemy();
            spawner();
        }
    }
    repaint();
}

/*
 * generateEnemy
 * @brief A function to spawn a new enemy
*/
void Game::generateEnemy(){
    killTimer(spawnTimer);
    if(enemyCount < 5){
        enemies.push_back(new Enemy(navPath[0]));
        enemyCount++;
        spawnTimer = startTimer(2000);
    }    
}

void Game::spawner(){
    //Stop the current timer
    killTimer(spawnTimer);

    //Then start spawning the new enemies
    if(!spawnList.empty()){
       // Enemy* spawn = spawnList.back();

        enemies.push_back(spawnList.back());
        enemyCount++;
        spawnTimer = startTimer(spawnList.back()->getSpawnDelay());
        spawnList.pop_back();
    }
}

/*
    moveEnemies
    @brief A function to update the enemies position
*/
void Game::moveEnemies(){
    for(auto& e : enemies){
        //If the enemy has reached the final waypoint then don't take any action
        if(e->getRect()->contains(navPath[CONSTANTS::PATH_TILE_COUNT - 1].toPoint())){
            //REACHED END
            setMouseTracking(true);
            state = MENU;
            break;
        }
        //If the enemy as reached its target waypoint then update its target waypoint
        if(e->getRect()->contains(navPath[e->getCurWaypoint()+1].toPoint()))
        {
            e->incrementCurWaypoint();
        }
        //Move the enemy towards its targeted waypoint
        e->move(navPath[e->getCurWaypoint()+1]);
    }
}

/*
    keyPressEvent
    @brief Checks for input of 'P' or 'ESC' keys while playing the game.
*/
void Game::keyPressEvent(QKeyEvent* event){
    //While playing the game
    if(state == INGAME){
        switch(event->key()){
            //The 'P' key will activate the pause state
            case Qt::Key_P:
                {
                    state = PAUSED;
                    //Enable mouse tracking so that the buttons will be interactive
                    setMouseTracking(true);
                    break;
                }
            //The 'ESC' key closes the program
            case Qt::Key_Escape:
                {
                    qApp->exit();
                    break;
                }
            default:
                QWidget::keyPressEvent(event);
        }
    }
    //Otherwise ensure that the default keyPressEvent is called
    else{
        QWidget::keyPressEvent(event);
    }
}

/*
    mouseMoveEvent
    @brief With setMouseTracking enabled, I am able to create a hover effect
    on my Buttons with this function.
*/
void Game::mouseMoveEvent(QMouseEvent *event){
    switch(state){
        case MENU:{
            //If hovering over start change start's image and make sure the others are passive
            if(start_button->getRect()->contains(event->pos())){
                start_button->setActive(true);
                help_button->setActive(false);
                quit_button->setActive(false);
            }
            //If hovering over help change help's image and make sure the others are passive
            else if(help_button->getRect()->contains(event->pos())){
                help_button->setActive(true);
                start_button->setActive(false);
                quit_button->setActive(false);
            }
            //If hovering over quit change quit's image and make sure the others are passive
            else if(quit_button->getRect()->contains(event->pos())){
                quit_button->setActive(true);
                start_button->setActive(false);
                help_button->setActive(false);
            }
            //Otherwise make sure that all the buttons are displaying passive images
            else{
                start_button->setActive(false);
                help_button->setActive(false);
                quit_button->setActive(false);
            }
            break;
        }
        case PAUSED:{
            //If hovering over resume change resume's image and make sure main menu is passive
            if(pauseButtons[0]->getRect()->contains(event->pos())){
                pauseButtons[0]->setActive(true);
                pauseButtons[1]->setActive(false);
            }
            //If hovering over main menu change main menu's image and make sure resume is passive
            else if(pauseButtons[1]->getRect()->contains(event->pos())){
                pauseButtons[0]->setActive(false);
                pauseButtons[1]->setActive(true);
            }
            //Otherwise set resume and main menu to the passive image
            else{
                pauseButtons[0]->setActive(false);
                pauseButtons[1]->setActive(false);
            }
            break;
        }
    case HELP:{
        //If hovering over left arrow change left arrow's image and make sure the others are passive
        if(arrows[0]->getRect()->contains(event->pos())){
            arrows[0]->setActive(true);
            arrows[1]->setActive(false);
            arrows[2]->setActive(false);
        }
        //If hovering over right arrow change right arrow's image and make sure the others are passive
        else if(arrows[1]->getRect()->contains(event->pos())){
            arrows[0]->setActive(false);
            arrows[1]->setActive(true);
            arrows[2]->setActive(false);
        }
        //If hovering over back change back's image and make sure the others are passive
        else if(arrows[2]->getRect()->contains(event->pos())){
            arrows[0]->setActive(false);
            arrows[1]->setActive(false);
            arrows[2]->setActive(true);
        }
        //Otherwise set all three to passive
        else{
            arrows[0]->setActive(false);
            arrows[1]->setActive(false);
            arrows[2]->setActive(false);
        }
        break;
    }
    }
    repaint();
}

/*
    mousePressEvent
    @brief This function handles all mouse clicks on Button objects.
    It checks the position of the click agains the Buttons' QRects.
*/
void Game::mousePressEvent(QMouseEvent *event){
    switch(state){
        case MENU:{
            //Pressing the start button will start the game
            if(start_button->getRect()->contains(event->pos())){
                //Start game
                state = INGAME;
                //set up a new game
                newGame();
                //disable tracking for better performance
                setMouseTracking(false);
            }
            //Pressing the help button will activate the Help state
            else if(help_button->getRect()->contains(event->pos())){
                //Open Help Window
                state = HELP;
            }
            //Pressing the quit button will end the application
            else if(quit_button->getRect()->contains(event->pos())){
                qApp->quit();
            }
            break;
        }
        case PAUSED:{
            //Pressing resume will continue the current game
            if(pauseButtons[0]->getRect()->contains(event->pos())){
                //Resume game
                setMouseTracking(false);
                state = INGAME;
            }
            //Pressing Main Menu will return the user to the main menu
            else if(pauseButtons[1]->getRect()->contains(event->pos())){
                //Return to main menu
                setMouseTracking(true);
                killTimer(timerId);
                state = MENU;
            }
            break;
        }
        case HELP:{
            //Pressing the left arrow will update the helpIndex so that it shows the image on the left
            if(arrows[0]->getRect()->contains(event->pos())){
                if(helpIndex == 0)
                    helpIndex = helpImages.size()-1;
                else
                    helpIndex--;
            }
            //Pressing the right arrow will update the helpIndex so that it shows the image on the right
            else if(arrows[1]->getRect()->contains(event->pos())){
                if(helpIndex == helpImages.size()-1)
                    helpIndex = 0;
                else
                    helpIndex++;
            }
            //Pressing back will return the user to the main menu
            else if(arrows[2]->getRect()->contains(event->pos())){
                //Reset the helpIndex so that it will reopen at the first image
                helpIndex = 0;
                //Return to main menu
                state = MENU;
            }
            break;
        }
    case INGAME:{
        //Check if the event occurred on a non-path tile that is empty. If so then highlight that tile
        for(auto& t : map)
            (!t->isPath() && !t->isOccupied() && t->getRect()->contains(event->pos())) ? selectTile(t) : t->setActive(false);

        //Check if the click event occured on one of the tower builder options and if so update the selected tower type
        for(size_t i=0; i<towerOptions.size(); i++){
            if(towerOptions[i]->getRect()->contains(event->pos()))
                curTowerOpt = i;
        }
        break;
    }
    }
}

/*
    newGame
    @brief A function to set up a new tower defense game
*/
void Game::newGame(){
    //Delete any existing data
    clearGame();
    //load new data
    spawnList = wave_generator.generateSpawnList( getWave(), navPath[0] );
    score_value = 10;
    enemyCount = 0;
    //start the timer that will call the 'update loop'
    timerId = startTimer(10);
    //Start the timer that will check for enemies within the towers range
    collisionTimer = startTimer(100);
    //Start the timer that will update the enemies positions
    moveTimer = startTimer(25);
    //Start the timer that will spawn enemies
    spawnTimer = startTimer(2000);
}

//A function to clear the existing game data
void Game::clearGame(){
    for(auto& e : enemies)
        delete e;
    enemies.clear();
    for(auto& t : towers)
        delete t;
    towers.clear();
    for(auto& t : map){
        t->setOccupied(false);
    }
    spawnList.clear();
}

//A function to load the menu components
void Game::loadMenu(){
    //load the corresponding images for each of the components
    title_line1 = new Image(CONSTANTS::TITLE_PATH_1, 0.125);
    title_line2 = new Image(CONSTANTS::TITLE_PATH_2, 0.125);
    start_button = new Button(CONSTANTS::START_PATH, CONSTANTS::START_H_PATH, 0.25);
    help_button = new Button(CONSTANTS::HELP_PATH, CONSTANTS::HELP_H_PATH, 0.25);
    quit_button = new Button(CONSTANTS::QUIT_PATH, CONSTANTS::QUIT_H_PATH, 0.25);

    //position the components
    title_line1->getRect()->translate( (width()-title_line1->getRect()->width())/2 , CONSTANTS::MARGIN_TOP );
    title_line2->getRect()->translate( (width()-title_line2->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title_line1->getRect()->height());
    start_button->getRect()->translate( (width()-start_button->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title_line1->getRect()->height() + title_line2->getRect()->height());
    help_button->getRect()->translate( (width()-help_button->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title_line1->getRect()->height() + title_line2->getRect()->height() + start_button->getRect()->height());
    quit_button->getRect()->translate( (width()-quit_button->getRect()->width())/2 , CONSTANTS::MARGIN_TOP + title_line1->getRect()->height() + title_line2->getRect()->height() + start_button->getRect()->height() + help_button->getRect()->height());
}

//A function to delete the menu components
void Game::cleanMenu(){
    //Delete all of the Menu components
    delete title_line1;
    delete title_line2;
    delete start_button;
    delete help_button;
    delete quit_button;
}

//A function to load the ingame components
void Game::loadInGame(){
    //load the corresponding images for each of the components
    score_visual = new Image(CONSTANTS::SCORE_PATH, 0.5);
    score_title = new Image(CONSTANTS::SCORE_TITLE_PATH, 1);
    wave_visual = new Image(CONSTANTS::SCORE_PATH, 0.5);
    wave_title = new Image(CONSTANTS::WAVE_TITLE_PATH, 1);
    tileHighlight = new Image(CONSTANTS::HIGHLIGHT_TILE);
    towerOptions.push_back(new Image(CONSTANTS::TOWER_FIRE));
    towerOptions.push_back(new Image(CONSTANTS::TOWER_ICE));
    towerOptions.push_back(new Image(CONSTANTS::TOWER_EARTH));
    towerOptHighlight = new Image(CONSTANTS::TOWEROPT_H);

    numChars.push_back(new Image(CONSTANTS::CHAR_0));
    numChars.push_back(new Image(CONSTANTS::CHAR_1));
    numChars.push_back(new Image(CONSTANTS::CHAR_2));
    numChars.push_back(new Image(CONSTANTS::CHAR_3));
    numChars.push_back(new Image(CONSTANTS::CHAR_4));
    numChars.push_back(new Image(CONSTANTS::CHAR_5));
    numChars.push_back(new Image(CONSTANTS::CHAR_6));
    numChars.push_back(new Image(CONSTANTS::CHAR_7));
    numChars.push_back(new Image(CONSTANTS::CHAR_8));
    numChars.push_back(new Image(CONSTANTS::CHAR_9));

    //position the components
    wave_title->getRect()->translate(10,10);
    wave_visual->getRect()->translate(10,10+wave_title->getRect()->height());
    score_title->getRect()->translate(width()-10-score_visual->getRect()->width(), 10);
    score_visual->getRect()->translate(width()-10-score_visual->getRect()->width(), 10+score_title->getRect()->height());
    towerOptions[0]->getRect()->moveTo(width()-towerOptions[0]->getRect()->width()-5, 50);
    towerOptions[1]->getRect()->moveTo(width()-towerOptions[1]->getRect()->width()-5, 50 + towerOptions[0]->getRect()->height());
    towerOptions[2]->getRect()->moveTo(width()-towerOptions[2]->getRect()->width()-5, 50 + towerOptions[0]->getRect()->height() + towerOptions[1]->getRect()->height());

    buildMap();
    createNavigationPath();
}

//A function to delete the ingame components
void Game::cleanInGame(){
    //Delete all of the InGame components
    delete score_visual;
    delete score_title;
    delete wave_visual;
    delete wave_title;
    delete tileHighlight;
    for(auto& t : map)
        delete t;
    for(auto& o : towerOptions)
        delete o;
    for(auto& c : numChars)
        delete c;
}

//A function to load the pause components
void Game::loadPause(){
    //load the corresponding images for each of the components
    pauseButtons.push_back(new Button(CONSTANTS::RESUME_PATH, CONSTANTS::RESUME_H_PATH, 0.25));
    pauseButtons.push_back(new Button(CONSTANTS::MAINMENU_PATH, CONSTANTS::MAINMENU_H_PATH, 0.25));

    //position the components
    pauseButtons[0]->getRect()->translate( (width()-pauseButtons[0]->getRect()->width())/2 , CONSTANTS::MARGIN_TOP);
    pauseButtons[1]->getRect()->translate( (width()-pauseButtons[1]->getRect()->width())/2 , CONSTANTS::MARGIN_TOP+pauseButtons[0]->getRect()->height());
}

//A function to delete the pause components
void Game::cleanPause(){
    //Delete all of the Pause components
    for(auto& b : pauseButtons)
        delete b;
}

//A function to load the help components
void Game::loadHelp(){
    //load the corresponding images for each of the components
    arrows.push_back(new Button(CONSTANTS::LEFT_PATH, CONSTANTS::LEFT_H_PATH, 0.25));
    arrows.push_back(new Button(CONSTANTS::RIGHT_PATH, CONSTANTS::RIGHT_H_PATH, 0.25));
    arrows.push_back(new Button(CONSTANTS::BACK_PATH, CONSTANTS::BACK_H_PATH, 0.5));

    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_1));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_2));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_3));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_4));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_5));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_6));

    //position the components
    arrows[2]->getRect()->translate( 10, 10);
    arrows[0]->getRect()->translate( 30, (height()-arrows[0]->getRect()->height())/2);
    arrows[1]->getRect()->translate( width()-30-arrows[1]->getRect()->width(), (height()-arrows[1]->getRect()->height())/2);

    for(auto& i : helpImages)
        i->getRect()->translate((width()-i->getRect()->width())/2, (height()-i->getRect()->height())/2);
}

//A function to delete the help components
void Game::cleanHelp(){
    //Delete all of the Help components
    for(auto& b : arrows)
        delete b;
    for(auto& i : helpImages)
        delete i;
}

//A function to generate the tiles that will display the map
void Game::buildMap(){
    //For each entry in the MAP array create a dirt tile if true and a grass tile if false
    for(const auto d : CONSTANTS::MAP)
        d==0 ?  map.push_back(new Tile(CONSTANTS::GRASS_TILE)) : map.push_back(new Tile(CONSTANTS::DIRT_TILE,d));
    //Tile starting positions
    int xPos = 50;
    int yPos = 50;
    //Tile column counter
    int colCounter = 0;
    //Reposition the newly created tiles into a grid
    for(auto& t : map){
        t->getRect()->translate(xPos, yPos);
        //Update the column until the maximum width is achieved
        if(++colCounter<CONSTANTS::TILE_COL)
            xPos += t->getRect()->width();
        //Otherwise move down to the next row
        else{
            xPos = 50;
            colCounter = 0;
            yPos += t->getRect()->height();
        }
    }
}

//A function to handle the selection of a tile
void Game::selectTile(Tile* t){
    //If the tile isn't already selected then select it by highlighting it
    if(!t->isActive()){
        t->setActive(true);
        tileHighlight->getRect()->moveTo(t->getRect()->topLeft());
    }
    //Otherwise if it is already selected build the selected tower type on the tile
    else{
        t->setActive(false);
        switch(curTowerOpt){
            case 0:
                if(getScore() >= CONSTANTS::TOWER_COST){
                    updateScore(-CONSTANTS::TOWER_COST);
                    towers.push_back(new Tower(CONSTANTS::TOWER_FIRE, *t->getRect()));
                }
                break;
            case 1:
                if(getScore() >= CONSTANTS::TOWER_COST){
                    updateScore(-CONSTANTS::TOWER_COST);
                    towers.push_back(new Tower(CONSTANTS::TOWER_ICE, *t->getRect()));
                }
                break;
            case 2:
                if(getScore() >= CONSTANTS::TOWER_COST){
                    updateScore(-CONSTANTS::TOWER_COST);
                    towers.push_back(new Tower(CONSTANTS::TOWER_EARTH, *t->getRect()));
                }
                break;
        }
        t->setOccupied(true);
    }
}

//A function to detect collisions between Tower and Enemy objects
void Game::raycast(){
    for(auto& t : towers){
        for(auto& e : enemies){
            //Draw a line between each tower and all of the enemies
            int distance = QLineF(t->getRect()->center(), e->getRect()->center()).length();
            //If the line's distance is less than the tower's range then that enemy can be attacked
            if(distance < t->getRange() && !t->isCoolDown()){
                //Cool down tower
                t->setCoolDown(true);
                t->setTimer(startTimer(t->getCoolDown()));
                //damage the enemy
                e->inflictDamage(t->getDamage());
                qDebug() << e->getHealth();
                //If the enemy's health is depleted then indicate that it is dead
                if(e->getHealth() <= 0)
                    e->setDead(true);
                break;
            }
        }
    }
}

//A function to delete all dead Enemies in the game
void Game::cleanEnemyList(){
    for(size_t i = 0; i<enemies.size(); i++){
        if(enemies[i]->isDead()){
            //When deleting the enemy, award the player the appropriate points
            updateScore(enemies[i]->getScore());
            delete enemies[i];
            enemies.erase(enemies.begin()+i);
        }
    }
}

//A function to create the Images for the number displays
void Game::paintNum(int number, QPainter& p, int x, int y){
    //Convert the desired number into a string to be parsed
    std::string num = std::to_string(number);
    //For each char in the string draw the appropriate Image
    //Also update the cordinates x,y so that the Images display corrrectly
    for(size_t i = 0; i < num.length(); i++){
        switch(num[i]){
            case '0':
                numChars[0]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[0]->getRect(),numChars[0]->getImage());
                x += numChars[0]->getRect()->width();
                break;
            case '1':
                numChars[1]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[1]->getRect(),numChars[1]->getImage());
                x += numChars[1]->getRect()->width();
                break;
            case '2':
                numChars[2]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[2]->getRect(),numChars[2]->getImage());
                x += numChars[2]->getRect()->width();
                break;
            case '3':
                numChars[3]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[3]->getRect(),numChars[3]->getImage());
                x += numChars[3]->getRect()->width();
                break;
            case '4':
                numChars[4]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[4]->getRect(),numChars[4]->getImage());
                x += numChars[4]->getRect()->width();
                break;
            case '5':
                numChars[5]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[5]->getRect(),numChars[5]->getImage());
                x += numChars[5]->getRect()->width();
                break;
            case '6':
                numChars[6]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[6]->getRect(),numChars[6]->getImage());
                x += numChars[6]->getRect()->width();
                break;
            case '7':
                numChars[7]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[7]->getRect(),numChars[7]->getImage());
                x += numChars[7]->getRect()->width();
                break;
            case '8':
                numChars[8]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[8]->getRect(),numChars[8]->getImage());
                x += numChars[8]->getRect()->width();
                break;
            case '9':
                numChars[9]->getRect()->moveTo(x,y);
                p.drawImage(*numChars[9]->getRect(),numChars[9]->getImage());
                x += numChars[9]->getRect()->width();
                break;
        }
    }

}

void Game::createNavigationPath(){
    for(auto& t : map){
        if(t->isPath())
            navPath[t->getPathID()-1] = t->getRect()->center();
    }
}


