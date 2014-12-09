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
    const int SCREEN_WIDTH = 400;
    const int SCREEN_HEIGHT = 400;
    const int MARGIN_TOP = 64;
        
    //GUI constants
    const QString RIGHT_PATH = "C:/Qt/Projects/GameProject/rightarrow.png";
    const QString RIGHT_H_PATH = "C:/Qt/Projects/GameProject/rightarrow_hover.png";
    const QString LEFT_PATH = "C:/Qt/Projects/GameProject/leftarrow.png";
    const QString LEFT_H_PATH = "C:/Qt/Projects/GameProject/leftarrow_hover.png";
    const QString HELP_IMAGE_1 = "C:/Qt/Projects/GameProject/help1.png";
    const QString HELP_IMAGE_2 = "C:/Qt/Projects/GameProject/help2.png";
    const QString HELP_IMAGE_3 = "C:/Qt/Projects/GameProject/help3.png";
    const QString HELP_IMAGE_4 = "C:/Qt/Projects/GameProject/help4.png";
    const QString HELP_IMAGE_5 = "C:/Qt/Projects/GameProject/help5.png";
    const QString HELP_IMAGE_6 = "C:/Qt/Projects/GameProject/help6.png";
    const QString DIRT_TILE = "C:/Qt/Projects/GameProject/dirt_tile1.png";
    const QString GRASS_TILE = "C:/Qt/Projects/GameProject/grass_tile1.png";
    const QString HIGHLIGHT_TILE = "C:/Qt/Projects/GameProject/tile_highlight.png";
    const QString TOWER_FIRE = "C:/Qt/Projects/GameProject/fire.png";
    const QString TOWER_ICE = "C:/Qt/Projects/GameProject/ice.png";
    const QString TOWER_EARTH = "C:/Qt/Projects/GameProject/rock.png";
    const QString TOWEROPT_H = "C:/Qt/Projects/GameProject/toweroption_h.png";
    const QString UPGRADE = "C:/Qt/Projects/GameProject/upgrade_button.png";
    const QString UPGRADE_FIRE_BASE = "C:/Qt/Projects/GameProject/fire_icon_base.png";
    const QString UPGRADE_ICE_BASE = "C:/Qt/Projects/GameProject/ice_icon_base.png";
    const QString UPGRADE_EARTH_BASE = "C:/Qt/Projects/GameProject/earth_icon_base.png";
    const QString UPGRADE_STRENGTH = "C:/Qt/Projects/GameProject/strength_icon.png";
    const QString UPGRADE_RANGE = "C:/Qt/Projects/GameProject/target_icon.png";
    const QString UPGRADE_RATE = "C:/Qt/Projects/GameProject/time_icon.png";

    const QString CHAR_0 = "C:/Qt/Projects/GameProject/characters/normal/0.png";
    const QString CHAR_1 = "C:/Qt/Projects/GameProject/characters/normal/1.png";
    const QString CHAR_2 = "C:/Qt/Projects/GameProject/characters/normal/2.png";
    const QString CHAR_3 = "C:/Qt/Projects/GameProject/characters/normal/3.png";
    const QString CHAR_4 = "C:/Qt/Projects/GameProject/characters/normal/4.png";
    const QString CHAR_5 = "C:/Qt/Projects/GameProject/characters/normal/5.png";
    const QString CHAR_6 = "C:/Qt/Projects/GameProject/characters/normal/6.png";
    const QString CHAR_7 = "C:/Qt/Projects/GameProject/characters/normal/7.png";
    const QString CHAR_8 = "C:/Qt/Projects/GameProject/characters/normal/8.png";
    const QString CHAR_9 = "C:/Qt/Projects/GameProject/characters/normal/9.png";
    const QString CHAR_A = "C:/Qt/Projects/GameProject/characters/normal/A.png";
    const QString CHAR_B = "C:/Qt/Projects/GameProject/characters/normal/B.png";
    const QString CHAR_C = "C:/Qt/Projects/GameProject/characters/normal/C.png";
    const QString CHAR_D = "C:/Qt/Projects/GameProject/characters/normal/D.png";
    const QString CHAR_E = "C:/Qt/Projects/GameProject/characters/normal/E.png";
    const QString CHAR_F = "C:/Qt/Projects/GameProject/characters/normal/F.png";
    const QString CHAR_G = "C:/Qt/Projects/GameProject/characters/normal/G.png";
    const QString CHAR_H = "C:/Qt/Projects/GameProject/characters/normal/H.png";
    const QString CHAR_I = "C:/Qt/Projects/GameProject/characters/normal/I.png";
    const QString CHAR_J = "C:/Qt/Projects/GameProject/characters/normal/J.png";
    const QString CHAR_K = "C:/Qt/Projects/GameProject/characters/normal/K.png";
    const QString CHAR_L = "C:/Qt/Projects/GameProject/characters/normal/L.png";
    const QString CHAR_M = "C:/Qt/Projects/GameProject/characters/normal/M.png";
    const QString CHAR_N = "C:/Qt/Projects/GameProject/characters/normal/N.png";
    const QString CHAR_O = "C:/Qt/Projects/GameProject/characters/normal/O.png";
    const QString CHAR_P = "C:/Qt/Projects/GameProject/characters/normal/P.png";
    const QString CHAR_Q = "C:/Qt/Projects/GameProject/characters/normal/Q.png";
    const QString CHAR_R = "C:/Qt/Projects/GameProject/characters/normal/R.png";
    const QString CHAR_S = "C:/Qt/Projects/GameProject/characters/normal/S.png";
    const QString CHAR_T = "C:/Qt/Projects/GameProject/characters/normal/T.png";
    const QString CHAR_U = "C:/Qt/Projects/GameProject/characters/normal/U.png";
    const QString CHAR_V = "C:/Qt/Projects/GameProject/characters/normal/V.png";
    const QString CHAR_W = "C:/Qt/Projects/GameProject/characters/normal/W.png";
    const QString CHAR_X = "C:/Qt/Projects/GameProject/characters/normal/X.png";
    const QString CHAR_Y = "C:/Qt/Projects/GameProject/characters/normal/Y.png";
    const QString CHAR_Z = "C:/Qt/Projects/GameProject/characters/normal/Z.png";

    const QString CHAR_0_ACT = "C:/Qt/Projects/GameProject/characters/active/0.png";
    const QString CHAR_1_ACT = "C:/Qt/Projects/GameProject/characters/active/1.png";
    const QString CHAR_2_ACT = "C:/Qt/Projects/GameProject/characters/active/2.png";
    const QString CHAR_3_ACT = "C:/Qt/Projects/GameProject/characters/active/3.png";
    const QString CHAR_4_ACT = "C:/Qt/Projects/GameProject/characters/active/4.png";
    const QString CHAR_5_ACT = "C:/Qt/Projects/GameProject/characters/active/5.png";
    const QString CHAR_6_ACT = "C:/Qt/Projects/GameProject/characters/active/6.png";
    const QString CHAR_7_ACT = "C:/Qt/Projects/GameProject/characters/active/7.png";
    const QString CHAR_8_ACT = "C:/Qt/Projects/GameProject/characters/active/8.png";
    const QString CHAR_9_ACT = "C:/Qt/Projects/GameProject/characters/active/9.png";
    const QString CHAR_A_ACT = "C:/Qt/Projects/GameProject/characters/active/A.png";
    const QString CHAR_B_ACT = "C:/Qt/Projects/GameProject/characters/active/B.png";
    const QString CHAR_C_ACT = "C:/Qt/Projects/GameProject/characters/active/C.png";
    const QString CHAR_D_ACT = "C:/Qt/Projects/GameProject/characters/active/D.png";
    const QString CHAR_E_ACT = "C:/Qt/Projects/GameProject/characters/active/E.png";
    const QString CHAR_F_ACT = "C:/Qt/Projects/GameProject/characters/active/F.png";
    const QString CHAR_G_ACT = "C:/Qt/Projects/GameProject/characters/active/G.png";
    const QString CHAR_H_ACT = "C:/Qt/Projects/GameProject/characters/active/H.png";
    const QString CHAR_I_ACT = "C:/Qt/Projects/GameProject/characters/active/I.png";
    const QString CHAR_J_ACT = "C:/Qt/Projects/GameProject/characters/active/J.png";
    const QString CHAR_K_ACT = "C:/Qt/Projects/GameProject/characters/active/K.png";
    const QString CHAR_L_ACT = "C:/Qt/Projects/GameProject/characters/active/L.png";
    const QString CHAR_M_ACT = "C:/Qt/Projects/GameProject/characters/active/M.png";
    const QString CHAR_N_ACT = "C:/Qt/Projects/GameProject/characters/active/N.png";
    const QString CHAR_O_ACT = "C:/Qt/Projects/GameProject/characters/active/O.png";
    const QString CHAR_P_ACT = "C:/Qt/Projects/GameProject/characters/active/P.png";
    const QString CHAR_Q_ACT = "C:/Qt/Projects/GameProject/characters/active/Q.png";
    const QString CHAR_R_ACT = "C:/Qt/Projects/GameProject/characters/active/R.png";
    const QString CHAR_S_ACT = "C:/Qt/Projects/GameProject/characters/active/S.png";
    const QString CHAR_T_ACT = "C:/Qt/Projects/GameProject/characters/active/T.png";
    const QString CHAR_U_ACT = "C:/Qt/Projects/GameProject/characters/active/U.png";
    const QString CHAR_V_ACT = "C:/Qt/Projects/GameProject/characters/active/V.png";
    const QString CHAR_W_ACT = "C:/Qt/Projects/GameProject/characters/active/W.png";
    const QString CHAR_X_ACT = "C:/Qt/Projects/GameProject/characters/active/X.png";
    const QString CHAR_Y_ACT = "C:/Qt/Projects/GameProject/characters/active/Y.png";
    const QString CHAR_Z_ACT = "C:/Qt/Projects/GameProject/characters/active/Z.png";
    const QString CHAR_SPACE = "C:/Qt/Projects/GameProject/characters/space.png";

    const QString CHAR_0_RED = "C:/Qt/Projects/GameProject/characters/red/0.png";
    const QString CHAR_1_RED = "C:/Qt/Projects/GameProject/characters/red/1.png";
    const QString CHAR_2_RED = "C:/Qt/Projects/GameProject/characters/red/2.png";
    const QString CHAR_3_RED = "C:/Qt/Projects/GameProject/characters/red/3.png";
    const QString CHAR_4_RED = "C:/Qt/Projects/GameProject/characters/red/4.png";
    const QString CHAR_5_RED = "C:/Qt/Projects/GameProject/characters/red/5.png";
    const QString CHAR_6_RED = "C:/Qt/Projects/GameProject/characters/red/6.png";
    const QString CHAR_7_RED = "C:/Qt/Projects/GameProject/characters/red/7.png";
    const QString CHAR_8_RED = "C:/Qt/Projects/GameProject/characters/red/8.png";
    const QString CHAR_9_RED = "C:/Qt/Projects/GameProject/characters/red/9.png";

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
class GameObject : public QObject
{
    Q_OBJECT
public:
    //Constructors and Destructors
    GameObject();
    GameObject(QString, qreal=1);
    virtual ~GameObject();

    //rect and image getter functions
    QRect* getRect(){ return rect; }
    QImage* getImage() { return image; }
    QRect getRectV() const { return *rect; }
    QImage getImageV() const { return *image; }
    void setImage(QImage i) { image = new QImage(i); }
    void setRect(QRect r) { rect = new QRect(r); }
private:
    //properties
    QImage* image;
    QRect* rect;
};

#endif // GAMEOBJECT_H
