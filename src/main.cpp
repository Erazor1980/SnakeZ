#include <stdio.h>
#include "Snake.h"

//void updateMap( cv::Mat& img, const Ball& ball );

int main()
{
	
	
    cv::Mat icecream = cv::imread( "D:/Projects/_images_/iceCream.png", 1 );
    //cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/julia.png", 1 );
	cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/bvb.png", 1 );
	//cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/melina.png", 1 );
	//cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/luki.png", 1 );
	if( snakeHead.empty() || icecream.empty() )
	{
		printf( "No image data.\n" );
		return -1;
	}

	cv::namedWindow( "Snake", cv::WINDOW_AUTOSIZE );

    SnakeGame myGame( 15, 10, snakeHead, icecream, 50, true );

    while( !myGame.finishGame() )
    {
        myGame.update();
        cv::imshow( "Snake", myGame.getGameImg() );
    }
    
	return 0;
}
