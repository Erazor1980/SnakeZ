#include <stdio.h>
#include "Snake.h"

int main()
{
    // load food images and sounds
    std::vector< cv::Mat > vFoodImg;
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "chicken.png", 1 ) );
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "iceCream.png", 1 ) );
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "soap.png", 1 ) );
    vFoodImg.push_back( cv::imread( PATH_TO_IMAGES + "coin.png", 1 ) );     // HAS TO BE LAST!!!!

    std::vector< std::string > vFoodSounds;
    vFoodSounds.push_back( PATH_TO_SOUNDS + "papaKauen.wav" );
    vFoodSounds.push_back( PATH_TO_SOUNDS + "icecream.wav" );
    vFoodSounds.push_back( PATH_TO_SOUNDS + "blow.wav" );
    vFoodSounds.push_back( PATH_TO_SOUNDS + "coinFlip.wav" );               // HAS TO BE LAST!!!!

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

    /*Rocket test( "rocket", cv::imread( PATH_TO_IMAGES + "rocket.png", 1 ), 11, 12,
                 PATH_TO_SOUNDS + "rocketShowUp.wav", PATH_TO_SOUNDS + "rocketConsume.wav" );*/

    // add powerups
    int lifeTime = 10;
    int boostTime = 15;
    std::vector< PowerUp* > vPowerUps;
    PowerUp* myRocket = new Rocket( "rocket", cv::imread( PATH_TO_IMAGES + "rocket.png", 1 ), lifeTime, boostTime,
                                  PATH_TO_SOUNDS + "rocketShowUp.wav", PATH_TO_SOUNDS + "rocketConsume.wav" );
    //vPowerUps.push_back( myRocket );

    PowerUp* myChest = new Chest( "chest", cv::imread( PATH_TO_IMAGES + "chest.png", 1 ), lifeTime, boostTime,
                                   PATH_TO_SOUNDS + "fanfare.wav", PATH_TO_SOUNDS + "floop.wav" );
    vPowerUps.push_back( myChest );

    
#if 1
	cv::namedWindow( "SnakeZ", cv::WINDOW_AUTOSIZE );
    SnakeGame myGame( 15, 10, vPlayerImg, vPlayerNames, vFoodImg, vFoodSounds, vPowerUps, 100, true );

    myGame.startMenu( PATH_TO_SOUNDS + "start.wav", "SnakeZ" );

    while( !myGame.finishGame() )
    {
        myGame.update();
        //TODO getGamImg vielleicht durch displayGame oder so ersetzen! und innerhalb der klasse anzeigen!
        cv::imshow( "SnakeZ", myGame.getGameImg() );
    }
#endif
    cv::destroyAllWindows();

    // clear memory
    for( auto it = vPowerUps.begin(); it != vPowerUps.end(); ++it )
    {
        delete ( *it );
    }
    vPowerUps.clear();
	return 0;
}
