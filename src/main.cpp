#include <stdio.h>
#include "Snake.h"

//void updateMap( cv::Mat& img, const Ball& ball );

int main()
{
	cv::Mat image;
	
	//image = cv::imread("C:\\Users\\Erazor1980\\Desktop\\CPP_STUFF\\julia.jpg", 1);
	//cv::Mat ballImg = cv::imread( "D:/Projects/julia.png", 1 );
	cv::Mat snakeHead = cv::imread( "D:/Projects/_images_/bvb.png", 1 );
	//cv::Mat ballImg = cv::imread( "D:/Projects/melina.png", 1 );
	//cv::Mat ballImg = cv::imread( "D:/Projects/_images_/luki.png", 1 );
	if( snakeHead.empty() )
	{
		printf( "No image data.\n" );
		return -1;
	}

	cv::namedWindow( "Snake", cv::WINDOW_AUTOSIZE );

    SnakeGame myGame( 800, 600, snakeHead );


    while( !myGame.gameOver() )
    {
        cv::imshow( "Snake", myGame.getGameImg() );

        //int key = cv::waitKey( 1 );
        myGame.update();
    }
    

	//while ( true )
	//{		
	//	updateMap( img, ball );
	//	cv::imshow( "JumperZ", img );

	//	int key = cv::waitKey(1);
	//	if (key == 27)
	//	{
	//		break;
	//	}
	//	else if ('w' == key)
	//	{
	//		moveSpeed++;
	//		ball.setMoveSpeed( moveSpeed );
	//	}
	//	else if ('s' == key)
	//	{
	//		moveSpeed /= 2;
	//		ball.setMoveSpeed( moveSpeed );
	//	}
	//	else if ('a' == key)
	//	{
	//		ball.setDirection( LEFT );
	//	}
	//	else if ('d' == key)
	//	{
	//		ball.setDirection( RIGHT );
	//	}
	//	else if( 32 == key )	//space -> jump
	//	{
	//		ball.jump();
	//	}
	//	else if( '+' == key )
	//	{
	//		jumpSpeed += 2;
	//		ball.setJumpStartSpeed( jumpSpeed );
	//	}
	//	else if( '-' == key )
	//	{
	//		jumpSpeed -= 2;
	//		ball.setJumpStartSpeed( jumpSpeed );
	//	}

	//	ball.update();

	//	//clean image
	//	img.setTo(0);
	//}
	
	return 0;
}

//void updateMap( cv::Mat& img, const Ball& ball )
//{
//	cv::Point p1 = cv::Point( 0, img.rows / 2 );
//	cv::Point p2 = cv::Point( img.cols - 1, img.rows / 2 );
//	cv::line( img, p1, p2, RED, 2 );
//
//	ball.draw( img );
//}