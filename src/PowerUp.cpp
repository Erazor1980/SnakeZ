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

void PowerUp::init( SnakeGame * pSnakeGame, int minTime, int maxTime )
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
                m_pos = mp_snakeGame->findFreeTile( true );
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

void PowerUp::draw()
{
#if DEBUG_INFOS
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = 1.5;
    int fontThickness = 2;
    char infosPU[ 255 ];
    clock_t end         = clock();
    float elapsedTime   = ( float )( end - m_timer ) / CLOCKS_PER_SEC;
    sprintf_s( infosPU, "PowerUp status: %d, timer: %0.2f", (int)m_state, elapsedTime );
    cv::putText( mp_snakeGame->m_gameImg, infosPU, cv::Point( 300, 25 ), fontFace, fontScale, CYAN, fontThickness );
#endif
    if( m_state == _BOOST )
    {
        int x = mp_snakeGame->m_headPos.x * mp_snakeGame->m_tileSize;
        int y = mp_snakeGame->m_headPos.y * mp_snakeGame->m_tileSize;
        cv::rectangle( mp_snakeGame->m_gameImg, cv::Rect( x, y, mp_snakeGame->m_vPlayerImg[ mp_snakeGame->m_currPlayerIdx ].cols - 1, 
                                                          mp_snakeGame->m_vPlayerImg[ mp_snakeGame->m_currPlayerIdx ].rows - 1 ), RED, 3 );
    }
    else if( m_state == _VISIBLE )
    {
        int x = m_pos.x * mp_snakeGame->m_tileSize;
        int y = m_pos.y * mp_snakeGame->m_tileSize;
        mp_snakeGame->drawIntoTile( x, y, m_img );
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

void PowerUp::enableBoostEffect()
{
    //TODO mit vererbung arbeiten und abgeleitete klassen für einzelne PU typen erstellen! die ändern dann diese methode!
    mp_snakeGame->m_timeMove           /= 2;
    mp_snakeGame->m_addedScoreNumber   = 2;
}

void PowerUp::disableBoostEffect()
{
    //TODO mit vererbung arbeiten und abgeleitete klassen für einzelne PU typen erstellen! die ändern dann diese methode!
    mp_snakeGame->m_timeMove           = mp_snakeGame->m_lastTimeMove;
    mp_snakeGame->m_addedScoreNumber   = 1;
}
