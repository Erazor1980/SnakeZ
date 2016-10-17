#pragma once
#include "defines.h"

class SnakeGame;

class Food
{
public:
    Food( const cv::Mat* pImg, const cv::Point2i pos, const std::string sound );
    ~Food();

    void playSound();
    void draw( SnakeGame* pSnakeGame );
    cv::Point2i getPos() const
    {
        return m_pos;
    }

private:
    const cv::Mat*      mp_img;
    cv::Point2i         m_pos;      /* position of the food in tiles (x, y) */
    std::string         m_sound;    /* full path including the file name (.wav) */
};

