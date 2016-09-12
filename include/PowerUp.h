#pragma once
#include "defines.h"

class SnakeGame;

enum ePU_State
{
    _WAIT = 0,
    _VISIBLE = 1,
    _BOOST = 2
};

class PowerUp
{
public:
    PowerUp( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
             const std::string showUpSound, const std::string consumeSound );
    ~PowerUp() {};

    // new PowerUp between minTime and maxTime
    void init( SnakeGame* pSnakeGame, int minTime = 5, int maxTime = 5 );
    void update();
    void draw();

    cv::Point2i getPos() const
    {
        return m_pos;
    }
private:
    void calcNewTime();

    //TODO für die vererbung! nur diese beiden methoden muss geändert werden!
    void enableBoostEffect();
    void disableBoostEffect();

    cv::String  m_name;
    cv::Mat     m_img;
    cv::Point2i m_pos;          /* position of the power up in tiles (x, y) */
    int         m_boostTime;    /* how long will the effect of the power up last in seconds */
    int         m_lifeTime;     /* how long will the power up remain in seconds */
    int         m_newTime;      /* how long after end of boost/end of life till new life in seconds */

    int         m_minTime;      /* to calculate a random m_newTime (between minTime and maxTime) */
    int         m_maxTime;      /* to calculate a random m_newTime (between minTime and maxTime) */

    clock_t     m_timer;        /* for tracking time for each state */

    ePU_State   m_state;        /* in which state is the PU: waiting (for new life), visible or boost */

    std::string m_showUpSound;
    std::string m_consumeSound;

    SnakeGame*  mp_snakeGame;
};
