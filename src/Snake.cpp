#include "Snake.h"
#include "Windows.h"

SnakeGame::SnakeGame( const int tilesX, const int tilesY, const cv::Mat& headImg, const int sizeTile, const bool easyMode )
{
    m_tilesX = tilesX;
    m_tilesY = tilesY;

    m_bEasyMode = easyMode;

    cv::resize( headImg, m_headImg, cv::Size( sizeTile, sizeTile ) );
    cv::resize( headImg, m_tailImg, cv::Size( sizeTile / 2, sizeTile / 2 ) );

    m_gameImg = cv::Mat( tilesY * sizeTile, tilesX * sizeTile, CV_8UC3, BLACK );
    m_headPos = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );
}

void SnakeGame::update()
{
    // timer and position change
    if( m_bPause == false )
    {
        m_startTime         = clock();
        m_bPause            = true;
    }
    else
    {
        clock_t end         = clock();
        float elapsedTime   = ( float )( end - m_startTime ) / CLOCKS_PER_SEC;
        if( elapsedTime > TIME_FOR_MOVE_IN_SEC )
        {
            m_bPause = false;

            switch( m_movDir )
            {
            case LEFT:
                m_headPos.x--;
                break;
            case RIGHT:
                m_headPos.x++;
                break;
            case UP:
                m_headPos.y--;
                break;
            case DOWN:
                m_headPos.y++;
                break;
            default:
                break;
            }
        }
    }
    
    // boundary check
    if( m_bEasyMode )
    {
        if( m_headPos.x >= m_tilesX )
            m_headPos.x = 0;
        else if( m_headPos.x < 0 )
            m_headPos.x = m_tilesX - 1;
        if( m_headPos.y >= m_tilesY )
            m_headPos.y = 0;
        else if( m_headPos.y < 0 )
            m_headPos.y = m_tilesY - 1;
    }
    else
    {
        if( m_headPos.x >= m_tilesX || m_headPos.x < 0 || m_headPos.y >= m_tilesY || m_headPos.y < 0 )
        {
            m_bGameOver = true;
            return;
        }
    }

    // key handling
    int keyCode = cv::waitKey( 1 );
    if( keyCode > 0 )
    {
        keyHandling( keyCode );
        return;
    }
}

cv::Mat SnakeGame::getGameImg()
{
    drawScene();
    return m_gameImg;
}

void SnakeGame::drawScene()
{
    m_gameImg.setTo( cv::Scalar( 0, 0, 0 ) );
    //m_headImg.copyTo( m_gameImg( cv::Rect( m_headPos.x - headRadius, m_headPos.y - headRadius, m_headImg.cols, m_headImg.rows ) ) );
    const int tileSize = m_headImg.rows;
    const int x = m_headPos.x * tileSize;
    const int y = m_headPos.y * tileSize;
    m_headImg.copyTo( m_gameImg( cv::Rect( x, y, m_headImg.cols, m_headImg.rows ) ) );
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
        m_bGameOver = true;
    }
    else if( 'e' == keyCode )
    {
        m_bEasyMode = !m_bEasyMode;
    }
}
