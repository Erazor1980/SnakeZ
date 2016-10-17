#include "Food.h"
#include "Snake.h"

Food::Food( const cv::Mat * pImg, const cv::Point2i pos, const std::string sound )
{
    mp_img  = pImg;
    m_pos   = pos;
    m_sound = sound;
}

Food::~Food()
{
}

void Food::playSound()
{
    PlaySound( m_sound.c_str(), NULL, SND_ASYNC );
}

void Food::draw( SnakeGame* pSnakeGame )
{
    pSnakeGame->drawIntoTile( m_pos.x * pSnakeGame->m_tileSize, m_pos.y * pSnakeGame->m_tileSize, *mp_img );
}
