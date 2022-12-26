#include "PurchasesScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace sdkbox;
using namespace cocostudio::timeline;

Scene* PurchasesScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PurchasesScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PurchasesScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	sdkbox::IAP::setDebug(true);
	sdkbox::IAP::setListener(this);
	sdkbox::IAP::init();
	sdkbox::IAP::refresh();
	sdkbox::IAP::restore();

	Size size = CCDirector::getInstance()->getWinSize();

	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);

	auto exit = Button::create("x.png");
	exit->setScale(0.5f);
	exit->setPosition(Vec2(size.width / 2, size.height * 0.5 - 300));
	this->addChild(exit, 1000);

	exit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			//auto newScene = MenuScene::createScene();
			//this->removeAllChildrenWithCleanup(true);
			Director::getInstance()->pause();
			Director::getInstance()->popScene();
		}
	});

	auto adkiller = Button::create("buyad.png");
	adkiller->setAnchorPoint(Vec2(0.5, 0.5));
	adkiller->setScale(0.5f);
	adkiller->setPosition(Vec2(size.width / 2, size.height * 0.5 + 300));
	this->addChild(adkiller, 1000);

	adkiller->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::IAP::purchase("remove ads");
		}
	});

	auto buy4 = Button::create("buy4.png");
	buy4->setAnchorPoint(Vec2(0.5, 0.5));
	buy4->setScale(0.5f);
	buy4->setPosition(Vec2(size.width / 2, size.height * 0.5 +100));
	this->addChild(buy4, 1000);

	buy4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::IAP::purchase("start game with 4 health wheels");
		}
	});

	auto buy6 = Button::create("buy6.png");
	buy6->setAnchorPoint(Vec2(0.5, 0.5));
	buy6->setScale(0.5f);
	buy6->setPosition(Vec2(size.width / 2, size.height * 0.5 - 100));
	this->addChild(buy6, 1000);

	buy6->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::IAP::purchase("start game with 6 health wheels");
		}
	});


	return true;
}


void PurchasesScene::onRequestIAP(cocos2d::Ref* sender)
{
	IAP::refresh();
}

void PurchasesScene::onRestoreIAP(cocos2d::Ref* sender)
{
	IAP::restore();
}

void PurchasesScene::onIAP(cocos2d::Ref *sender)
{
	int i = reinterpret_cast<int>(dynamic_cast<MenuItemFont*>(sender)->getUserData());
	auto const &product = _products[i];
	CCLOG("Start IAP %s", product.name.c_str());
	IAP::purchase(product.name);
}

void PurchasesScene::onInitialized(bool ok)
{
	CCLOG("%s : %d", __func__, ok);
}

void PurchasesScene::onSuccess(const Product &p)
{
	if (p.name == "special_coins_4") {
		CCLOG("Purchase complete: coin_package");
		_coinCount = 4;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("key12", _coinCount);
		//_txtCoin->setString(tostr(_coinCount));
	}
	else if (p.name == "special_coins_6") {
		CCLOG("Purchase complete: coin_package2");
		_coinCount = 6;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("key12", _coinCount);
		//_txtCoin->setString(tostr(_coinCount));
	}
	else if (p.name == "remove_ads") {
		CCLOG("Purchase complete: Remove Ads");
		CCUserDefault::sharedUserDefault()->setBoolForKey("key15",true);
	}

	CCLOG("Purchase Success: %s", p.id.c_str());
}

void PurchasesScene::onFailure(const Product &p, const std::string &msg)
{
	CCLOG("Purchase Failed: %s", msg.c_str());
}

void PurchasesScene::onCanceled(const Product &p)
{
	CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void PurchasesScene::onRestored(const Product& p)
{
	CCLOG("Purchase Restored: %s", p.name.c_str());
}

void PurchasesScene::updateIAP(const std::vector<sdkbox::Product>& products)
{
	//_iapMenu->removeAllChildren();
	_products = products;


	for (int i = 0; i < _products.size(); i++)
	{
		CCLOG("IAP: ========= IAP Item =========");
		CCLOG("IAP: Name: %s", _products[i].name.c_str());
		CCLOG("IAP: ID: %s", _products[i].id.c_str());
		CCLOG("IAP: Title: %s", _products[i].title.c_str());
		CCLOG("IAP: Desc: %s", _products[i].description.c_str());
		CCLOG("IAP: Price: %s", _products[i].price.c_str());
		CCLOG("IAP: Price Value: %f", _products[i].priceValue);

		auto item = MenuItemFont::create(_products[i].name, CC_CALLBACK_1(PurchasesScene::onIAP, this));
		item->setUserData(reinterpret_cast<void*>(i));
		//_iapMenu->addChild(item);
	}

	Size size = Director::getInstance()->getWinSize();
	//_iapMenu->alignItemsVerticallyWithPadding(5);
	//_iapMenu->setPosition(Vec2(size.width / 2, size.height / 2));
}

void PurchasesScene::onProductRequestSuccess(const std::vector<Product> &products)
{
	updateIAP(products);
}

void PurchasesScene::onProductRequestFailure(const std::string &msg)
{
	CCLOG("Fail to load products");
}

void PurchasesScene::onRestoreComplete(bool ok, const std::string &msg)
{
	CCLOG("%s:%d:%s", __func__, ok, msg.data());
}
