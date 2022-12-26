#ifndef __GameProperties_H__
#define __GameProperties_H__
#include "GameController.h"

#define ACTIVEBALL		true
#define INACTIVEBALL	false
#define INACTIVEBALL_IN_Y  -1000 
#define COLOR 0

#define BallSpawnMinTime 4.55
#define BallSpawnTimeVariance 12

#define lightIndex 1
#define lightON 90
#define lightOFF 0
#define wheelscalelow 0.58f
#define wheelscalemin 0.55f
#define wheelscalehigh 0.63f
#define wheelscalemax 0.665f
#define ColorWheelIndex 3
#define ColorBallIndex  200
#define FontName "Exo2-Bold.otf"
#define FontSize = 42.0

enum COLORWHEEL
{
	RED, 
	GREEN,
	BLUE,
	RED_BOMB,
	RED_HEART,
	GREEN_HEART,
	BLUE_HEART,
	TOTAL_NUM_COLORS

};


#endif