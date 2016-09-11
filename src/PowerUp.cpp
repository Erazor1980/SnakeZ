#include "PowerUp.h"
#include "Snake.h"

PowerUp::PowerUp( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime, 
                  const std::string showUpSound, const std::string consumeSound )
{
    m_name          = name;
    m_lifeTime      = lifeTime;
    m_boostTime     = boostTime;
    m_bVisible      = false;
    m_bBoostOn      = false;
    m_showUpSound   = showUpSound;
    m_consumeSound  = consumeSound;
    img.copyTo( m_img );
}

void PowerUp::activate( const SnakeGame* pGame )
{
    m_timerLife     = clock();
    m_bVisible      = true;
    m_bBoostOn      = false;
    //TODO add free tile detection and coordinate assignment
    m_pos.x  = rand() % pGame->m_tilesX;
    m_pos.y  = rand() % pGame->m_tilesY;
    PlaySound( m_showUpSound.c_str(), NULL, SND_ASYNC );
}

bool PowerUp::isAlive()
{

    if( !m_bVisible )
    {
        return false;
    }
    clock_t end         = clock();
    float elapsedTime   = ( float )( end - m_timerLife ) / CLOCKS_PER_SEC;

    if( elapsedTime > m_lifeTime )
    {
        m_bVisible = false;
        return false;
    }
    else
    {
        return true;
    }
}

void PowerUp::enableBoost( SnakeGame * pGame )
{
    PlaySound( m_consumeSound.c_str(), NULL, SND_ASYNC );
    //TODO mit vererbung arbeiten und abgeleitete klassen für einzelne PU typen erstellen! die ändern dann diese methode!
    pGame->m_timeMove /= 2;

    m_timerBoost    = clock();
    m_bBoostOn      = true;
    m_bVisible      = false;
}

void PowerUp::boostEndTest( SnakeGame * pGame )
{
    if( !m_bBoostOn )
        return;

    clock_t end         = clock();
    float elapsedTime   = ( float )( end - m_timerLife ) / CLOCKS_PER_SEC;
    if( elapsedTime > m_boostTime )
    {
        pGame->m_timeMove = pGame->m_lastTimeMove;
        pGame->m_bPUvisibleOrActive = false;
        m_bBoostOn = false;
    }
}

void PowerUp::resetPowerUp()
{
    m_bBoostOn = false;
    m_bVisible = false;
}
