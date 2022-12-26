#include "MenuScene.h"
#include "SettingsScene.h"
#include "ShareScene.h"
#include "GameScene.h"
#include "PurchasesScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include"SimpleAudioEngine.h"
#include "LeaderboardScene.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
Size visibleSize;
int games_played = 0;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);
#ifdef SDK
	sdkbox::PluginReview::init(); //initialise review
#endif
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);
	//rating event
	//sdkbox::PluginReview::setListener(this);

	/*
	preload audio
	*/

	SimpleAudioEngine::getInstance()->preloadEffect("Hyper GoldenDays Clave.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Hyper DoubleBarrel Danger FX.wav");

	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("15 Fever.m4a");

	//simpleAudioEngine::getInstance()->playBackgroundMusic("15 Fever.m4a", true);
#ifdef SDK
	//sdkbox::PluginChartboost::init(); //initialise chartboost
	//sdkbox::PluginSdkboxPlay::init();
#endif
	//director screen size
	//director screen size
	visibleSize = Director::getInstance()->getVisibleSize();

	auto button1 = Button::create("purchases.png", "purchases2.png", "purchases.png");
	button1->setScale(1.4f);
	button1->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height * 0.55) + 134 * 1.4f));

	auto button2 = Button::create("play.png", "play2.png", "play.png");
	button2->setScale(1.4f);
	button2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55));

	auto button3 = Button::create("quit.png", "quit2.png", "quit.png");
	button3->setScale(1.4f);
	button3->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height * 0.55) - 132 * 1.4f));

	auto button4 = Button::create("share.png", "share2.png", "share.png");
	button4->setScale(1.4f);
	button4->setPosition(Vec2(visibleSize.width / (1.40f* 3.475), visibleSize.height * 0.55));

	auto button5 = Button::create("settings.png", "settings2.png", "settings.png");
	button5->setScale(1.4f);
	button5->setPosition(Vec2(visibleSize.width / (1.345 / 1.06f), visibleSize.height * 0.55));

	auto button6 = Button::create("W_AD.png");
	button6->setScale(0.6f);
	button6->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55 - 400));

	auto button7 = Button::create("W_LEADER.png");
	button7->setScale(0.6f);
	button7->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.2, visibleSize.height * 0.55 - 400));

	auto button8 = Button::create("W_RATE.png");
	button8->setScale(0.6f);
	button8->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.8, visibleSize.height * 0.55 - 400));

	auto label = Sprite::create("title.png");
	label->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point
	label->setScale(1.2f);
	label->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height * 0.55) + 275));


	//Purchases button
	button1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			auto purchases_scene = PurchasesScene::createScene();
			Director::getInstance()->pushScene(purchases_scene);

		}

	});


	//PLAY button
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			auto game_scene = GameScene::createScene();
			Director::getInstance()->pushScene(TransitionFade::create(0.01f, game_scene, Color3B(0.0f, 0.0f, 0.0f)));
			Director::getInstance()->resume();
		}
	});

	//QUIT button
	button3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->end();
		}
	});

	//SHARE button
	button4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			//auto share_scene = ShareScene::createScene();
			//Director::getInstance()->pushScene(share_scene);
			high_score = CCUserDefault::sharedUserDefault()->getIntegerForKey("key3");
#ifdef SDK
			sdkbox::PluginFacebook::login();
			sdkbox::PluginFacebook::requestReadPermissions({ FB_PERM_READ_PUBLIC_PROFILE, FB_PERM_READ_USER_FRIENDS });
			sdkbox::PluginFacebook::requestPublishPermissions({ FB_PERM_PUBLISH_POST });

			sdkbox::FBShareInfo info;
			info.type = sdkbox::FB_LINK;
			info.link = "http://www.treoplay.com";
			info.title = "I bet you can't beat my score";
			sprintf(txt, "MY BEST SCORE IS %d", high_score);
			info.text = txt;
			info.image = "http://treoplay.com/wp-content/uploads/2016/07/logo-1fb.png";
			sdkbox::PluginFacebook::dialog(info);


#endif
		}
	});

	//settings button
	button5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
#ifdef SDK

#endif
			auto settings_scene = SettingsScene::createScene();
			Director::getInstance()->pushScene(settings_scene);
		}


	});

	//AD button
	button6->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
#ifdef SDK
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			auto share_scene = ShareScene::createScene();
			Director::getInstance()->pushScene(share_scene);
		}
#endif

	});

	button7->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
#ifdef SDK
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->resume();
			auto leaderboard_scene = LeaderboardScene::createScene();
			Director::getInstance()->pushScene(leaderboard_scene);
		}
#endif

	});

	button8->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
#ifdef SDK
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			if (sdkbox::PluginSdkboxPlay::isSignedIn())
			{
				sdkbox::PluginReview::show(true /* force */);
				CCLOG("sdkbox::PluginReview::show()");
			}
			else
			{

			}

		}
#endif

	});

	_ball_1 = cocos2d::Sprite::create("blue.png");
	_ball_1->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.2, visibleSize.height * 0.55 - 300));

	_ball_1->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point

	//this->addChild(_ball_1, 10);


	_ball_3 = cocos2d::Sprite::create("green.png");
	_ball_3->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.8, visibleSize.height * 0.55 - 300));

	_ball_3->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point

	//this->addChild(_ball_3, 30);

	this->addChild(button1);
	this->addChild(button2);
	this->addChild(button3);
	this->addChild(button4);
	this->addChild(button5);
	this->addChild(button6);
	this->addChild(button7);
	this->addChild(button8);
	this->addChild(label);



	return true;
}


#ifdef SDK
void MenuScene::onShowReview(cocos2d::Ref* sender)
{
	sdkbox::PluginReview::show(true /* force */);
	CCLOG("sdkbox::PluginReview::show()");
}

void MenuScene::onDisplayAlert()
{
	CCLOG("onDisplayAlert");
}

void MenuScene::onDeclineToRate()
{
	CCLOG("onDeclineToRate");
}

void MenuScene::onRate()
{
	CCLOG("onRate");
}

void MenuScene::onRemindLater()
{
	CCLOG("onRemindLater");
}
#endif