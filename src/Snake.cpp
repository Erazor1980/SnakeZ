#include "Snake.h"

//PlaySound( m_vFoodSounds[ m_currFoodIdx ].c_str(), NULL, SND_ASYNC );
SnakeGame::SnakeGame( const int tilesX, const int tilesY, const std::vector< cv::Mat > vPlayerImg, const std::vector< std::string > vPlayerNames,
                      const std::vector< cv::Mat > vFoodImg, const std::vector< std::string > vFoodSounds, const std::vector< PowerUp > vPowerUps,
                      const int tileSize, const bool easyMode )
{
    std::srand( ( unsigned int )std::time( 0 ) );

    m_tilesX        = tilesX;
    m_tilesY        = tilesY;
    m_tileSize      = tileSize;
    m_bEasyMode     = easyMode;
    mp_tailParts    = NULL;
    mp_tailColors   = NULL;
    
    assert( vPlayerImg.size() == vPlayerNames.size() );
    assert( vFoodImg.size() == vFoodSounds.size() );
    assert( vPowerUps.size() > 0 );

    for( int i = 0; i < vPlayerImg.size(); ++i )
    {
        cv::Mat currPlayerImg;
        cv::resize( vPlayerImg[ i ], currPlayerImg, cv::Size( m_tileSize, m_tileSize ) );
        m_vPlayerImg.push_back( currPlayerImg );
        m_vPlayerNames.push_back( vPlayerNames[ i ] );
    }
    
    //TODO wird ja aktuell nicht benutzt, allerdings muss die größe schon angepasst werden... 
    cv::resize( m_vPlayerImg[ 0 ], m_tailImg, cv::Size( m_tileSize - 10, m_tileSize - 10 ) );

    for( int i = 0; i < vFoodImg.size(); ++i )
    {
        cv::Mat currFoodImg;
        cv::resize( vFoodImg[ i ], currFoodImg, cv::Size( m_tileSize, m_tileSize ) );
        m_vFoodImg.push_back( currFoodImg );
        m_vFoodSounds.push_back( vFoodSounds[ i ] );
    }

    m_vPowerUps = vPowerUps;

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

void SnakeGame::startMenu( const std::string startSound, const std::string wndName )
{
    m_gameWndName       = wndName;
    int fontFace        = cv::FONT_HERSHEY_PLAIN;
    int fontScale       = 4;
    int fontThickness   = 3;

    if( startSound != "" )
    {
        //PlaySound( startSound.c_str(), NULL, SND_ASYNC );
    }
    
    const int numPlayers    = (int)m_vPlayerImg.size();
    const int spaceForImg   = m_gameImg.cols / numPlayers;
    const int imgSizeToShow = ( int )( spaceForImg * 0.4 );
    const int xStart        = ( spaceForImg - imgSizeToShow ) / 2;
    const int yStart        = 200;
    
    char lvlText[ 100 ];
    int speedLvl = 4;

    // selection bounding box and speed level
    const cv::Rect startBB( cv::Point2i( xStart - 20, yStart - 20 ), cv::Point2i( xStart + imgSizeToShow + 20, yStart + imgSizeToShow + 20 ) );
    int idx = 0;
    while( true )
    {
        // clear screen
        m_gameImg.setTo( cv::Scalar( 0, 0, 0 ) );

        // display game title
        cv::putText( m_gameImg, "S N A K E Z", cv::Point( m_gameImg.cols / 3, 100 ), fontFace, fontScale, GREEN, fontThickness );

        // display player images
        for( int i = 0; i < numPlayers; ++i )
        {
            cv::Mat currImg;
            cv::resize( m_vPlayerImg[ i ], currImg, cv::Size( imgSizeToShow, imgSizeToShow ) );
            currImg.copyTo( m_gameImg( cv::Rect( i * spaceForImg + xStart, yStart, currImg.cols, currImg.rows ) ) );
        }

        // display speed level
        if( speedLvl == 1 )
            m_timeMove = 0.7;
        else if( speedLvl == 2 )
            m_timeMove = 0.5;
        else if( speedLvl == 3 )
            m_timeMove = 0.3;
        else if( speedLvl == 4 )
            m_timeMove = 0.1;
        sprintf_s( lvlText, "(S)peed level: %d", speedLvl );
        cv::putText( m_gameImg, lvlText, cv::Point( m_gameImg.cols / 3, 350 + imgSizeToShow ), fontFace, 3, BLUE, fontThickness );

        // display bounding box
        cv::Rect bb = startBB;
        bb.x += idx * spaceForImg;
        cv::rectangle( m_gameImg, bb, WHITE, 3 );
        cv::imshow( m_gameWndName, m_gameImg );
        
        // key handling
        int key = cv::waitKey( 0 );
        if( 2424832 == key )        //  left
        {
            idx--;
            if( idx < 0 )
                idx = numPlayers - 1;
        }
        else if( 2555904 == key )   //  right
        {
            idx++;
            if( idx >= numPlayers )
                idx = 0;
        }
        else if( 's' == key || 'S' == key ) // speed level
        {
            speedLvl++;
            if( speedLvl > 4 )
                speedLvl = 1;
        }
        else if( 13 == key )        // enter
        {
            m_currPlayerIdx = idx;
            //TODO  hard coded... evtl später auslagern, erstmal kein bock ^^
            //      oder halt structs (wie bei power ups) für die player machen, mit sound, name usw
            if( m_vPlayerNames[ idx ] == "JULIA" )
            {
                PlaySound( "D:\\Projects\\_sounds_\\julia.wav", NULL, SND_ASYNC );
            }
            else if( m_vPlayerNames[ idx ] == "MELINA" )
            {
                PlaySound( "D:\\Projects\\_sounds_\\melina.wav", NULL, SND_ASYNC );
            }
            else if( m_vPlayerNames[ idx ] == "MAMA" )
            {
                PlaySound( "D:\\Projects\\_sounds_\\simone.wav", NULL, SND_ASYNC );
            }
            else if( m_vPlayerNames[ idx ] == "PAPA" )
            {
                PlaySound( "D:\\Projects\\_sounds_\\lukas.wav", NULL, SND_ASYNC );
            }

            break;
        }       
    }
    m_lastTimeMove = m_timeMove;
    startGame();
}

void SnakeGame::startGame()
{
    m_timerPU = clock();
}

void SnakeGame::resetGame()
{
    m_foodPos.x     = rand() % m_tilesX;
    m_foodPos.y     = rand() % m_tilesY;

    m_headPos       = cv::Point2i( m_tilesX / 2, m_tilesY / 2 );
    m_movDir        = RIGHT;
    m_lastDir       = RIGHT;
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

    m_timeTillNextPU    = rand() % 10 + MIN_TIME_FOR_NEXT_PU;
    m_timeMove          = m_lastTimeMove;
    //m_bActiveBoost      = false;
}

void SnakeGame::update()
{
    if( m_bGameOver )
    {
        int key = cv::waitKey( 1 );
        if( key > 0 )
        {
            if( key == 13 )         // enter = new game
            {
                resetGame();
                m_bGameOver = false;
            }
            else if( key == 27 )    // esc = end game
            {
                m_bFinishGame = true;
            }
            else if( key == 's' )   // 's' = start menu
            {
                resetGame();
                m_bGameOver = false;
                startMenu();
            }
        }
        return;
    }

    // power ups
    {
        if( !m_bPUvisibleOrActive )     /* no visible power up or active boost */
        {
            clock_t end         = clock();
            float elapsedTime   = ( float )( end - m_timerPU ) / CLOCKS_PER_SEC;
            if( elapsedTime > m_timeTillNextPU )
            {
                m_currPowerUp = rand() % m_vPowerUps.size();
                m_vPowerUps[ m_currPowerUp ].activate( this );
                m_bPUvisibleOrActive = true;
            }
        }
        else
        {
            if( !m_vPowerUps[ m_currPowerUp ].isAlive() )
            {
                //m_currPowerUp = -1;
                m_timerPU = clock();
                m_timeTillNextPU = rand() % 10 + MIN_TIME_FOR_NEXT_PU;
            }

            m_vPowerUps[ m_currPowerUp ].boostEndTest( this );
        }

    }

    // timer and position change
    if( m_bWaitForNextMove == false )
    {
        m_timerNextMove     = clock();
        m_bWaitForNextMove  = true;
    }
    else
    {
        clock_t end         = clock();
        float elapsedTime   = ( float )( end - m_timerNextMove ) / CLOCKS_PER_SEC;
        if( elapsedTime > m_timeMove )
        {
            updateTailParts();

            m_bWaitForNextMove = false;

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
        // power up check
        if( m_vPowerUps[ m_currPowerUp ].isAlive()  )
        {
            if( m_headPos == m_vPowerUps[ m_currPowerUp ].getPos() )
            {
                m_timerPU = clock();
                m_timeTillNextPU = rand() % 10 + MIN_TIME_FOR_NEXT_PU;

                m_vPowerUps[ m_currPowerUp ].enableBoost( this );
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
                PlaySound( "D:\\Projects\\_sounds_\\gameover.wav", NULL, SND_ASYNC );
                return;
            }
        }

        // collision check (with tail)
        for( int i = 0; i < m_numTailParts; ++i )
        {
            if( m_headPos == mp_tailParts[ i ] )
            {
                m_bGameOver = true;
                PlaySound( "D:\\Projects\\_sounds_\\gameover.wav", NULL, SND_ASYNC );
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
    drawIntoTile( x, y, m_vPlayerImg[ m_currPlayerIdx ] );

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

    // draw power up
    if( m_vPowerUps[ m_currPowerUp ].isAlive() )
    {
        x = m_vPowerUps[ m_currPowerUp ].getPos().x * m_tileSize;
        y = m_vPowerUps[ m_currPowerUp ].getPos().y * m_tileSize;
        drawIntoTile( x, y, m_vPowerUps[ m_currPowerUp ].getImg() );
    }

    // draw border
    if( m_bEasyMode )
    {
        cv::rectangle( m_gameImg, cv::Rect( 0, 0, m_gameImg.cols - 1, m_gameImg.rows - 1 ), GREEN, 3 );
    }
    else
    {
        cv::rectangle( m_gameImg, cv::Rect( 0, 0, m_gameImg.cols - 1, m_gameImg.rows - 1 ), RED, 3 );
    }
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

    const int x = m_gameImg.cols / 3;

    cv::putText( m_gameImg, "GAME OVER", cv::Point( x, m_gameImg.rows / 2 ), fontFace, fontScale, GREEN, fontThickness );
    cv::putText( m_gameImg, "Close game -> press ESC", cv::Point( x, m_gameImg.rows / 2 + 40 ), fontFace, 1.5, RED, 2 );
    cv::putText( m_gameImg, "New game   -> press Enter", cv::Point( x, m_gameImg.rows / 2 + 70 ), fontFace, 1.5, RED, 2 );
    cv::putText( m_gameImg, "Start menu -> press 's'", cv::Point( x, m_gameImg.rows / 2 + 100 ), fontFace, 1.5, RED, 2 );
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
    else if( 'p' == keyCode )       // pause game
    {
        cv::waitKey( 0 );
    }
}
