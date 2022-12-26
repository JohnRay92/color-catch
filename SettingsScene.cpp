#include "SettingsScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"SimpleAudioEngine.h"


USING_NS_CC;

using namespace ui;
extern Size visibleSize;

const char *_EFFECT_VOLUME_ = "key6";//effect volume settings
const char *_BACKGROUND_VOLUME_ = "key9";//background volume settings

extern int effect_vol;//effect volume
extern int background_vol;//back ground volume

//extern CCUserDefault *userdef;
Scene* SettingsScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SettingsScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SettingsScene::init()
{
	//////////////////////////////

	//labels
	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);

	effect_vol = CCUserDefault::sharedUserDefault()->getIntegerForKey(_EFFECT_VOLUME_);
	background_vol = CCUserDefault::sharedUserDefault()->getIntegerForKey(_BACKGROUND_VOLUME_);
	sprintf(BG_str, "%d", background_vol);
	sprintf(eff_str, "%d", effect_vol);

	m_effLabel = AddLabel(42.0f, eff_str, cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2((visibleSize.width / 2) + 60, (visibleSize.height * 0.55) - 134 * 1.2f), Color3B::WHITE);
	m_BGLabel = AddLabel(42.0f, BG_str, cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2((visibleSize.width / 2)  + 60, (visibleSize.height * 0.55) - 265 * 1.2f), Color3B::WHITE);
	music = AddLabel(42.0f, "Music", cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2((visibleSize.width / 2)-260 + 60, (visibleSize.height * 0.55) - 265 * 1.2f), Color3B::WHITE);
	sounds = AddLabel(42.0f, "Sounds", cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2((visibleSize.width / 2) - 260 + 60, (visibleSize.height * 0.55) - 134 * 1.2f), Color3B::WHITE);

	auto plus_button_effect = Button::create("plus.png");
	plus_button_effect->setScale(0.5f);
	plus_button_effect->setPosition(Vec2((visibleSize.width / 2) + 130 + 60 , (visibleSize.height * 0.55) - 134 * 1.2f));

	auto minus_button_effect = Button::create("minus.png");
	minus_button_effect->setScale(0.5f);
	minus_button_effect->setPosition(Vec2((visibleSize.width / 2) - 130 + 60, (visibleSize.height * 0.55) - 134 * 1.2f));

	auto plus_button_background = Button::create("plus.png");
	plus_button_background->setScale(0.5f);
	plus_button_background->setPosition(Vec2((visibleSize.width / 2) + 130 + 60, (visibleSize.height * 0.55) - 265 * 1.2f));

	auto minus_button_background = Button::create("minus.png");
	minus_button_background->setScale(0.5f);
	minus_button_background->setPosition(Vec2((visibleSize.width / 2) - 130 + 60, (visibleSize.height * 0.55) - 265 * 1.2f));

	// add logo
	auto logo = Sprite::create("Color Wheel.png");
	logo->setScale(0.8f);
	logo->setPosition(cocos2d::Vec2((visibleSize.width / 2), (visibleSize.height * 0.55) + 100 * 1.2f));
	addChild(logo, 20);

	exit = Button::create("x.png");
	exit->setScale(0.55f);
	exit->setAnchorPoint(Vec2(0.5, 0.5));
	exit->setPosition(cocos2d::Vec2((visibleSize.width / 2), (visibleSize.height * 0.55) + 100 * 1.2f));
	this->addChild(exit, 1000);



	//increase effect volume
	plus_button_effect->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			if (effect_vol < 5)
			{
				effect_vol++;
				sprintf(eff_str, "%d", effect_vol);
				m_effLabel->setString(eff_str);
				CCUserDefault::sharedUserDefault()->setIntegerForKey(_EFFECT_VOLUME_, effect_vol);
				CCUserDefault::sharedUserDefault()->flush();
			}
		}
	});

	//decrease effect volume
	minus_button_effect->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			if (effect_vol > 0)
			{
				effect_vol--;
				sprintf(eff_str, "%d", effect_vol);
				m_effLabel->setString(eff_str);
				CCUserDefault::sharedUserDefault()->setIntegerForKey(_EFFECT_VOLUME_, effect_vol);
				CCUserDefault::sharedUserDefault()->flush();
			}
		}
	});

	//increase background volume
	plus_button_background->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			if (background_vol < 5)
			{
				background_vol++;
				sprintf(BG_str, "%d", background_vol);
				m_BGLabel->setString(BG_str);
				CCUserDefault::sharedUserDefault()->setIntegerForKey(_BACKGROUND_VOLUME_, background_vol);
				CCUserDefault::sharedUserDefault()->flush();
			}
		}
	});

	//decrease background volume
	minus_button_background->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			if (background_vol > 0)
			{
				background_vol--;
				sprintf(BG_str, "%d", background_vol);
				m_BGLabel->setString(BG_str);
				CCUserDefault::sharedUserDefault()->setIntegerForKey(_BACKGROUND_VOLUME_, background_vol);
				CCUserDefault::sharedUserDefault()->flush();
			}
		}
	});

	exit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			//auto newScene = MenuScene::createScene();
			//this->removeAllChildrenWithCleanup(true);
			Director::getInstance()->popScene();
		}
	});

	this->addChild(plus_button_effect);
	this->addChild(minus_button_effect);
	this->addChild(plus_button_background);
	this->addChild(minus_button_background);
	this->addChild(exit);

	return true;
}

//add label method
Label * SettingsScene::AddLabel(const float fontSize, const char *text, const cocos2d::Vec2 anchor, const cocos2d::Vec2 position, Color3B color, int index)
{
	Label *newLabel = Label::createWithTTF(text, FontName, fontSize);
	newLabel->setAnchorPoint(anchor);
	newLabel->setPosition(position);
	newLabel->setColor(color);
	newLabel->getShadowOffset();
	this->addChild(newLabel, index); //INDEX modify
	return newLabel;
}