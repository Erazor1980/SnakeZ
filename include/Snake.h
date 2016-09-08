#pragma once
#include "defines.h"
#include <ctime>

#define TIME_FOR_MOVE_IN_SEC (0.1f)

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
    SnakeGame( const int tilesX, const int tilesY, const cv::Mat& headImg, const cv::Mat& foodImg,
               const int tileSize = 50, const bool easyMode = true );
    ~SnakeGame();

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
    eMovDirection m_movDir  = NONE;
    eMovDirection m_lastDir = NONE;
    bool m_bEasyMode        = true;
    bool m_bGameOver        = false;
    bool m_bFinishGame      = false;

    cv::Point2i m_headPos;              /* position of the head in tiles ( x, y ) */
    cv::Point2i m_foodPos;
    cv::Point2i* mp_tailParts;
    int m_numTailParts      = 0;

    cv::Mat m_headImg;
    cv::Mat m_tailImg;
    cv::Mat m_foodImg;
    cv::Mat m_gameImg;
    
    clock_t m_startTime;	            /* time member, to stop time between moves */
    bool m_bPause           = false;    /* waiting TIME_FOR_MOVE_IN_SEC for next move */
};
