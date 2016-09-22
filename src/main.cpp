#include <stdio.h>
#include "Snake.h"

int main()
{
    // load food images and sounds
    std::vector< cv::Mat > vFoodImg;
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "chicken.png", 1 ) );
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "iceCream.png", 1 ) );
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "soap.png", 1 ) );
    std::vector< std::string > vFoodSounds;
    vFoodSounds.push_back( PATH_TO_SOUNDS + "papaKauen.wav" );
    vFoodSounds.push_back( PATH_TO_SOUNDS + "icecream.wav" );
    vFoodSounds.push_back( PATH_TO_SOUNDS + "blow.wav" );

    // load player images
    std::vector< cv::Mat > vPlayerImg;
    vPlayerImg.push_back( cv::imread( PATH_TO_IMAGES + "julia.png", 1 ) );
    vPlayerImg.push_back( cv::imread( PATH_TO_IMAGES + "melina.png", 1 ) );
    vPlayerImg.push_back( cv::imread( PATH_TO_IMAGES + "mama.png", 1 ) );
    vPlayerImg.push_back( cv::imread( PATH_TO_IMAGES + "papa.png", 1 ) );

    // add player names
    std::vector< std::string > vPlayerNames;
    vPlayerNames.push_back( "JULIA" );
    vPlayerNames.push_back( "MELINA" );
    vPlayerNames.push_back( "MAMA" );
    vPlayerNames.push_back( "PAPA" );

    // add powerups
    int lifeTime = 10;
    int boostTime = 15;
    std::vector< PowerUp > vPowerUps;
    vPowerUps.push_back( PowerUp( "rocket", cv::imread( PATH_TO_IMAGES + "rocket.png", 1 ), lifeTime, boostTime,
                                  PATH_TO_SOUNDS + "rocketShowUp.wav", PATH_TO_SOUNDS + "rocketConsume.wav" ) );


	cv::namedWindow( "SnakeZ", cv::WINDOW_AUTOSIZE );
    SnakeGame myGame( 15, 10, vPlayerImg, vPlayerNames, vFoodImg, vFoodSounds, vPowerUps, 100, true );

    myGame.startMenu( PATH_TO_SOUNDS + "start.wav", "SnakeZ" );

    while( !myGame.finishGame() )
    {
        myGame.update();
        //TODO getGamImg vielleicht durch displayGame oder so ersetzen! und innerhalb der klasse anzeigen!
        cv::imshow( "SnakeZ", myGame.getGameImg() );
    }
    
    cv::destroyAllWindows();

	return 0;
}
