#pragma once
#include "defines.h"
#include <ctime>

//TODO  - highscore speichern
//      - pause einbauen
//      - random powerups... + für schneller (ne gewisse zeit), da kriegt man mehr punkte z.b.
//      - schwierigkeit auswählen, evtl automatisch bei julia setzen!


#define CONNECT_TAIL_PARTS      (1)

enum eMovDirection
{
	NONE = 0,
	LEFT,
	RIGHT,
    UP,
    DOWN
};

struct powerUp
{
    powerUp( const cv::String name, const cv::Mat img, const int dur )
    {
        m_name = name;
        img.copyTo( m_img );
        m_duration = dur;
    }
    cv::String  m_name;
    cv::Mat     m_img;
    int         m_duration;     /* how long will the power up remain in seconds */
};

class SnakeGame
{
public:
    SnakeGame( const int tilesX, const int tilesY, const std::vector< cv::Mat > vPlayerImg, const std::vector< std::string > vPlayerNames, 
               const std::vector< cv::Mat > vFoodImg, const std::vector< std::string > vFoodSounds, const std::vector< powerUp > vPowerUps,
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
private:
    void resetGame();
    void updateTailParts();
    void drawScene();
    void drawIntoTile( const int x, const int y, const cv::Mat& img );
    void drawGameOver();
    int m_tilesX            = 30;
    int m_tilesY            = 20;
    int m_tileSize;
    int m_score             = 0;
    eMovDirection m_movDir  = NONE;
    eMovDirection m_lastDir = NONE;
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

    std::vector< powerUp > m_vPowerUps;
    int m_currPowerUp       = 0;

    std::vector< cv::Mat > m_vFoodImg;
    std::vector< std::string > m_vFoodSounds;
    int m_currFoodIdx       = 0;

    cv::Mat m_tailImg;
    cv::Mat m_gameImg;

    clock_t m_timerNextMove;            /* time member, to stop time between moves */
    bool m_bWaitForNextMove = false;    /* waiting m_timeNextMove seconds for next move */
    double m_timeMove       = 0.1;      /* time till next move in seconds */

    double m_timeTillNextPU = 15;       /* time till next power up in seconds */
    clock_t m_timerPU;
};
