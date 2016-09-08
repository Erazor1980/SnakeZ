#include <stdio.h>
#include "Snake.h"

//void updateMap( cv::Mat& img, const Ball& ball );

int main()
{
	
	cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/julia.png", 1 );
	//cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/bvb.png", 1 );
	//cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/melina.png", 1 );
	//cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/luki.png", 1 );
	if( snakeHead.empty() )
	{
		printf( "No image data.\n" );
		return -1;
	}

	cv::namedWindow( "Snake", cv::WINDOW_AUTOSIZE );

    SnakeGame myGame( 15, 10, snakeHead, 50, true );


    while( !myGame.gameOver() )
    {
        cv::imshow( "Snake", myGame.getGameImg() );

        myGame.update();        
    }
    
	return 0;
}
