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

    m_overallScoreBoard.loadFromFile();
}

SnakeGame::~SnakeGame()
{
    if( mp_tailParts )
        delete[] mp_tailParts;
    if( mp_tailColors )
        delete[] mp_tailColors;

    if( m_overallScoreBoard.isNotEmpty() )
        m_overallScoreBoard.writeToFile();
}

void SnakeGame::startMenu( const std::string startSound, const std::string wndName )
{
    m_gameWndName       = wndName;
    int fontFace        = cv::FONT_HERSHEY_PLAIN;
    int fontScale       = 4;
    int fontThickness   = 3;

    if( startSound != "" )
    {
        PlaySound( startSound.c_str(), NULL, SND_ASYNC );
    }
    
    const int numPlayers    = (int)m_vPlayerImg.size();
    const int spaceForImg   = m_gameImg.cols / numPlayers;
    const int imgSizeToShow = ( int )( spaceForImg * 0.4 );
    const int xStart        = ( spaceForImg - imgSizeToShow ) / 2;
    const int yStart        = 200;
    
    char lvlText[ 100 ];
    int speedLvl = 2;

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
        sprintf_s( lvlText, "(L)evel: %d", speedLvl );
        cv::putText( m_gameImg, lvlText, cv::Point( m_gameImg.cols / 3 + 40, 300 + imgSizeToShow ), fontFace, 3, BLUE, fontThickness );

        cv::putText( m_gameImg, "Start           -> ENTER", cv::Point( m_gameImg.cols / 3, 400 + imgSizeToShow ), fontFace, 2, CYAN, 2 );
        cv::putText( m_gameImg, "Highscore list  -> 'h'", cv::Point( m_gameImg.cols / 3, 430 + imgSizeToShow ), fontFace, 2, CYAN, 2 );

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
        else if( 'l' == key || 'L' == key ) // speed level
        {
            speedLvl++;
            if( speedLvl > 4 )
                speedLvl = 1;
        }
        else if( 'h' == key || 'H' == key ) // show scoreboard
        {
            displayHighScore( m_overallScoreBoard );
            //TODO
        }
        else if( 27 == key )        // esc - finish game
        {
            m_bFinishGame = true;
            return;
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

    m_timeMove          = m_lastTimeMove;

    for( int i = 0; i < m_vPowerUps.size(); ++i )
    {
        m_vPowerUps[ i ].init( this, MIN_TIME_FOR_NEXT_PU, MAX_TIME_FOR_NEXT_PU );
    }
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
            else if( key == 'h' )   // 'h' = high score list
            {
                displayHighScore( m_overallScoreBoard );
            }
        }
        return;
    }

    // power ups
    //TODO m_currPowerUp ist immer 0 bis jetzt, muss noch irgendwo geändert werden! oder halt mehrere erlauben, die selbst ihre zeiten regeln!
    m_vPowerUps[ m_currPowerUp ].update();

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

        // food check
        if( m_headPos == m_foodPos )
        {
            PlaySound( m_vFoodSounds[ m_currFoodIdx ].c_str(), NULL, SND_ASYNC );

            // find a free tile for the new food placement
            m_foodPos = findFreeTile( false );

            mp_tailColors[ m_numTailParts ] = cv::Scalar( rand() % 255, rand() % 255, rand() % 255 );
            //mp_tailColors[ m_numTailParts ] = cv::Scalar( 200, 200, 200 );
            m_numTailParts++;
            m_score += m_addedScoreNumber;
            
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
                int posHighScore = m_overallScoreBoard.addScore( m_vPlayerNames[ m_currPlayerIdx ], m_score );
                if( 0 == posHighScore )
                {
                    PlaySound( "D:\\Projects\\_sounds_\\highScore.wav", NULL, SND_ASYNC );
                }
                else
                {
                    PlaySound( "D:\\Projects\\_sounds_\\gameover.wav", NULL, SND_ASYNC );
                }
                if( posHighScore >= 0 )
                    displayHighScore( m_overallScoreBoard, posHighScore );
                return;
            }
        }

        // collision check (with tail)
        for( int i = 0; i < m_numTailParts; ++i )
        {
            if( m_headPos == mp_tailParts[ i ] )
            {
                m_bGameOver = true;
                int posHighScore = m_overallScoreBoard.addScore( m_vPlayerNames[ m_currPlayerIdx ], m_score );
                if( 0 == posHighScore )
                {
                    PlaySound( "D:\\Projects\\_sounds_\\highScore.wav", NULL, SND_ASYNC );
                }
                else
                {
                    PlaySound( "D:\\Projects\\_sounds_\\gameover.wav", NULL, SND_ASYNC );
                }
                if( posHighScore >= 0 )
                    displayHighScore( m_overallScoreBoard, posHighScore );
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

    m_vPowerUps[ m_currPowerUp ].draw();

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
    cv::putText( m_gameImg, "High Score -> press 'h'", cv::Point( x, m_gameImg.rows / 2 + 130 ), fontFace, 1.5, RED, 2 );
}

void SnakeGame::displayHighScore( const ScoreBoard& sb, const int posHS )
{
    int fontFace        = cv::FONT_HERSHEY_PLAIN;
    int fontScale       = 4;
    int fontThickness   = 3;

    std::vector<Node> vNodes = sb.getScoreBoardList();

    // fill up the list with default values, if its too small
    while( vNodes.size() < MAX_NUMBER_SCORES )
    {
        vNodes.push_back( Node( "---", 0 ) );
    }

    cv::Mat imgHS( m_gameImg.size(), CV_8UC3 );

    while( true )
    {
        // clear screen
        imgHS.setTo( cv::Scalar( 0, 0, 0 ) );

        // display highscore title
        cv::putText( imgHS, "H I G H S C O R E", cv::Point( imgHS.cols / 3 - 20, 100 ), fontFace, fontScale, GREEN, fontThickness );

        // display highscore list
        const int startX    = imgHS.cols / 5;
        const int startY    = 200;
        const int diffY     = 70;
        for( int i = 0; i < vNodes.size(); ++i )
        {
            cv::Scalar color = WHITE;
            if( i == posHS )
                color = RED;
            cv::putText( imgHS, std::to_string( i + 1 ) + ":", cv::Point( startX, startY + i * diffY ), fontFace, 3, color, fontThickness );
            cv::putText( imgHS, vNodes[ i ].m_name, cv::Point( startX + 150, startY + i * diffY ), fontFace, 3, color, fontThickness );
            cv::putText( imgHS, std::to_string( vNodes[ i ].m_points ), cv::Point( startX + 550, startY + i * diffY ), fontFace, 3, color, fontThickness );
        }

        cv::imshow( m_gameWndName, imgHS );

        int key = cv::waitKey( 0 );

        if( 2424832 == key )        //  left
        {
            //TODO add personal highscores, and display them
        }
        else
        {
            return;
        }
    }

}

cv::Point2i SnakeGame::findFreeTile( bool isPU )
{
    cv::Point2i freeTile;
    while( true )
    {
        freeTile.x = rand() % m_tilesX;
        freeTile.y = rand() % m_tilesY;
        bool occupiedTile = false;
        // compare to head position
        if( freeTile == m_headPos )
        {
            continue;
        }

        // compare to food position (we are power up)
        if( isPU )
        {
            if( freeTile == m_foodPos )
            {
                continue;
            }
        }
        // compare to power up position (we are food)
        else
        {
            if( freeTile == m_vPowerUps[ m_currPowerUp ].getPos() )
            {
                continue;
            }
        }
        
        for( int i = 0; i < m_numTailParts; ++i )
        {
            if( freeTile == mp_tailParts[ i ] )
            {
                occupiedTile = true;
                break;
            }
        }
        if( !occupiedTile )
            break;
    }   

    return freeTile;
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
