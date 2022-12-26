#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//init first
	if (!Layer::init())
	{
		return false;
	}
	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);
	//get director visible size and origin
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add "TREOPLAY" splash screen"
	//auto logo = Sprite::create("TREO_GREEN.png");
	auto logo = Sprite::create("TREO.png");
	// position the sprite on the center of the screen
	logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 20.0f));
	logo->setScale(1.2f);
	// add the sprite as a child to this layer
	this->addChild(logo, 5);

	return true;
}

void HelloWorld::onEnter() {
	Layer::onEnter();

	// Wait for 0.5 seconds to load main scene
	this->scheduleOnce(schedule_selector(HelloWorld::finishSplash), 2.0f);
}
void HelloWorld::finishSplash(float dt) {

	//creat menu scene
	auto menu_scene = MenuScene::createScene();

	//replace current scene with fade effect
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, menu_scene, Color3B(0, 0, 0)));

}
