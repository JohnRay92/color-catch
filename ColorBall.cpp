#include "ColorBall.h"
USING_NS_CC;


ColorBall* ColorBall::createWithFileName(char *fileName) 
{
	auto sprite = new ColorBall;
	if (sprite && sprite->initWithFile(fileName)) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void ColorBall::Start() 
{
	
	this->stopAllActions(); // stop all moves or actions
	this->setOpacity(255); // set to full opacity indicating that the ball is currently active
	State = ACTIVEBALL; // make the ball active
	float distance = m_yOffSet + m_screenHeight ;
	float time = distance / m_speed;
	Vec2 destination = Vec2(this->getPositionX(), -m_yOffSet);// destination for the current ball
	this->setTag(m_colorEnum); //set the sprite tag to it's current color
	this->setPosition(Vec2(this->getPositionX(), m_yOffSet + m_screenHeight));
	MoveTo *actionMove = MoveTo::create(time, destination);//creat move action
	CallFunc *actionMoveDone = CallFunc::create(CC_CALLBACK_0(ColorBall::ReachedDestination, this)); //function call back for reaching destination
	this->runAction(Sequence::create(actionMove, actionMoveDone, NULL));// run actions created for the ball
}

void ColorBall::Stop() 
{
	this->setOpacity(0); //to be checked ************
	State = INACTIVEBALL; //current state is inactive
	this->setPosition(Vec2(this->getPositionX(), m_yOffSet + m_screenHeight)); //set position for inactive ball
	m_colorEnum = RED; //set Color Enum to red
}

void ColorBall::IncreaseSpeed(int speed)
{
	float distance = this->getPositionY() - 0.5*m_yOffSet;//******
	if (distance > 0)
	{
		this->stopAllActions();
		float time = distance / m_speed;
		Vec2 destination = Vec2(this->getPositionX(), -m_yOffSet);// destination for the current ball
		MoveTo *actionMove = MoveTo::create(time, destination);
		CallFunc *actionMoveDone = CallFunc::create(CC_CALLBACK_0(ColorBall::ReachedDestination, this));
		this->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
	}
}

void ColorBall::Initialise(const float speed, const float height) 
{
	auto ballBody = PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 0, 0)); //physics of the current ball
	ballBody->setDynamic(false); // not affected by gravity
	ballBody->setCollisionBitmask(2); 
	ballBody->setContactTestBitmask(0X00000001);
	this->setPhysicsBody(ballBody); //attach physics body to current ball
	m_speed = speed; //set ball's speed
	m_screenHeight = height; //set screen height
	m_yOffSet = this->getBoundingBox().size.height; //set y offset of the ball to ball's height
	State = INACTIVEBALL; //current state is inactive to be revised **************
	this->setName("ball");
	this->setOpacity(0);
}

void ColorBall::ReachedDestination() 
{
	Stop(); //stop the ball
}

void ColorBall::SetColorEnum(int color)
{
	m_colorEnum = color; // set Color Enum
}