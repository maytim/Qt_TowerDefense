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
#include <QTimer>

#include <QDebug>

/*
    Default constructor.
    @brief It sets up all of the game GUI components and starts the game in the MENU state.
*/
Game::Game(QWidget *parent) : QWidget(parent) , state(MENU), helpIndex(0) , curTowerOpt(0), wave_value(0), score_value(10) ,
    enemyCount(0), generator(SEED), damageDisplayOffset(-2,2), tooltip(NULL)
{
    setWindowTitle("Elemental Defense");
    setFixedSize(CONSTANTS::SCREEN_WIDTH, CONSTANTS::SCREEN_HEIGHT);

    //Since we start in the Menu portion of the game we need mouse tracking
    setMouseTracking(true);

    //Load up the components for each game state
    fillCharReferences();
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
    cleanCharReferences();
}

void Game::cleanCharReferences(){
    for(auto& c : letterChars)
        delete c;
    for(auto& c : letterCharsAct)
        delete c;
}

/*
    The overridden paintEvent
    @brief for each paint event the game checks which state it's in and then draws the
    corresponding GUI components.
*/
void Game::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    switch(state){
        case MENU:
            //Draw the title Images
            painter.drawImage(*title_line1->getRect(), *title_line1->getImage());
            painter.drawImage(*title_line2->getRect(), *title_line2->getImage());

            //Then for each of the buttons check to display active or passive image
            if(start_button->isActive())
                painter.drawImage(*start_button->getRect(), start_button->getActiveImage());
            else
                painter.drawImage(*start_button->getRect(), *start_button->getImage());

            if(help_button->isActive())
                painter.drawImage(*help_button->getRect(), help_button->getActiveImage());
            else
                painter.drawImage(*help_button->getRect(), *help_button->getImage());
            if(quit_button->isActive())
                painter.drawImage(*quit_button->getRect(), quit_button->getActiveImage());
            else
                painter.drawImage(*quit_button->getRect(), *quit_button->getImage());
            break;
        case INGAME:
            //Draw the score and wave Images
            paintChar(std::to_string(getWave()),1,painter,10,10+wave_title->getRect()->height(),false);
            painter.drawImage(*score_title->getRect(),*score_title->getImage());
            paintChar(std::to_string(getScore()),1,painter,width()-std::to_string(getScore()).length()*6-5, 10+score_title->getRect()->height(),false);
            painter.drawImage(*wave_title->getRect(),*wave_title->getImage());

            //Tower Builder Menu
            for(const auto o : towerOptions)
                painter.drawImage(*o->getRect(), *o->getImage());
            painter.drawImage(*towerOptions[curTowerOpt]->getRect(), *towerOptHighlight->getImage());

            //Upgrade menu
            switch(curTowerOpt){
                case 0:
                    for(auto& u : fire_upgrade)
                        painter.drawImage(*u->getRect(), *u->getImage());
                    break;
                case 1:
                    for(auto& u : ice_upgrade)
                        painter.drawImage(*u->getRect(), *u->getImage());
                    break;
                case 2:
                    for(auto& u : earth_upgrade)
                        painter.drawImage(*u->getRect(), *u->getImage());
                    break;
            }

            for(auto& i : upgrade_icon)
                painter.drawImage(*i->getRect(), *i->getImage());

            //Draw the map tiles
            for(auto& t : map){
                painter.drawImage(*t->getRect(), *t->getImage());
                if(t->isActive())
                    painter.drawImage(*tileHighlight->getRect(), *tileHighlight->getImage());
            }

            //Draw each of the enemies
            for(auto& e : enemies){
                if(!e->isDead())
                    painter.drawImage(*e->getRect(), *e->getImage());
            }
\
            //Draw the towers
            for(const auto t : towers)
                painter.drawImage(*t->getRect(), *t->getImage());

            //Draw the damage number images
            for(const auto d : damageDisplays)
                painter.drawImage(*d->getRect(), *d->getImage());

            //Draw the tooltip
            if(tooltip != NULL)
                tooltip->paint(&painter);
            break;
        case CLEARED:
            paintChar("wave "+std::to_string(getWave())+" cleared",0.25,painter,(width()-(13+std::to_string(getWave()).length())*20)/2,100,false);
            if(continue_button->isActive())
                painter.drawImage(*continue_button->getRect(), continue_button->getActiveImage());
            else
                painter.drawImage(*continue_button->getRect(), *continue_button->getImage());
            break;

        case PAUSED:
            //For each of the buttons check to display active or passive image
            for(const auto b : pauseButtons){
                if(b->isActive())
                    painter.drawImage(*b->getRect(), b->getActiveImage());
                else
                    painter.drawImage(*b->getRect(), *b->getImage());
            }
            break;
        case HELP:
            //Draw the appropriate help image depending on the users current index
            painter.drawImage(*helpImages[helpIndex]->getRect(), *helpImages[helpIndex]->getImage());

            //For each of the arrows check to display active or passive image
            for(const auto b : arrows){
                if(b->isActive())
                    painter.drawImage(*b->getRect(), b->getActiveImage());
                else
                    painter.drawImage(*b->getRect(), *b->getImage());
            }
            break;
    }
}

/*
    timerEvent
    @brief The timerEvent is treated as an update loop. It is called every 10
    milliseconds and will update enemy positions if ingame or repaint the game.
*/
void Game::timerEvent(QTimerEvent *event){
    //If the game is active then use the paintTimer to call repaint() on a loop
    if(state == INGAME){
        if(event->timerId() == spawnTimer)
            spawner();
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
        enemies.push_back(new Enemy(ENEMY::NORMAL, navPath[0]));
        enemyCount++;
        spawnTimer = startTimer(2000);
    }    
}

void Game::spawner(){
    //Then start spawning the new enemies
    if(!spawnList.empty()){
        //Stop the current timer
        killTimer(spawnTimer);
        //Spawn the next enemy
        enemies.push_back(spawnList.back());
        spawnTimer = startTimer(spawnList.back()->getSpawnDelay());
        spawnList.pop_back();
    }
    else
        killTimer(spawnTimer);
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
        case MENU:
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
        case PAUSED:
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
        case HELP:
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
        case CLEARED:
            if(continue_button->getRect()->contains(event->pos()))
                continue_button->setActive(true);
            else
                continue_button->setActive(false);
            break;
        case INGAME:
            delete tooltip;
            tooltip = NULL;

            Type curTowerType;
            switch(curTowerOpt){
                case 0:
                    curTowerType = FIRE;
                    break;
                case 1:
                    curTowerType = ICE;
                    break;
                case 2:
                    curTowerType = EARTH;
                    break;
            }

            //Upgrade #1
            if(upgrade_icon[0]->getRect()->contains(event->pos())){
                tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getDamageCost(curTowerType)), 1, ACTIVE),
                                      mergeChars("str", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getDamage(curTowerType)), 1, ACTIVE));
                tooltip->moveTo(event->pos());
            }
            //Upgrade #2
            else if(upgrade_icon[1]->getRect()->contains(event->pos())){
                tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getRangeCost(curTowerType)), 1, ACTIVE),
                                      mergeChars("range", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getRange(curTowerType)), 1, ACTIVE));
                tooltip->moveTo(event->pos());
            }
            //Upgrade #3
            else if(upgrade_icon[2]->getRect()->contains(event->pos())){
                tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getRateCost(curTowerType)), 1, ACTIVE),
                                      mergeChars("rate", 1, NORMAL),
                                      mergeChars(std::to_string(Tower::getCoolDown(curTowerType)), 1, ACTIVE));
                tooltip->moveTo(event->pos());
            }
            break;
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
                state = INGAME;
                startTimers();

            }
            //Pressing Main Menu will return the user to the main menu
            else if(pauseButtons[1]->getRect()->contains(event->pos())){
                //Return to main menu
                killTimer(paintTimer);
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
            repaint();
            break;
        }
    case INGAME:
        //Check if the event occurred on a non-path tile that is empty. If so then highlight that tile
        for(auto& t : map)
            (!t->isPath() && !t->isOccupied() && t->getRect()->contains(event->pos())) ? selectTile(t) : t->setActive(false);

        //Check if the click event occured on one of the tower builder options and if so update the selected tower type
        for(size_t i=0; i<towerOptions.size(); i++){
            if(towerOptions[i]->getRect()->contains(event->pos()))
                curTowerOpt = i;
        }
        Type curTowerType;
        switch(curTowerOpt){
            case 0:
                curTowerType = FIRE;
                break;
            case 1:
                curTowerType = ICE;
                break;
            case 2:
                curTowerType = EARTH;
                break;
        }

        //Upgrade #1
        if(getScore() > Tower::getDamageCost(curTowerType) && upgrade_icon[0]->getRect()->contains(event->pos())){
            updateScore(-Tower::getDamageCost(curTowerType));
            Tower::upgradeDamage(curTowerType,1);
            tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getDamageCost(curTowerType)), 1, ACTIVE),
                                  mergeChars("str", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getDamage(curTowerType)), 1, ACTIVE));
            tooltip->moveTo(event->pos());
        }
        //Upgrade #2
        else if(getScore() > Tower::getRangeCost(curTowerType) && upgrade_icon[1]->getRect()->contains(event->pos())){
            updateScore(-Tower::getRangeCost(curTowerType));
            Tower::upgradeRange(curTowerType, 1);
            tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getRangeCost(curTowerType)), 1, ACTIVE),
                                  mergeChars("range", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getRange(curTowerType)), 1, ACTIVE));
            tooltip->moveTo(event->pos());
        }
        //Upgrade #3
        else if(getScore() > Tower::getRateCost(curTowerType) && upgrade_icon[2]->getRect()->contains(event->pos())){
            updateScore(-Tower::getRateCost(curTowerType));
            Tower::upgradeSpeed(curTowerType, 1);
            tooltip = new ToolTip(mergeChars("cost", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getRateCost(curTowerType)), 1, ACTIVE),
                                  mergeChars("rate", 1, NORMAL),
                                  mergeChars(std::to_string(Tower::getCoolDown(curTowerType)), 1, ACTIVE));
            tooltip->moveTo(event->pos());
        }

        break;
    case CLEARED:
        //Pressing continue will start the next wave
        if(continue_button->getRect()->contains(event->pos())){
            //Resume game
            newWave();
            state = INGAME;
        }
        break;
    }
}

/*
    newGame
    @brief A function to set up a new tower defense game
*/
void Game::newGame(){
    qDebug() << "New game";
    //Delete any existing data
    clearGame();
    //load new wave data
    wave_value = 0;
    newWave();
    score_value = 20;
    //start the timer used for spawning events
    paintTimer = startTimer(10);
}

void Game::startTimers(){
    //Start the timer that will update the enemies positions
    QTimer::singleShot(30,this,SLOT(moveEvent()));
    //Start the timer that moves the damge number images
    QTimer::singleShot(150,this,SLOT(moveDecals()));
    //Start the timer that will check for enemies within the towers range
    QTimer::singleShot(30,this,SLOT(collisionEvent()));
}

void Game::newWave(){
    //New wave
    updateWave();
    //Clear enemy list
    for(auto& e : enemies)
        delete e;
    enemies.clear();

    //Clear spawnList
    spawnList.clear();

    //New spawnList
    spawnList = wave_generator.generateSpawnList(getWave(), navPath[0]);
    enemyCount = spawnList.size();

    spawnTimer = startTimer(2000);
    startTimers();
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
    //Reset the upgrade stats
    Tower::resetUpgrades();
}

//A function to load the menu components
void Game::loadMenu(){
    //load the corresponding images for each of the components
    title_line1 = mergeChars("tower",0.125,NORMAL);
    title_line2 = mergeChars("defense",0.125,NORMAL);

    start_button = new Button(mergeChars("start",0.25,NORMAL), mergeChars("start",0.25,ACTIVE));
    help_button = new Button(mergeChars("help",0.25,NORMAL), mergeChars("help",0.25,ACTIVE));
    quit_button = new Button(mergeChars("quit",0.25,NORMAL), mergeChars("quit",0.25,ACTIVE));

    int const top_margin = (height() - (title_line1->getRect()->height() + title_line2->getRect()->height() +
                           start_button->getRect()->height() + help_button->getRect()->height() +
                           quit_button->getRect()->height()))/2;

    //position the components
    title_line1->getRect()->moveTo( (width()-title_line1->getRect()->width())/2 , top_margin );
    title_line2->getRect()->moveTo( (width()-title_line2->getRect()->width())/2 , top_margin + title_line1->getRect()->height());
    start_button->getRect()->moveTo( (width()-start_button->getRect()->width())/2 , top_margin + title_line1->getRect()->height() + title_line2->getRect()->height());
    help_button->getRect()->moveTo( (width()-help_button->getRect()->width())/2 , top_margin + title_line1->getRect()->height() + title_line2->getRect()->height() + start_button->getRect()->height());
    quit_button->getRect()->moveTo( (width()-quit_button->getRect()->width())/2 , top_margin + title_line1->getRect()->height() + title_line2->getRect()->height() + start_button->getRect()->height() + help_button->getRect()->height());
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
    score_title = mergeChars("score",1,NORMAL);
    wave_title = mergeChars("wave",1,NORMAL);

    tileHighlight = new Image(CONSTANTS::HIGHLIGHT_TILE);
    towerOptions.push_back(new Image(CONSTANTS::TOWER_FIRE));
    towerOptions.push_back(new Image(CONSTANTS::TOWER_ICE));
    towerOptions.push_back(new Image(CONSTANTS::TOWER_EARTH));
    towerOptHighlight = new Image(CONSTANTS::TOWEROPT_H);

    //Upgrade Menu
    for(int i = 0; i<3; i++){
        fire_upgrade.push_back(new Image(CONSTANTS::UPGRADE_FIRE_BASE));
        ice_upgrade.push_back(new Image(CONSTANTS::UPGRADE_ICE_BASE));
        earth_upgrade.push_back(new Image(CONSTANTS::UPGRADE_EARTH_BASE));
    }
    upgrade_icon.push_back(new Image(CONSTANTS::UPGRADE_STRENGTH));
    upgrade_icon.push_back(new Image(CONSTANTS::UPGRADE_RANGE));
    upgrade_icon.push_back(new Image(CONSTANTS::UPGRADE_RATE));

    continue_button = new Button(mergeChars("continue",0.25,NORMAL), mergeChars("continue",0.25,ACTIVE));

    //position the components
    wave_title->getRect()->moveTo(10,10);
    score_title->getRect()->moveTo(width()-score_title->getRect()->width()-5, 10);
    towerOptions[0]->getRect()->moveTo(width()-towerOptions[0]->getRect()->width()-5, 50);
    towerOptions[1]->getRect()->moveTo(width()-towerOptions[1]->getRect()->width()-5, 50 + towerOptions[0]->getRect()->height());
    towerOptions[2]->getRect()->moveTo(width()-towerOptions[2]->getRect()->width()-5, 50 + towerOptions[0]->getRect()->height() + towerOptions[1]->getRect()->height());

    //Upgrade Menu
    int x = width()-towerOptions[0]->getRect()->width()-5;
    int y = 75 + towerOptions[0]->getRect()->height() + towerOptions[1]->getRect()->height() + towerOptions[2]->getRect()->height();
    for(size_t i = 0, s = fire_upgrade.size(); i < s; i++){
        fire_upgrade[i]->getRect()->moveTo(x+(fire_upgrade[i]->getRect()->width())/4, y);
        ice_upgrade[i]->getRect()->moveTo(x+(fire_upgrade[i]->getRect()->width())/4, y);
        earth_upgrade[i]->getRect()->moveTo(x+(fire_upgrade[i]->getRect()->width())/4, y);
        upgrade_icon[i]->getRect()->moveTo(x+(fire_upgrade[i]->getRect()->width())/4, y);
        y+= fire_upgrade[i]->getRect()->height()+2;
    }

    //Cleared items
    continue_button->getRect()->moveTo( (width()-continue_button->getRect()->width())/2 , 264);

    buildMap();
    createNavigationPath();
}

void Game::fillCharReferences(){
    letterChars.push_back(new Image(CHARS::CHAR_0));
    letterChars.push_back(new Image(CHARS::CHAR_1));
    letterChars.push_back(new Image(CHARS::CHAR_2));
    letterChars.push_back(new Image(CHARS::CHAR_3));
    letterChars.push_back(new Image(CHARS::CHAR_4));
    letterChars.push_back(new Image(CHARS::CHAR_5));
    letterChars.push_back(new Image(CHARS::CHAR_6));
    letterChars.push_back(new Image(CHARS::CHAR_7));
    letterChars.push_back(new Image(CHARS::CHAR_8));
    letterChars.push_back(new Image(CHARS::CHAR_9));
    letterChars.push_back(new Image(CHARS::CHAR_A));
    letterChars.push_back(new Image(CHARS::CHAR_B));
    letterChars.push_back(new Image(CHARS::CHAR_C));
    letterChars.push_back(new Image(CHARS::CHAR_D));
    letterChars.push_back(new Image(CHARS::CHAR_E));
    letterChars.push_back(new Image(CHARS::CHAR_F));
    letterChars.push_back(new Image(CHARS::CHAR_G));
    letterChars.push_back(new Image(CHARS::CHAR_H));
    letterChars.push_back(new Image(CHARS::CHAR_I));
    letterChars.push_back(new Image(CHARS::CHAR_J));
    letterChars.push_back(new Image(CHARS::CHAR_K));
    letterChars.push_back(new Image(CHARS::CHAR_L));
    letterChars.push_back(new Image(CHARS::CHAR_M));
    letterChars.push_back(new Image(CHARS::CHAR_N));
    letterChars.push_back(new Image(CHARS::CHAR_O));
    letterChars.push_back(new Image(CHARS::CHAR_P));
    letterChars.push_back(new Image(CHARS::CHAR_Q));
    letterChars.push_back(new Image(CHARS::CHAR_R));
    letterChars.push_back(new Image(CHARS::CHAR_S));
    letterChars.push_back(new Image(CHARS::CHAR_T));
    letterChars.push_back(new Image(CHARS::CHAR_U));
    letterChars.push_back(new Image(CHARS::CHAR_V));
    letterChars.push_back(new Image(CHARS::CHAR_W));
    letterChars.push_back(new Image(CHARS::CHAR_X));
    letterChars.push_back(new Image(CHARS::CHAR_Y));
    letterChars.push_back(new Image(CHARS::CHAR_Z));

    letterCharsAct.push_back(new Image(CHARS::CHAR_0_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_1_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_2_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_3_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_4_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_5_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_6_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_7_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_8_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_9_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_A_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_B_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_C_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_D_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_E_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_F_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_G_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_H_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_I_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_J_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_K_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_L_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_M_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_N_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_O_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_P_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_Q_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_R_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_S_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_T_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_U_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_V_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_W_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_X_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_Y_ACT));
    letterCharsAct.push_back(new Image(CHARS::CHAR_Z_ACT));

    letterCharsRed.push_back(new Image(CHARS::CHAR_0_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_1_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_2_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_3_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_4_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_5_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_6_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_7_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_8_RED));
    letterCharsRed.push_back(new Image(CHARS::CHAR_9_RED));

    specialChars.push_back(new Image(CHARS::CHAR_SPACE));
}

//A function to delete the ingame components
void Game::cleanInGame(){
    //Delete all of the InGame components
    delete score_title;
    delete wave_title;
    delete tileHighlight;
    for(auto& t : map)
        delete t;
    for(auto& o : towerOptions)
        delete o;
}

//A function to load the pause components
void Game::loadPause(){
    //load the corresponding images for each of the components
    pauseButtons.push_back(new Button(mergeChars("resume",0.25,NORMAL), mergeChars("resume",0.25,ACTIVE)));
    pauseButtons.push_back(new Button(mergeChars("main menu",0.25,NORMAL), mergeChars("main menu",0.25,ACTIVE)));

    //position the components
    int const top_margin = (height() - (pauseButtons[0]->getRect()->height() + pauseButtons[1]->getRect()->height()))/2;
    pauseButtons[0]->getRect()->moveTo( (width()-pauseButtons[0]->getRect()->width())/2 , top_margin);
    pauseButtons[1]->getRect()->moveTo( (width()-pauseButtons[1]->getRect()->width())/2 , top_margin+pauseButtons[0]->getRect()->height());
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
    arrows.push_back(new Button(mergeChars("back",0.5,NORMAL), mergeChars("back",0.5,ACTIVE)));

    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_1));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_2));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_3));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_4));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_5));
    helpImages.push_back(new Image(CONSTANTS::HELP_IMAGE_6));

    //position the components
    arrows[2]->getRect()->moveTo( 10, 10);
    arrows[0]->getRect()->moveTo( 30, (height()-arrows[0]->getRect()->height())/2);
    arrows[1]->getRect()->moveTo( width()-30-arrows[1]->getRect()->width(), (height()-arrows[1]->getRect()->height())/2);

    for(auto& i : helpImages)
        i->getRect()->moveTo((width()-i->getRect()->width())/2, (height()-i->getRect()->height())/2);
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
        t->getRect()->moveTo(xPos, yPos);
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
                    t->setOccupied(true);
                }
                break;
            case 1:
                if(getScore() >= CONSTANTS::TOWER_COST){
                    updateScore(-CONSTANTS::TOWER_COST);
                    towers.push_back(new Tower(CONSTANTS::TOWER_ICE, *t->getRect()));
                    t->setOccupied(true);
                }
                break;
            case 2:
                if(getScore() >= CONSTANTS::TOWER_COST){
                    updateScore(-CONSTANTS::TOWER_COST);
                    towers.push_back(new Tower(CONSTANTS::TOWER_EARTH, *t->getRect()));
                    t->setOccupied(true);
                }
                break;
        }
    }
}

//A function to detect collisions between Tower and Enemy objects
void Game::raycast(){
    for(auto& t : towers){
        for(auto& e : enemies){
            //Draw a line between each tower and all of the enemies
            int distance = QLineF(t->getRect()->center(), e->getRect()->center()).length();
            //If the line's distance is less than the tower's range then that enemy can be attacked
            if(distance < t->getRange(t->getType()) && !t->isCoolDown()){
                //Cool down tower
                t->setCoolDown(true);
                QTimer::singleShot(t->getCoolDown(t->getType()),t,SLOT(testing()));
                //damage the enemy
                e->inflictDamage(t->getDamage(t->getType()));
                Image* damage = mergeChars(std::to_string(t->getDamage(t->getType())),1,RED);
                damage->getRect()->moveTo(e->getRect()->center().x()+damageDisplayOffset(generator), e->getRect()->top());
                damageDisplays.push_back(damage);
                QTimer::singleShot(1000,this,SLOT(removeDecal()));

                qDebug() << "Health: " << e->getHealth();
                //If the enemy's health is depleted then indicate that it is dead
                if(e->getHealth() <= 0){
                    e->setDead(true);
                    enemyCount--;
                    cleanEnemyList();
                    //End wave
                    if(enemyCount == 0)
                        state = CLEARED;
                }
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

Image* Game::mergeChars(std::string word, double scale, Chars c){
    //Create an Image to append to
    Image* image = new Image();

    for(size_t i = 0; i < word.length(); i++){
        if(c == ACTIVE){
            switch(word[i]){
            case '0':
                appendChar(letterCharsAct[0], scale, image);
                break;
            case '1':
                appendChar(letterCharsAct[1], scale, image);
                break;
            case '2':
                appendChar(letterCharsAct[2], scale, image);
                break;
            case '3':
                appendChar(letterCharsAct[3], scale, image);
                break;
            case '4':
                appendChar(letterCharsAct[4], scale, image);
                break;
            case '5':
                appendChar(letterCharsAct[5], scale, image);
                break;
            case '6':
                appendChar(letterCharsAct[6], scale, image);
                break;
            case '7':
                appendChar(letterCharsAct[7], scale, image);
                break;
            case '8':
                appendChar(letterCharsAct[8], scale, image);
                break;
            case '9':
                appendChar(letterCharsAct[9], scale, image);
                break;
            case 'a':
                appendChar(letterCharsAct[10], scale, image);
                break;
            case 'b':
                appendChar(letterCharsAct[11], scale, image);
                break;
            case 'c':
                appendChar(letterCharsAct[12], scale, image);
                break;
            case 'd':
                appendChar(letterCharsAct[13], scale, image);
                break;
            case 'e':
                appendChar(letterCharsAct[14], scale, image);
                break;
            case 'f':
                appendChar(letterCharsAct[15], scale, image);
                break;
            case 'g':
                appendChar(letterCharsAct[16], scale, image);
                break;
            case 'h':
                appendChar(letterCharsAct[17], scale, image);
                break;
            case 'i':
                appendChar(letterCharsAct[18], scale, image);
                break;
            case 'j':
                appendChar(letterCharsAct[19], scale, image);
                break;
            case 'k':
                appendChar(letterCharsAct[20], scale, image);
                break;
            case 'l':
                appendChar(letterCharsAct[21], scale, image);
                break;
            case 'm':
                appendChar(letterCharsAct[22], scale, image);
                break;
            case 'n':
                appendChar(letterCharsAct[23], scale, image);
                break;
            case 'o':
                appendChar(letterCharsAct[24], scale, image);
                break;
            case 'p':
                appendChar(letterCharsAct[25], scale, image);
                break;
            case 'q':
                appendChar(letterCharsAct[26], scale, image);
                break;
            case 'r':
                appendChar(letterCharsAct[27], scale, image);
                break;
            case 's':
                appendChar(letterCharsAct[28], scale, image);
                break;
            case 't':
                appendChar(letterCharsAct[29], scale, image);
                break;
            case 'u':
                appendChar(letterCharsAct[30], scale, image);
                break;
            case 'v':
                appendChar(letterCharsAct[31], scale, image);
                break;
            case 'w':
                appendChar(letterCharsAct[32], scale, image);
                break;
            case 'x':
                appendChar(letterCharsAct[33], scale, image);
                break;
            case 'y':
                appendChar(letterCharsAct[34], scale, image);
                break;
            case 'z':
                appendChar(letterCharsAct[35], scale, image);
                break;
            case ' ':
                appendChar(specialChars[0], scale, image);
                break;
        }
        }
        else if(c == NORMAL){
            switch(word[i]){
                case '0':
                    appendChar(letterChars[0], scale, image);
                    break;
                case '1':
                    appendChar(letterChars[1], scale, image);
                    break;
                case '2':
                    appendChar(letterChars[2], scale, image);
                    break;
                case '3':
                    appendChar(letterChars[3], scale, image);
                    break;
                case '4':
                    appendChar(letterChars[4], scale, image);
                    break;
                case '5':
                    appendChar(letterChars[5], scale, image);
                    break;
                case '6':
                    appendChar(letterChars[6], scale, image);
                    break;
                case '7':
                    appendChar(letterChars[7], scale, image);
                    break;
                case '8':
                    appendChar(letterChars[8], scale, image);
                    break;
                case '9':
                    appendChar(letterChars[9], scale, image);
                    break;
                case 'a':
                    appendChar(letterChars[10], scale, image);
                    break;
                case 'b':
                    appendChar(letterChars[11], scale, image);
                    break;
                case 'c':
                    appendChar(letterChars[12], scale, image);
                    break;
                case 'd':
                    appendChar(letterChars[13], scale, image);
                    break;
                case 'e':
                    appendChar(letterChars[14], scale, image);
                    break;
                case 'f':
                    appendChar(letterChars[15], scale, image);
                    break;
                case 'g':
                    appendChar(letterChars[16], scale, image);
                    break;
                case 'h':
                    appendChar(letterChars[17], scale, image);
                    break;
                case 'i':
                    appendChar(letterChars[18], scale, image);
                    break;
                case 'j':
                    appendChar(letterChars[19], scale, image);
                    break;
                case 'k':
                    appendChar(letterChars[20], scale, image);
                    break;
                case 'l':
                    appendChar(letterChars[21], scale, image);
                    break;
                case 'm':
                    appendChar(letterChars[22], scale, image);
                    break;
                case 'n':
                    appendChar(letterChars[23], scale, image);
                    break;
                case 'o':
                    appendChar(letterChars[24], scale, image);
                    break;
                case 'p':
                    appendChar(letterChars[25], scale, image);
                    break;
                case 'q':
                    appendChar(letterChars[26], scale, image);
                    break;
                case 'r':
                    appendChar(letterChars[27], scale, image);
                    break;
                case 's':
                    appendChar(letterChars[28], scale, image);
                    break;
                case 't':
                    appendChar(letterChars[29], scale, image);
                    break;
                case 'u':
                    appendChar(letterChars[30], scale, image);
                    break;
                case 'v':
                    appendChar(letterChars[31], scale, image);
                    break;
                case 'w':
                    appendChar(letterChars[32], scale, image);
                    break;
                case 'x':
                    appendChar(letterChars[33], scale, image);
                    break;
                case 'y':
                    appendChar(letterChars[34], scale, image);
                    break;
                case 'z':
                    appendChar(letterChars[35], scale, image);
                    break;
                case ' ':
                    appendChar(specialChars[0], scale, image);
                    break;
            }
        }
        else if(c == RED){
            switch(word[i]){
                case '0':
                    appendChar(letterCharsRed[0], scale, image);
                    break;
                case '1':
                    appendChar(letterCharsRed[1], scale, image);
                    break;
                case '2':
                    appendChar(letterCharsRed[2], scale, image);
                    break;
                case '3':
                    appendChar(letterCharsRed[3], scale, image);
                    break;
                case '4':
                    appendChar(letterCharsRed[4], scale, image);
                    break;
                case '5':
                    appendChar(letterCharsRed[5], scale, image);
                    break;
                case '6':
                    appendChar(letterCharsRed[6], scale, image);
                    break;
                case '7':
                    appendChar(letterCharsRed[7], scale, image);
                    break;
                case '8':
                    appendChar(letterCharsRed[8], scale, image);
                    break;
                case '9':
                    appendChar(letterCharsRed[9], scale, image);
                    break;
            }
        }
    }

    return image;
}

void Game::appendChar(Image* character, double scale, Image* i){
    Image* copy = character->scaledCopy(scale);
    i->append(copy);
}

void Game::printChar(Image* character, double scale, QPainter& p, int& x, int& y){
    Image* copy = character->scaledCopy(scale);
    copy->getRect()->moveTo(x,y);
    p.drawImage(*copy->getRect(),*copy->getImage());
    x += copy->getRect()->width();
}

//A function to create the Images for the number displays
void Game::paintChar(std::string word, double scale, QPainter& p, int x, int y, bool active){
    //For each char in the string draw the appropriate Image
    //Also update the cordinates x,y so that the Images display corrrectly
    for(size_t i = 0; i < word.length(); i++){
        if(active){
            switch(word[i]){
            case '0':
                printChar(letterCharsAct[0], scale, p, x, y);
                break;
            case '1':
                printChar(letterCharsAct[1], scale, p, x, y);
                break;
            case '2':
                printChar(letterCharsAct[2], scale, p, x, y);
                break;
            case '3':
                printChar(letterCharsAct[3], scale, p, x, y);
                break;
            case '4':
                printChar(letterCharsAct[4], scale, p, x, y);
                break;
            case '5':
                printChar(letterCharsAct[5], scale, p, x, y);
                break;
            case '6':
                printChar(letterCharsAct[6], scale, p, x, y);
                break;
            case '7':
                printChar(letterCharsAct[7], scale, p, x, y);
                break;
            case '8':
                printChar(letterCharsAct[8], scale, p, x, y);
                break;
            case '9':
                printChar(letterCharsAct[9], scale, p, x, y);
                break;
            case 'a':
                printChar(letterCharsAct[10], scale, p, x, y);
                break;
            case 'b':
                printChar(letterCharsAct[11], scale, p, x, y);
                break;
            case 'c':
                printChar(letterCharsAct[12], scale, p, x, y);
                break;
            case 'd':
                printChar(letterCharsAct[13], scale, p, x, y);
                break;
            case 'e':
                printChar(letterCharsAct[14], scale, p, x, y);
                break;
            case 'f':
                printChar(letterCharsAct[15], scale, p, x, y);
                break;
            case 'g':
                printChar(letterCharsAct[16], scale, p, x, y);
                break;
            case 'h':
                printChar(letterCharsAct[17], scale, p, x, y);
                break;
            case 'i':
                printChar(letterCharsAct[18], scale, p, x, y);
                break;
            case 'j':
                printChar(letterCharsAct[19], scale, p, x, y);
                break;
            case 'k':
                printChar(letterCharsAct[20], scale, p, x, y);
                break;
            case 'l':
                printChar(letterCharsAct[21], scale, p, x, y);
                break;
            case 'm':
                printChar(letterCharsAct[22], scale, p, x, y);
                break;
            case 'n':
                printChar(letterCharsAct[23], scale, p, x, y);
                break;
            case 'o':
                printChar(letterCharsAct[24], scale, p, x, y);
                break;
            case 'p':
                printChar(letterCharsAct[25], scale, p, x, y);
                break;
            case 'q':
                printChar(letterCharsAct[26], scale, p, x, y);
                break;
            case 'r':
                printChar(letterCharsAct[27], scale, p, x, y);
                break;
            case 's':
                printChar(letterCharsAct[28], scale, p, x, y);
                break;
            case 't':
                printChar(letterCharsAct[29], scale, p, x, y);
                break;
            case 'u':
                printChar(letterCharsAct[30], scale, p, x, y);
                break;
            case 'v':
                printChar(letterCharsAct[31], scale, p, x, y);
                break;
            case 'w':
                printChar(letterCharsAct[32], scale, p, x, y);
                break;
            case 'x':
                printChar(letterCharsAct[33], scale, p, x, y);
                break;
            case 'y':
                printChar(letterCharsAct[34], scale, p, x, y);
                break;
            case 'z':
                printChar(letterCharsAct[35], scale, p, x, y);
                break;
            case ' ':
                printChar(specialChars[0], scale, p, x, y);
                break;
        }
        }
        else{
            switch(word[i]){
                case '0':
                    printChar(letterChars[0], scale, p, x, y);
                    break;
                case '1':
                    printChar(letterChars[1], scale, p, x, y);
                    break;
                case '2':
                    printChar(letterChars[2], scale, p, x, y);
                    break;
                case '3':
                    printChar(letterChars[3], scale, p, x, y);
                    break;
                case '4':
                    printChar(letterChars[4], scale, p, x, y);
                    break;
                case '5':
                    printChar(letterChars[5], scale, p, x, y);
                    break;
                case '6':
                    printChar(letterChars[6], scale, p, x, y);
                    break;
                case '7':
                    printChar(letterChars[7], scale, p, x, y);
                    break;
                case '8':
                    printChar(letterChars[8], scale, p, x, y);
                    break;
                case '9':
                    printChar(letterChars[9], scale, p, x, y);
                    break;
                case 'a':
                    printChar(letterChars[10], scale, p, x, y);
                    break;
                case 'b':
                    printChar(letterChars[11], scale, p, x, y);
                    break;
                case 'c':
                    printChar(letterChars[12], scale, p, x, y);
                    break;
                case 'd':
                    printChar(letterChars[13], scale, p, x, y);
                    break;
                case 'e':
                    printChar(letterChars[14], scale, p, x, y);
                    break;
                case 'f':
                    printChar(letterChars[15], scale, p, x, y);
                    break;
                case 'g':
                    printChar(letterChars[16], scale, p, x, y);
                    break;
                case 'h':
                    printChar(letterChars[17], scale, p, x, y);
                    break;
                case 'i':
                    printChar(letterChars[18], scale, p, x, y);
                    break;
                case 'j':
                    printChar(letterChars[19], scale, p, x, y);
                    break;
                case 'k':
                    printChar(letterChars[20], scale, p, x, y);
                    break;
                case 'l':
                    printChar(letterChars[21], scale, p, x, y);
                    break;
                case 'm':
                    printChar(letterChars[22], scale, p, x, y);
                    break;
                case 'n':
                    printChar(letterChars[23], scale, p, x, y);
                    break;
                case 'o':
                    printChar(letterChars[24], scale, p, x, y);
                    break;
                case 'p':
                    printChar(letterChars[25], scale, p, x, y);
                    break;
                case 'q':
                    printChar(letterChars[26], scale, p, x, y);
                    break;
                case 'r':
                    printChar(letterChars[27], scale, p, x, y);
                    break;
                case 's':
                    printChar(letterChars[28], scale, p, x, y);
                    break;
                case 't':
                    printChar(letterChars[29], scale, p, x, y);
                    break;
                case 'u':
                    printChar(letterChars[30], scale, p, x, y);
                    break;
                case 'v':
                    printChar(letterChars[31], scale, p, x, y);
                    break;
                case 'w':
                    printChar(letterChars[32], scale, p, x, y);
                    break;
                case 'x':
                    printChar(letterChars[33], scale, p, x, y);
                    break;
                case 'y':
                    printChar(letterChars[34], scale, p, x, y);
                    break;
                case 'z':
                    printChar(letterChars[35], scale, p, x, y);
                    break;
                case ' ':
                    printChar(specialChars[0], scale, p, x, y);
                    break;
            }
        }
    }

}

void Game::createNavigationPath(){
    for(auto& t : map){
        if(t->isPath())
            navPath[t->getPathID()-1] = t->getRect()->center();
    }
}

Game::ToolTip::ToolTip(Image* c, Image* c_a, Image* s, Image* s_u)
{
    cost = c;
    cost_amount = c_a;
    stat = s;
    stat_upgrade = s_u;
    background = new Image(TOOLTIP::BASE);
}

Game::ToolTip::~ToolTip(){
    delete cost;
    delete cost_amount;
    delete background;
    delete stat;
    delete stat_upgrade;
}

void Game::ToolTip::moveTo(QPointF position){
    int x = position.x();
    int y = position.y();
    //x and y correspond to the mosue coordinates so need to draw a rect to the bottom left of the coordinates
    resizeBackground();
    background->getRect()->moveTo(x-background->getRect()->width(), y);
    stat->getRect()->moveTo(background->getRect()->x()+2, background->getRect()->y()+2);
    stat_upgrade->getRect()->moveTo(stat->getRect()->right()+3, stat->getRect()->y());
    cost->getRect()->moveTo(stat_upgrade->getRect()->right()+5, stat_upgrade->getRect()->y());
    cost_amount->getRect()->moveTo(cost->getRect()->right()+3, cost->getRect()->y());
}

void Game::ToolTip::paint(QPainter *p){
    p->drawImage(*background->getRect(), *background->getImage());
    p->drawImage(*stat->getRect(), *stat->getImage());
    p->drawImage(*stat_upgrade->getRect(), *stat_upgrade->getImage());
    p->drawImage(*cost->getRect(), *cost->getImage());
    p->drawImage(*cost_amount->getRect(), *cost_amount->getImage());
}

void Game::ToolTip::resizeBackground(){
    int width = 2 + stat->getRect()->width() + 3 + stat_upgrade->getRect()->width() + 5 +
            cost->getRect()->width() + 3 + cost_amount->getRect()->width();
    int height = cost->getRect()->height() + 4;
    background->setImage(background->getImage()->scaled(width, height, Qt::IgnoreAspectRatio));
    background->setRect(background->getImage()->rect());
}

