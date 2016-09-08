#pragma once
#include "defines.h"
#include <ctime>

#define TIME_FOR_MOVE_IN_SEC (0.2f)

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
    SnakeGame( const int tilesX, const int tilesY, const cv::Mat& headImg, const int sizeTile = 50, const bool easyMode = true );

    void update();
    bool gameOver()
    {
        return m_bGameOver;
    }
    void keyHandling( const int keyCode );
    cv::Mat getGameImg();
private:
    
    void drawScene();

    int m_tilesX            = 30;
    int m_tilesY            = 20;
    eMovDirection m_movDir  = NONE;
    bool m_bEasyMode        = true;
    bool m_bGameOver        = false;

    cv::Point2i m_headPos;              /* position of the head in tiles ( x, y ) */
    cv::Mat m_headImg;
    cv::Mat m_tailImg;

    cv::Mat m_gameImg;
    
    clock_t m_startTime;	            /* time member, to stop time between moves */
    bool m_bPause           = false;    /* waiting TIME_FOR_MOVE_IN_SEC for next move */
};
