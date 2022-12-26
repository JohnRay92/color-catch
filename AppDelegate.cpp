#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

#define SDK

#ifdef SDK
#include "PluginFacebook.h"
#include "PluginAdColony/PluginAdColony.h"
#include "PluginChartboost/PluginChartboost.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "PluginReview/PluginReview.h"
#include "PluginAdMob/PluginAdMob.h"
#include "PluginIAP/PluginIAP.h"
#endif

USING_NS_CC;
using namespace CocosDenshion;

bool scene_paused{ false }; //pause

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
#ifdef SDK
	sdkbox::PluginAdMob::init(); //initialise AdMob
	sdkbox::PluginFacebook::init();// initialise facebook
	sdkbox::PluginAdColony::init(); // initialise AdColony
	sdkbox::PluginSdkboxPlay::init(); //initialise playstore
	sdkbox::PluginChartboost::init(); //initialise chartboost
	sdkbox::IAP::init();
#endif
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("share", Rect(0, 0, 640, 960));
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(640, 960, ResolutionPolicy::FIXED_WIDTH);

	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	FileUtils::getInstance()->addSearchPath("res");

	// create a scene. it's an autorelease object
	auto scene = HelloWorld::createScene();

	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	//Director::getInstance()->stopAnimation();
	scene_paused = true;
	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//Director::getInstance()->startAnimation();
	// if you use SimpleAudioEngine, it must resume here
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
