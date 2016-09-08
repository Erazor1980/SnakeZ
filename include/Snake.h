#pragma once
#include "defines.h"
#include <ctime>

#define DEBUG	1

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
    SnakeGame( const int gameWidth, const int gameHeight, const cv::Mat& headImg );

    void update();
    bool gameOver()
    {
        return m_gameOver;
    }
    cv::Mat getGameImg();
private:
    void keyHandling( const int keyCode );
    void drawScene();
    void init();

    int m_gameWidth         = 800;
    int m_gameHeight        = 600;
    eMovDirection m_movDir  = NONE;
    bool m_gameOver         = false;

    cv::Point2i m_headPos;
    cv::Mat m_headImg;
    cv::Mat m_gameImg;
};
