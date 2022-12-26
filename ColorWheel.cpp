#include "ColorWheel.h"

using namespace cocos2d;

ColorWheel::ColorWheel() {}

ColorWheel::~ColorWheel() {}


//create sprite with file name
ColorWheel* ColorWheel::createWithFileName(char *fileName)
{
	auto pSprite = new ColorWheel;
	if (pSprite && pSprite->initWithFile(fileName)) {
		//pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

//init some properties
void ColorWheel::initOptions(cocos2d::Node* child)
{
	m_wheelColor = RED; //set color to blue
	this->setTag(m_wheelColor); //set sprite name
	this->setName("wheel"); //set sprite name
	//creat physics body
	auto wheelBody_ = PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 0, 0));
	wheelBody_->setDynamic(true);
	wheelBody_->setGravityEnable(false);
	wheelBody_->setCollisionBitmask(1);
	wheelBody_->setContactTestBitmask(true);
	this->setPhysicsBody(wheelBody_);
	// set scale for the wheel
	this->setScale(wheelscalelow);
}

void ColorWheel::displayAnimation()
{
	auto scale = this->getScale();
	auto scaleup = ScaleTo::create(0.1f, 0.575f); // make scale up and down animation for the wheel
	auto scaledown = ScaleTo::create(0.1f, scale);
	// create a sequence with the actions and callbacks
	auto seq = Sequence::create(scaleup, scaledown, nullptr);
	// run it
	this->runAction(seq);
}

void ColorWheel::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{

		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode))
			{
				if (m_touchEnable)
				{
					return true; // to indicate that we have consumed it.
				}
			}
		
		return false; // we did not consume this event, pass thru.
	};

	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
			this->setScale(wheelscalelow);
			Rect rectr = Rect(0, 0, 0, 0);
			//Get the position of the current point relative to the wheel
			if (m_isLeft)
			{
				rectr = Rect(0, 0, visibleSize.width, visibleSize.height);
			}
			else
			{
				rectr = Rect(visibleSize.width / 2, 0, visibleSize.width, visibleSize.height);
			}


			//Check the click area
			if (rectr.containsPoint(touch->getLocation()))
			{
				if (m_touchEnable)
				{

						m_targetRotation += 180;
						this->setRotation(m_targetRotation);
						if (m_wheelColor == BLUE)
						{
							m_wheelColor = RED;
							log("RED");
							log("color is %d", m_wheelColor);
						}
						else
						{
							log("BLUE");
							m_wheelColor = BLUE;
							log("color is %d", m_wheelColor);
						}
						this->setTag(m_wheelColor); //set sprite name
						log("color is %d", m_wheelColor);
				}
			}
		
	};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}




