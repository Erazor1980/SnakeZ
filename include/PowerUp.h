#pragma once
#include "defines.h"

class SnakeGame;

class PowerUp
{
public:
    PowerUp( const cv::String name, const cv::Mat img, const int lifeTime, const int boostTime,
             const std::string showUpSound, const std::string consumeSound );
    ~PowerUp() {};

    // PU is activated and will appear on a random free position on the screen
    void activate( const SnakeGame* pGame );

    // is the PU still living (on the screen)?
    bool isAlive();

    // the PU boost is enabled, PU dissapears from the screen
    void enableBoost( SnakeGame* pGame );

    // check, if boost ends, if yes, disable its effect
    void boostEndTest( SnakeGame* pGame );

    cv::Point2i getPos() const
    {
        return m_pos;
    }
    cv::Mat getImg() const
    {
        return m_img;
    }
private:
    cv::String  m_name;
    cv::Mat     m_img;
    cv::Point2i m_pos;          /* position of the power up in tiles (x, y) */
    int         m_boostTime;    /* how long will the effect of the power up last in seconds */

    bool        m_bVisible;     /* PU on the screen, but not "eaten" */
    bool        m_bBoostOn;     /* boost activated */

    int         m_lifeTime;     /* how long will the power up remain in seconds */
    clock_t     m_timerLife;    /* for tracking its own "living time" */
    clock_t     m_timerBoost;
    std::string m_showUpSound;
    std::string m_consumeSound;
};
