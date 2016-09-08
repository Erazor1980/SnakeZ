#include "Snake.h"
#include "Windows.h"

SnakeGame::SnakeGame( const int tilesX, const int tilesY, const cv::Mat& headImg, const cv::Mat& foodImg,
                      const int tileSize, const bool easyMode )
{
    std::srand( ( unsigned int )std::time( 0 ) );

    m_tilesX        = tilesX;
    m_tilesY        = tilesY;
    m_tileSize      = tileSize;
    m_bEasyMode     = easyMode;
    mp_tailParts    = NULL;

    cv::resize( headImg, m_headImg, cv::Size( m_tileSize, m_tileSize ) );
    cv::resize( headImg, m_tailImg, cv::Size( m_tileSize / 2, m_tileSize / 2 ) );

    cv::resize( foodImg, m_foodImg, cv::Size( m_tileSize, m_tileSize ) );

    m_gameImg = cv::Mat( tilesY * m_tileSize, tilesX * m_tileSize, CV_8UC3, BLACK );
    m_headPos = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );

    resetGame();
}

SnakeGame::~SnakeGame()
{
    delete[] mp_tailParts;
}

void SnakeGame::resetGame()
{
    m_foodPos.x     = rand() % m_tilesX;
    m_foodPos.y     = rand() % m_tilesY;

    m_headPos       = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );
    m_movDir        = NONE;
    m_numTailParts  = 0;
    m_bGameOver     = false;

    if( mp_tailParts )
    {
        delete[] mp_tailParts;
    }
    mp_tailParts = new cv::Point2i[ m_tilesX * m_tilesY ];
    // set all to invalid values first, to avoid drawing at 0,0 after eating second, third,... one
    for( int i = 0; i < m_tilesX * m_tilesY; ++i )
    {
        mp_tailParts[ i ] = cv::Point2i( -1, -1 );
    }
}

void SnakeGame::update()
{
    if( m_bGameOver )
    {
        int key = cv::waitKey( 1 );
        if( key > 0 )
        {
            if( key != 27 )
            {
                resetGame();
                m_bGameOver = false;
            }
            else
            {
                m_bFinishGame = true;
            }
        }
        return;
    }

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
            updateTailParts();

            m_bPause = false;

            switch( m_movDir )
            {
            case LEFT:
                if( m_lastDir != RIGHT )
                {
                    m_headPos.x--;
                    m_lastDir = LEFT;
                }
                else
                {
                    m_headPos.x++;
                    m_movDir = RIGHT;
                }
                break;
            case RIGHT:
                if( m_lastDir != LEFT )
                {
                    m_headPos.x++;
                    m_lastDir = RIGHT;
                }
                else
                {
                    m_headPos.x--;
                    m_movDir = LEFT;
                }
                break;
            case UP:
                if( m_lastDir != DOWN )
                {
                    m_headPos.y--;
                    m_lastDir = UP;
                }
                else
                {
                    m_headPos.y++;
                    m_movDir = DOWN;
                }
                break;
            case DOWN:
                if( m_lastDir != UP )
                {
                    m_headPos.y++;
                    m_lastDir = DOWN;
                }
                else
                {
                    m_headPos.y--;
                    m_movDir = UP;
                }
                break;
            default:
                break;
            }
        }

        // food check
        if( m_headPos == m_foodPos )
        {
            m_numTailParts++;
            m_foodPos.x = rand() % m_tilesX;
            m_foodPos.y = rand() % m_tilesY;
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

        // collision check (with tail)
        for( int i = 0; i < m_numTailParts; ++i )
        {
            if( m_headPos == mp_tailParts[ i ] )
            {
                m_bGameOver = true;
                return;
            }
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

void SnakeGame::updateTailParts()
{
    for( int i = m_numTailParts - 1; i > 0; --i )
    {
        mp_tailParts[ i ] = mp_tailParts[ i - 1 ];
    }
    mp_tailParts[ 0 ] = m_headPos;
}

void SnakeGame::drawScene()
{
    if( m_bGameOver )
    {
        drawGameOver();
        return;
    }

    m_gameImg.setTo( cv::Scalar( 0, 0, 0 ) );
    //m_headImg.copyTo( m_gameImg( cv::Rect( m_headPos.x - headRadius, m_headPos.y - headRadius, m_headImg.cols, m_headImg.rows ) ) );

    // draw head
    int x = m_headPos.x * m_tileSize;
    int y = m_headPos.y * m_tileSize;
    drawIntoTile( x, y, m_headImg );

    // draw tail
    for( int i = 0; i < m_numTailParts; ++i )
    {
        if( mp_tailParts[ i ].x < 0 )
        {
            continue;
        }
        const int x = mp_tailParts[ i ].x * m_tileSize;
        const int y = mp_tailParts[ i ].y * m_tileSize;
        drawIntoTile( x, y, m_tailImg );
    }

    // draw food
    x = m_foodPos.x * m_tileSize;
    y = m_foodPos.y * m_tileSize;
    drawIntoTile( x, y, m_foodImg );
}

void SnakeGame::drawIntoTile( const int x, const int y, const cv::Mat& img )
{
    if( img.cols < m_tileSize )
    {
        img.copyTo( m_gameImg( cv::Rect( x + m_tileSize / 4, y + m_tileSize / 4, img.cols, img.rows ) ) );
    }
    else
    {
        img.copyTo( m_gameImg( cv::Rect( x, y, m_tileSize, m_tileSize ) ) );
    }    
}

void SnakeGame::drawGameOver()
{
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    int fontScale = 4;
    int fontThickness = 3;

    cv::putText( m_gameImg, "GAME OVER", cv::Point( m_gameImg.cols / 4, m_gameImg.rows / 2 ), fontFace, fontScale, GREEN, fontThickness );
    cv::putText( m_gameImg, "Close game -> ESC", cv::Point( m_gameImg.cols / 4, m_gameImg.rows / 2 + 40 ), fontFace, 1.5, RED, 2 );
    cv::putText( m_gameImg, "New game   -> other key", cv::Point( m_gameImg.cols / 4, m_gameImg.rows / 2 + 70 ), fontFace, 1.5, RED, 2 );
}

void SnakeGame::keyHandling( const int keyCode )
{
    if( 2424832 == keyCode )        //  left
    {
        if( m_movDir != RIGHT )
            m_movDir = LEFT;
    }
    else if( 2555904 == keyCode )   //  right
    {
        if( m_movDir != LEFT )
            m_movDir = RIGHT;
    }
    else if( 2490368 == keyCode )     //  up
    {
        if( m_movDir != DOWN )
            m_movDir = UP;
    }
    else if( 2621440 == keyCode )     //  down
    {
        if( m_movDir != UP )
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
