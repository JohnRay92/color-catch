#include "ShareScene.h"
#include "ui/UIButton.h"

USING_NS_CC;
using namespace ui;
int g_reward = 0;

Scene* ShareScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ShareScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ShareScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// add logo
	auto winsize = Director::getInstance()->getWinSize();
	auto logo = Sprite::create("big-logo.png");
	logo->setPosition(Vec2(winsize.width/2, winsize.height / 1.75));
	addChild(logo,20);
	auto ray = Sprite::create("ray.png");
	ray->setPosition(Vec2(winsize.width / 2, winsize.height / 1.75));
	addChild(ray,0);
	auto txt = Sprite::create("WatchAdtxt2.png");
	txt->setPosition(Vec2(winsize.width / 2, (winsize.height / 1.5) + 180));
	txt->setScale(0.6f);
	addChild(txt, 0);
	

	//rewarded label
	_rewarded = Label::createWithSystemFont("REWARDED", "Exo2-Bold.otf", 45);
	_rewarded->setPosition(Vec2(winsize.width / 2, (winsize.height / 1.5) - 560));
	_rewarded->setColor(Color3B::GREEN);	
	addChild(_rewarded);
	if (g_reward == 0)
	{
		_rewarded->setVisible(false);
	}
	else
	{
		_rewarded->setVisible(true);
	}


	// add test menu
#ifdef SDK
	createTestMenu();
	sdkbox::PluginAdColony::setListener(this);
#endif
	return true;
}

#ifdef SDK
void ShareScene::createTestMenu()
{
	Size size = Director::getInstance()->getWinSize();

	// add status label
	
	_status = Label::createWithSystemFont("No available AD.", "sans", 32);
	_status->setTextColor(Color4B(0, 255, 0, 255));
	_status->setAnchorPoint(Vec2(0, 0));
	_status->setPosition(5, 5);
	addChild(_status);
	_status->setVisible(false);

	
	//MenuItemFont::setFontName("Exo2-Bold.otf");
	//MenuItemFont::setFontSize(46);
	
	//auto menu = Menu::create(
		//MenuItemFont::create("Watch Video AD", CC_CALLBACK_1(ShareScene::onShowV4vc, this)),
		//MenuItemFont::create("Back", CC_CALLBACK_1(ShareScene::back, this)),
		//NULL);
	auto winsize = Director::getInstance()->getWinSize();

	//menu->alignItemsVerticallyWithPadding(8);
	//menu->setPosition(Vec2(winsize.width / 2, (winsize.height / 1.5) - 400));
	auto button1 = Button::create("Watch ad.png");
	button1->setScale(0.55f);
	button1->setPosition(cocos2d::Vec2(winsize.width - (winsize.width*0.65), (winsize.height / 2) - 260));
	addChild(button1);

	button1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::PluginAdColony::show("v4vc");
		}

	});

	auto button2 = Button::create("x.png");
	button2->setScale(0.55f);
	button2->setPosition(cocos2d::Vec2(winsize.width - (winsize.width*0.35), (winsize.height / 2) - 260));
	addChild(button2);

	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		//when touch ends replace scene
		if (type == Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->popScene();
		}

	});
}

void ShareScene::onShowVideo(cocos2d::Ref* sender)
{
	sdkbox::PluginAdColony::show("video");
	CCLOG("onShowVideo");
}

void ShareScene::back(cocos2d::Ref* sender)
{
	Director::getInstance()->popScene();
}

void ShareScene::onShowV4vc(cocos2d::Ref* sender)
{
	sdkbox::PluginAdColony::show("v4vc");
	CCLOG("onShowVideo");
}

void ShareScene::onAdColonyChange(const sdkbox::AdColonyAdInfo& info, bool available)
{
	    //struct AdColonyAdInfo
	    //{
	        //std::string name;
	        //bool shown;
	        //std::string zoneID;
	        //bool iapEnabled;
	        //std::string iapProductID;
	        //int iapQuantity;
	        //int iapEngagementType;
	    //};
	
	_status->setString(__FUNCTION__);
	
	CCLOG("onAdColonyChange");
	CCLOG("info.name: %s", info.name.c_str());
	CCLOG("info.shown: %s", info.shown ? "true" : "false");
	CCLOG("info.zoneID: %s", info.zoneID.c_str());
	CCLOG("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
	CCLOG("info.iapProductID: %s", info.iapProductID.c_str());
	CCLOG("info.iapQuantity: %d", info.iapQuantity);
	CCLOG("info.iapEngagementType: %d", info.iapEngagementType);
}

void ShareScene::onAdColonyReward(const sdkbox::AdColonyAdInfo& info,
	const std::string& currencyName, int amount, bool success)
{
	
	_status->setString(__FUNCTION__);
	//_reward->setString(StringUtils::toString((int)utils::atof(_reward->getString().c_str()) + amount));
	
	g_reward = 2;
	_rewarded->setVisible(true);
	CCLOG("onAdColonyReward, currencyName: %s, amount: %d, success: %s", currencyName.c_str(), amount, success ? "true" : "false");
	CCLOG("info.name: %s", info.name.c_str());
	CCLOG("info.shown: %s", info.shown ? "true" : "false");
	CCLOG("info.zoneID: %s", info.zoneID.c_str());
	CCLOG("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
	CCLOG("info.iapProductID: %s", info.iapProductID.c_str());
	CCLOG("info.iapQuantity: %d", info.iapQuantity);
	CCLOG("info.iapEngagementType: %d", info.iapEngagementType);
	
}

void ShareScene::onAdColonyStarted(const sdkbox::AdColonyAdInfo& info)
{
	
	_status->setString(__FUNCTION__);
	
	CCLOG("onAdColonyStarted");
	CCLOG("info.name: %s", info.name.c_str());
	CCLOG("info.shown: %s", info.shown ? "true" : "false");
	CCLOG("info.zoneID: %s", info.zoneID.c_str());
	CCLOG("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
	CCLOG("info.iapProductID: %s", info.iapProductID.c_str());
	CCLOG("info.iapQuantity: %d", info.iapQuantity);
	CCLOG("info.iapEngagementType: %d", info.iapEngagementType);
}

void ShareScene::onAdColonyFinished(const sdkbox::AdColonyAdInfo& info)
{
	
	_status->setString(__FUNCTION__);
	
	CCLOG("onAdColonyFinished");
	CCLOG("info.name: %s", info.name.c_str());
	CCLOG("info.shown: %s", info.shown ? "true" : "false");
	CCLOG("info.zoneID: %s", info.zoneID.c_str());
	CCLOG("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
	CCLOG("info.iapProductID: %s", info.iapProductID.c_str());
	CCLOG("info.iapQuantity: %d", info.iapQuantity);
	CCLOG("info.iapEngagementType: %d", info.iapEngagementType);
}

#endif