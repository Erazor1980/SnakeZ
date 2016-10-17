#pragma once
#include "defines.h"

#define NEW_FOOD_FROM_CHEST (5)

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
    virtual void update();
    virtual void draw();

    cv::Point2i getPos() const
    {
        return m_pos;
    }
    std::string getName() const
    {
        return m_name;
    }
protected:
    void calcNewTime();

    virtual void enableBoostEffect() = 0;
    virtual void disableBoostEffect() = 0;

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

class Rocket : public PowerUp
{
public:
    Rocket( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
             const std::string showUpSound, const std::string consumeSound );

    void draw();
private:
    void enableBoostEffect();
    void disableBoostEffect();
};

class Chest : public PowerUp
{
public:
    Chest( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
           const std::string showUpSound, const std::string consumeSound );

    void draw();
    void update();
private:
    void enableBoostEffect();
    void disableBoostEffect();
};