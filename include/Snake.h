#pragma once
#include "defines.h"
#include "PowerUp.h"

//TODO  - highscore speichern
//      - pause einbauen
//      - random powerups... + f�r schneller (ne gewisse zeit), da kriegt man mehr punkte z.b.
//      - schwierigkeit ausw�hlen, evtl automatisch bei julia setzen!
//      - anzeige von punkten UND schwanzl�nge (die extra tracken)
//      - anzeige restzeit vom boost


#define CONNECT_TAIL_PARTS      (1)
#define MIN_TIME_FOR_NEXT_PU    (4)    /* minimum time in seconds for next PU */
#define MAX_TIME_FOR_NEXT_PU    (4)    /* minimum time in seconds for next PU */

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
               const std::vector< cv::Mat > vFoodImg, const std::vector< std::string > vFoodSounds, const std::vector< PowerUp > vPowerUps,
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
private:
    //void startGame();
    void resetGame();
    void updateTailParts();
    void drawScene();
    void drawIntoTile( const int x, const int y, const cv::Mat& img );
    void drawGameOver();
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
    cv::Point2i m_foodPos;
    cv::Point2i* mp_tailParts;
    cv::Scalar* mp_tailColors;
    int m_numTailParts      = 0;
    std::string m_gameWndName;

    std::vector< cv::Mat > m_vPlayerImg;
    std::vector< std::string > m_vPlayerNames;
    int m_currPlayerIdx     = 0;

    std::vector< PowerUp > m_vPowerUps;
    int m_currPowerUp       = 0;

    std::vector< cv::Mat > m_vFoodImg;
    std::vector< std::string > m_vFoodSounds;
    int m_currFoodIdx       = 0;

    cv::Mat m_tailImg;
    cv::Mat m_gameImg;

    // movement
    bool m_bWaitForNextMove = false;    /* waiting m_timeNextMove seconds for next move */
    double m_timeMove       = 0.1;      /* time till next move in seconds (current) -> i.a. for boosts */
    double m_lastTimeMove   = 0.1;      /* time till next move in seconds (selected in menu) -> for reset */
    clock_t m_timerNextMove;            /* for tracking time between moves */
};
