#pragma once
#include "defines.h"
#include "PowerUp.h"
#include "Food.h"
#include "ScoreBoard.h"

//TODO  
//      (- anzeige von punkten UND schwanzlänge (die extra tracken))
//      - in den schwanz laufen evtl besser (oder überhaupt) darstellen


#define CONNECT_TAIL_PARTS      (1)
#define MIN_TIME_FOR_NEXT_PU    (4)    /* minimum time in seconds for next power up */
#define MAX_TIME_FOR_NEXT_PU    (8)    /* maximum time in seconds for next power up */

enum eMovDirection
{
	NONE = 0,
	LEFT,
	RIGHT,
    UP,
    DOWN
};

class SnakeGame
{
public:
    SnakeGame( const int tilesX, const int tilesY, const std::vector< cv::Mat > vPlayerImg, const std::vector< std::string > vPlayerNames, 
               const std::vector< cv::Mat > vFoodImg, const std::vector< std::string > vFoodSounds, const std::vector< PowerUp* > vPowerUps,
               const int tileSize = 50, const bool easyMode = true );
    ~SnakeGame();

    void startMenu( const std::string startSound = "", const std::string wndName = "SnakeZ" );
    void update();
    bool finishGame()
    {
        return m_bFinishGame;
    }
    void keyHandling( const int keyCode );
    cv::Mat getGameImg();

    friend PowerUp;
    friend Rocket;
    friend Chest;
    friend Food;
private:
    void resetGame();
    void updateTailParts();
    void drawScene();
    void drawIntoTile( const int x, const int y, const cv::Mat& img );
    void drawGameOver();
    void displayHighScore( const ScoreBoard& sb, const int posHS = -1 );      /* to be able to use it with "personal" SBs */

    void addRandomFood();

    cv::Point2i findFreeTile();         /* considering head, all tail parts and food/PU etc */

    int m_tilesX            = 30;
    int m_tilesY            = 20;
    int m_tileSize;
    int m_score             = 0;
    int m_addedScoreNumber  = 1;
    eMovDirection m_movDir  = RIGHT;
    eMovDirection m_lastDir = RIGHT;
    bool m_bEasyMode        = true;
    bool m_bGameOver        = false;
    bool m_bFinishGame      = false;

    cv::Point2i m_headPos;              /* position of the head in tiles ( x, y ) */
    cv::Point2i* mp_tailParts;
    cv::Scalar* mp_tailColors;
    int m_numTailParts      = 0;
    std::string m_gameWndName;

    std::vector< cv::Mat >      m_vPlayerImg;
    std::vector< std::string >  m_vPlayerNames;
    int m_currPlayerIdx         = 0;

    std::vector< PowerUp* >     m_vPowerUps;
    //int m_currPowerUp           = 0;

    // food
    std::vector< cv::Mat >      m_vFoodImg;
    std::vector< std::string >  m_vFoodSounds;
    std::vector< Food >         m_vFoodInGame;

    // images 
    cv::Mat m_tailImg;
    cv::Mat m_gameImg;

    // movement
    bool m_bWaitForNextMove = false;    /* waiting m_timeNextMove seconds for next move */
    double m_timeMove       = 0.1;      /* time till next move in seconds (current) -> i.a. for boosts */
    double m_lastTimeMove   = 0.1;      /* time till next move in seconds (selected in menu) -> for reset */
    clock_t m_timerNextMove;            /* for tracking time between moves */

    // highscores
    ScoreBoard m_overallScoreBoard;

};
