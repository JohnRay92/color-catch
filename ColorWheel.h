#ifndef _COLORWHEEL_H_
#define _COLORWHEEL_H_

#include "cocos2d.h"
#include "GameProperties.h"

USING_NS_CC;
extern Size visibleSize;
class ColorWheel : public cocos2d::Sprite
{
public:
	ColorWheel();
	~ColorWheel();
	static ColorWheel* createWithFileName(char *fileName); // methode for creating wheel sprite
	void initOptions(cocos2d::Node* child);
	void addEvents();
	void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);
	void setLeft(){ m_isLeft = true; };
	void enableTouch(){ m_touchEnable = true; };
	void disableTouch(){ m_touchEnable = false; };
	void displayAnimation();
private:
	bool m_tap{ true };
	bool m_touchEnable{ true };
	double m_targetRotation{ 0.0 };
	double m_newRotation{ 0.0 };
	double m_prevRotation{ 0.0 };
	unsigned int m_wheelColor{ 0 };
	bool m_isLeft{ false };
	//cocos2d::Sprite* light_colorWheel_;

};

#endif