#include "PowerUp.h"
#include "Snake.h"

PowerUp::PowerUp( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
                  const std::string showUpSound, const std::string consumeSound )
{
    m_name          = name;
    m_lifeTime      = lifeTime;
    m_boostTime     = boostTime;
    m_showUpSound   = showUpSound;
    m_consumeSound  = consumeSound;
    img.copyTo( m_img );
    mp_snakeGame    = NULL;
}

void PowerUp::init( SnakeGame* pSnakeGame, int minTime, int maxTime )
{
    mp_snakeGame    = pSnakeGame;
    m_minTime       = minTime;
    m_maxTime       = maxTime;

    m_timer         = -1;
    m_state         = _WAIT;

    calcNewTime();
}

void PowerUp::update()
{
    assert( mp_snakeGame != NULL );

    if( _WAIT == m_state )
    {
        if( m_timer < 0 )
        {
            // countdown till new PU life starts
            m_timer = clock();
            calcNewTime();
        }
        else
        {
            clock_t end         = clock();
            float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;
            if( elapsedTime > m_newTime )
            {
                // create new PU
                m_pos = mp_snakeGame->findFreeTile();
                PlaySound( m_showUpSound.c_str(), NULL, SND_ASYNC );

                m_timer = -1;
                m_state = _VISIBLE;
            }
        }
    }
    else if( _VISIBLE == m_state )
    {
        if( m_timer < 0 )
        {
            m_timer = clock();
        }
        else
        {
            clock_t end         = clock();
            float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;
            if( elapsedTime > m_lifeTime )
            {
                // PU life time expired (without beeing consumed)
                m_timer  = -1;
                m_state = _WAIT;
            }
            else
            {
                // collision check with head
                if( mp_snakeGame->m_headPos == m_pos )
                {
                    // PU consumed!
                    m_timer = -1;
                    m_state = _BOOST;
                    mp_snakeGame->m_score += 2;
                    PlaySound( m_consumeSound.c_str(), NULL, SND_ASYNC );
                    enableBoostEffect();
                }
            }
        }
    }
    else if( _BOOST == m_state )
    {
        if( m_timer < 0 )
        {
            m_timer = clock();
        }
        else
        {
            clock_t end         = clock();
            float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;
            if( elapsedTime > m_boostTime )
            {
                m_timer = -1;
                m_state = _WAIT;
                disableBoostEffect();
            }
        }
    }
}

void PowerUp::calcNewTime()
{
    if( m_minTime == m_maxTime )
    {
        m_newTime = m_minTime;
    }
    else
    {
        m_newTime = m_minTime + rand() % ( m_maxTime - m_minTime );
    }
}

void PowerUp::draw()
{
#if DEBUG_INFOS
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = 1.5;
    int fontThickness = 2;
    char infosPU[ 255 ];
    clock_t end         = clock();
    float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;
    sprintf_s( infosPU, "PowerUp status: %d, timer: %0.2f", ( int )m_state, elapsedTime );
    cv::putText( mp_snakeGame->m_gameImg, infosPU, cv::Point( 300, 25 ), fontFace, fontScale, CYAN, fontThickness );
#endif
    if( m_state == _VISIBLE )
    {
        int x = m_pos.x * mp_snakeGame->m_tileSize;
        int y = m_pos.y * mp_snakeGame->m_tileSize;
        mp_snakeGame->drawIntoTile( x, y, m_img );
    }
}

/////////////////////
////// ROCKET ///////
/////////////////////
Rocket::Rocket( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
                const std::string showUpSound, const std::string consumeSound )
    :
    PowerUp( name, img, lifeTime, boostTime, showUpSound, consumeSound )
{}

void Rocket::enableBoostEffect()
{
    mp_snakeGame->m_timeMove           /= 2;
    mp_snakeGame->m_addedScoreNumber   = 2;
}

void Rocket::disableBoostEffect()
{
    mp_snakeGame->m_timeMove           = mp_snakeGame->m_lastTimeMove;
    mp_snakeGame->m_addedScoreNumber   = 1;
}

void Rocket::draw()
{
    PowerUp::draw();

    if( m_state == _BOOST )
    {
        int x = mp_snakeGame->m_headPos.x * mp_snakeGame->m_tileSize;
        int y = mp_snakeGame->m_headPos.y * mp_snakeGame->m_tileSize;
        cv::rectangle( mp_snakeGame->m_gameImg, cv::Rect( x, y, mp_snakeGame->m_vPlayerImg[ mp_snakeGame->m_currPlayerIdx ].cols - 1,
                                                          mp_snakeGame->m_vPlayerImg[ mp_snakeGame->m_currPlayerIdx ].rows - 1 ), RED, 3 );
    }
}

////////////////////
////// CHEST ///////
////////////////////
Chest::Chest( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
              const std::string showUpSound, const std::string consumeSound )
    :
    PowerUp( name, img, lifeTime, boostTime, showUpSound, consumeSound )
{}

void Chest::enableBoostEffect()
{
    // remove food
    mp_snakeGame->m_vFoodInGame.clear();
    
    // create coins
    const int idx = (int)mp_snakeGame->m_vFoodImg.size() - 1;    // coin is last in the foodImg- and foodSound vector!

    for( int i = 0; i < NEW_FOOD_FROM_CHEST; ++i )
    {
        Food coin( &mp_snakeGame->m_vFoodImg[ idx ], "coin", mp_snakeGame->findFreeTile(), mp_snakeGame->m_vFoodSounds[ idx ] );
        mp_snakeGame->m_vFoodInGame.push_back( coin );
    }
    
    // more points for collecting coins
    mp_snakeGame->m_addedScoreNumber = 3;
}

void Chest::disableBoostEffect()
{
    // remove all coins
    mp_snakeGame->m_vFoodInGame.clear();

    // create new random food
    mp_snakeGame->addRandomFood();

    // reset points for collecting food
    mp_snakeGame->m_addedScoreNumber = 1;
}

void Chest::update()
{
    PowerUp::update();

    // change to _WAIT state, when all coins are collected
    if( m_state == _BOOST )
    {
        if( mp_snakeGame->m_vFoodInGame[ mp_snakeGame->m_vFoodInGame.size() - 1 ].getName() != "coin" )
        {
            m_timer = -1;
            m_state = _WAIT;
        }
    }
}

void Chest::draw()
{
    PowerUp::draw();

    // draw fuse
    if( _BOOST == m_state )
    {
        clock_t end         = clock();
        float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;

        float timeLeftPerc = 1 - ( elapsedTime / m_boostTime );

        const int length = ( int )( ( mp_snakeGame->m_gameImg.cols - 10 ) * timeLeftPerc );
        cv::Point2i p1( 5, 5 );
        cv::Point2i p2( 5 + length, 5 );
        cv::line( mp_snakeGame->m_gameImg, p1, p2, RED, 4 );
    }
}
