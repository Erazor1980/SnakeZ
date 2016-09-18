#include <stdio.h>
#include "Snake.h"

int main()
{
    // load food images and sounds
    std::vector< cv::Mat > vFoodImg;
    vFoodImg.push_back( cv::imread( "D:/Projects/_images_/chicken.png", 1 ) );
    vFoodImg.push_back( cv::imread( "D:/Projects/_images_/iceCream.png", 1 ) );
    vFoodImg.push_back( cv::imread( "D:/Projects/_images_/soap.png", 1 ) );
    std::vector< std::string > vFoodSounds;
    vFoodSounds.push_back( "D:\\Projects\\_sounds_\\papaKauen.wav" );
    vFoodSounds.push_back( "D:\\Projects\\_sounds_\\icecream.wav" );
    vFoodSounds.push_back( "D:\\Projects\\_sounds_\\blow.wav" );

    // load player images
    std::vector< cv::Mat > vPlayerImg;
    vPlayerImg.push_back( cv::imread( "D:/Projects/_images_/julia.png", 1 ) );
    vPlayerImg.push_back( cv::imread( "D:/Projects/_images_/melina.png", 1 ) );
    vPlayerImg.push_back( cv::imread( "D:/Projects/_images_/mama.png", 1 ) );
    vPlayerImg.push_back( cv::imread( "D:/Projects/_images_/papa.png", 1 ) );

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
    vPowerUps.push_back( PowerUp( "rocket", cv::imread( "D:/Projects/_images_/rocket.png", 1 ), lifeTime, boostTime,
                                  "D:\\Projects\\_sounds_\\rocketShowUp.wav", "D:\\Projects\\_sounds_\\rocketConsume.wav" ) );

    //TODO evtl teste einbauen, ob alles richtig geladen wurde...

	cv::namedWindow( "SnakeZ", cv::WINDOW_AUTOSIZE );
    SnakeGame myGame( 15, 10, vPlayerImg, vPlayerNames, vFoodImg, vFoodSounds, vPowerUps, 100, true );

    myGame.startMenu( "D:\\Projects\\_sounds_\\start.wav", "SnakeZ" );

    while( !myGame.finishGame() )
    {
        myGame.update();
        //TODO getGamImg vielleicht durch displayGame oder so ersetzen! und innerhalb der klasse anzeigen!
        cv::imshow( "SnakeZ", myGame.getGameImg() );
    }
    
    cv::destroyAllWindows();

	return 0;
}
