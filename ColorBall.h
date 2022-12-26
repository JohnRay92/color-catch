#ifndef _COLORBALL_H_
#define _COLORBALL_H_

#include "cocos2d.h"
#include "GameProperties.h"


class ColorBall : public cocos2d::Sprite {

public:
	bool State { INACTIVEBALL };
	//bool m_Scored; //will be removed most likely
	static ColorBall* createWithFileName(char *fileName); // methode for creating ball sprite
	void Start(); //methode for starting a ball move and 
	void Stop();
	void Initialise(const float speed, const float height); //init properties
	ColorBall() {}; //constructor 
	int GetColorEnum(){ return m_colorEnum; }//get ball's color
	void SetColorEnum(int color);// set ball's color
	bool m_left { true }; // bool indicates whether the ball is spawned from left or right
	void setBallSpeed(int speed){ m_speed = speed; }; // set ball's speed ,increase speed when needed
	void IncreaseSpeed(int speed);
private:
	void ReachedDestination();
	int m_speed { 100 };
	float m_screenHeight;
	int m_colorEnum{ RED };
	float m_yOffSet;
};

#endif 