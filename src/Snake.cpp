#include "Snake.h"
#include "Windows.h"

SnakeGame::SnakeGame( const int gameWidth, const int gameHeight, const cv::Mat& headImg )
{
    m_gameWidth     = gameWidth;
    m_gameHeight    = gameHeight;

    cv::resize( headImg, m_headImg, cv::Size( 30, 30 ) );

    m_gameImg = cv::Mat( m_gameHeight, m_gameWidth, CV_8UC3, BLACK );
    m_headPos = cv::Point2i( m_gameWidth / 2, m_gameHeight / 2 );
}

void SnakeGame::update()
{
    int keyCode = cv::waitKey( 1 );
    if( keyCode > 0 )
    {
        keyHandling( keyCode );
        //return;
    }

    const int distToMove = m_headImg.rows;
    switch( m_movDir )
    {
    case LEFT:
        m_headPos.x -= distToMove;
        break;
    case RIGHT:
        m_headPos.x += distToMove;
        break;
    case UP:
        m_headPos.y -= distToMove;
        break;
    case DOWN:
        m_headPos.y += distToMove;
        break;
    default:
        break;
    }

    Sleep( 300 );
}

cv::Mat SnakeGame::getGameImg()
{
    drawScene();
    return m_gameImg;
}

void SnakeGame::drawScene()
{
    const int headRadius = m_headImg.rows / 2;

    m_gameImg.setTo( cv::Scalar( 0, 0, 0 ) );
    m_headImg.copyTo( m_gameImg( cv::Rect( m_headPos.x - headRadius, m_headPos.y - headRadius, m_headImg.cols, m_headImg.rows ) ) );
}

void SnakeGame::keyHandling( const int keyCode )
{
    if( 2424832 == keyCode )        //  left
    {
        m_movDir = LEFT;
    }
    else if( 2555904 == keyCode )   //  right
    {
        m_movDir = RIGHT;
    }
    else if( 2490368 == keyCode )     //  up
    {
        m_movDir = UP;
    }
    else if( 2621440 == keyCode )     //  down
    {
        m_movDir = DOWN;
    }
    else if( 27 == keyCode )
    {
        m_gameOver = true;
    }
}

void SnakeGame::init()
{
    
}
