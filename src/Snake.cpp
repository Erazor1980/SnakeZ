#include "Snake.h"
#include "windows.h"
#include <mmsystem.h>

SnakeGame::SnakeGame( const int tilesX, const int tilesY, const cv::Mat& headImg,
                      const std::vector< cv::Mat > vFoodItems, const std::vector< std::string > vFoodSounds,
                      const int tileSize, const bool easyMode  )
{
    std::srand( ( unsigned int )std::time( 0 ) );

    m_tilesX        = tilesX;
    m_tilesY        = tilesY;
    m_tileSize      = tileSize;
    m_bEasyMode     = easyMode;
    mp_tailParts    = NULL;
    mp_tailColors   = NULL;

    cv::resize( headImg, m_headImg, cv::Size( m_tileSize, m_tileSize ) );
    cv::resize( headImg, m_tailImg, cv::Size( m_tileSize - 10, m_tileSize - 10 ) );

    assert( vFoodItems.size() == vFoodSounds.size() );

    for( int i = 0; i < vFoodItems.size(); ++i )
    {
        cv::Mat currFoodImg;
        cv::resize( vFoodItems[ i ], currFoodImg, cv::Size( m_tileSize, m_tileSize ) );
        m_vFoodImg.push_back( currFoodImg );
        m_vFoodSounds.push_back( vFoodSounds[ i ] );
    }

    m_gameImg = cv::Mat( tilesY * m_tileSize, tilesX * m_tileSize, CV_8UC3, BLACK );
    m_headPos = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );

    resetGame();
}

SnakeGame::~SnakeGame()
{
    if( mp_tailParts )
        delete[] mp_tailParts;
    if( mp_tailColors )
        delete[] mp_tailColors;
}

void SnakeGame::resetGame()
{
    m_foodPos.x     = rand() % m_tilesX;
    m_foodPos.y     = rand() % m_tilesY;

    m_headPos       = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );
    m_movDir        = NONE;
    m_numTailParts  = 0;
    m_bGameOver     = false;
    m_score         = 0;

    if( mp_tailParts )
    {
        delete[] mp_tailParts;
    }
    mp_tailParts = new cv::Point2i[ m_tilesX * m_tilesY ];

    if( mp_tailColors )
    {
        delete[] mp_tailColors;
    }
    mp_tailColors = new cv::Scalar[ m_tilesX * m_tilesY ];
    
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
            PlaySound( m_vFoodSounds[ m_currFoodIdx ].c_str(), NULL, SND_ASYNC );

            // find a free tile for the new food placement
            while( true )
            {
                m_foodPos.x = rand() % m_tilesX;
                m_foodPos.y = rand() % m_tilesY;
                bool occupiedTile = false;
                if( m_foodPos == m_headPos )
                {
                    continue;
                }
                for( int i = 0; i < m_numTailParts; ++i )
                {
                    if( m_foodPos == mp_tailParts[ i ] )
                    {
                        occupiedTile = true;
                        break;
                    }                        
                }
                if( !occupiedTile )
                    break;
            }

            mp_tailColors[ m_numTailParts ] = cv::Scalar( rand() % 255, rand() % 255, rand() % 255 );
            m_numTailParts++;
            m_score++;
            
            m_currFoodIdx = rand() % m_vFoodImg.size();
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
    if( m_numTailParts > 0 )
    {
        for( int i = m_numTailParts - 1; i > 0; --i )
        {
            mp_tailParts[ i ] = mp_tailParts[ i - 1 ];
        }
        mp_tailParts[ 0 ] = m_headPos;
    }
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

    // draw tail
#if CONNECT_TAIL_PARTS
    const int halfTile = m_tileSize / 2;
    int lastX = m_headPos.x * m_tileSize + halfTile;    // for drawing a line between all tail parts
    int lastY = m_headPos.y * m_tileSize + halfTile;
#endif
    for( int i = 0; i < m_numTailParts; ++i )
    {
        if( mp_tailParts[ i ].x < 0 )
        {
            continue;
        }
        const int x = mp_tailParts[ i ].x * m_tileSize;
        const int y = mp_tailParts[ i ].y * m_tileSize;

        m_tailImg.setTo( cv::Scalar( 0, 0, 0 ) );
        cv::circle( m_tailImg, cv::Point( m_tailImg.cols / 2, m_tailImg.rows / 2 ), m_tileSize / 3, mp_tailColors[ i ], -1 );
        drawIntoTile( x, y, m_tailImg );
#if CONNECT_TAIL_PARTS
        if( abs( lastX - ( x + halfTile ) ) < 2 * m_tileSize && abs( lastY - ( y + halfTile ) ) < 2 * m_tileSize )
        {
            // drawing only, if the tail parts are connected ( for easy mode, with going through walls )
            cv::line( m_gameImg, cv::Point2i( lastX, lastY ), cv::Point2i( x + halfTile, y + halfTile ), WHITE, 3 );
        }
        
        lastX = x + halfTile;
        lastY = y + halfTile;
#endif
    }

    // draw head
    int x = m_headPos.x * m_tileSize;
    int y = m_headPos.y * m_tileSize;
    drawIntoTile( x, y, m_headImg );

    // draw food
    x = m_foodPos.x * m_tileSize;
    y = m_foodPos.y * m_tileSize;
    drawIntoTile( x, y, m_vFoodImg[ m_currFoodIdx ] );

    // display points
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = 1.5;
    int fontThickness = 2;
    char pts[ 10 ];
    sprintf_s( pts, "%d", m_score );
    cv::putText( m_gameImg, pts, cv::Point( 10, 25 ), fontFace, fontScale, RED, fontThickness );
}

void SnakeGame::drawIntoTile( const int x, const int y, const cv::Mat& img )
{
    if( img.cols < m_tileSize )
    {
        const int diff = ( m_tileSize - img.cols ) / 2;
        img.copyTo( m_gameImg( cv::Rect( x + diff, y + diff, img.cols, img.rows ) ) );
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
