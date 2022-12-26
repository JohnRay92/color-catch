#include "LeaderboardScene.h"
#include "ui/CocosGUI.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#else
#include <io.h>
#include <WS2tcpip.h>
#endif

//#define SDK

#ifdef SDK

USING_NS_CC;

using namespace sdkbox;
using namespace ui;

Scene* LeaderboardScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LeaderboardScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

static int score;

// on "init" you need to initialize your instance
bool LeaderboardScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Director::getInstance()->startAnimation();// start animation *solve stop animation problem
	Size size = CCDirector::getInstance()->getWinSize();
	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);

	std::string defaultFont("arial.ttf");
	int defaultFontSize = 32;

	char buffer[128];

	time_t tt;
	time(&tt);
	score = CCUserDefault::sharedUserDefault()->getIntegerForKey("key3");
	sprintf(buffer, "Send my best score (%d)", score);
	std::string str = buffer;
	
	Menu* menu = Menu::create(
		MenuItemFont::create("Connect", CC_CALLBACK_1(LeaderboardScene::connect, this)),
		MenuItemFont::create("Disconnect", CC_CALLBACK_1(LeaderboardScene::disconnect, this)),
		MenuItemFont::create("Show Leaderboard Hall Of Fame", CC_CALLBACK_1(LeaderboardScene::showLeaderboard, this)),
		MenuItemFont::create("Achievements", CC_CALLBACK_1(LeaderboardScene::showAchievements, this)),
		MenuItemFont::create(str, CC_CALLBACK_1(LeaderboardScene::send_score, this)),
		NULL
		);

	// add logo
	logo = Sprite::create("Color Wheel.png");
	logo->setScale(1.0f);
	logo->setAnchorPoint(Vec2(0.5, 0.5));
	logo->setPosition(cocos2d::Vec2(size.width / 2, (size.height * 0.55) + 100 * 1.2f));
	addChild(logo, 20);

	auto light = Sprite::create("ray2.png");
	light->setScale(1.2f);
	light->setPosition(cocos2d::Vec2(size.width / 2, (size.height * 0.55) + 100 * 1.2f));
	addChild(light, 2);

	auto exit = Button::create("x.png");
	exit->setPosition(cocos2d::Vec2(size.width / 2, (size.height * 0.55) + 100 * 1.2f));
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

	auto login = Button::create("playlogin.png");
	login->setAnchorPoint(Vec2(0.5, 0.5));
	login->setScale(0.5f);
	login->setPosition(Vec2(size.width - size.width * 0.8, size.height * 0.55 - 200));
	this->addChild(login, 1000);

	login->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::PluginSdkboxPlay::signin();
		}
	});

	auto logout = Button::create("playlogout.png");
	logout->setAnchorPoint(Vec2(0.5, 0.5));
	logout->setScale(0.5f);
	logout->setPosition(Vec2(size.width - size.width * 0.2, size.height * 0.55 - 200));
	this->addChild(logout, 1000);

	logout->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::PluginSdkboxPlay::signout();
		}
	});

	auto leader = Button::create("leaderboard.JPG");
	leader->setAnchorPoint(Vec2(0.5, 0.5));
	leader->setScale(0.5f);
	leader->setPosition(Vec2(size.width / 2, size.height * 0.55 - 200));
	this->addChild(leader, 1000);

	leader->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			sdkbox::PluginSdkboxPlay::signin();
			if (score >= 100)
			{
				sdkbox::PluginSdkboxPlay::submitScore("Hall Of Fame", score);
			}
			sdkbox::PluginSdkboxPlay::showLeaderboard("Hall Of Fame");
		}
	});

	menu->alignItemsVerticallyWithPadding(5);
	menu->setPosition(size.width / 2, (size.height * 0.55) - 200 * 1.2f);
	//addChild(menu);

	_txtStat = Label::create("No action yet.", "fonts/Marker Felt.ttf", 32);
	_txtStat->setAnchorPoint(cocos2d::Point(0, 0));
	_txtStat->setPosition(cocos2d::Point(10, 10));
	_txtStat->setVisible(false);
	addChild(_txtStat);

	_txtC = Label::create("Disconnected", "fonts/Marker Felt.ttf", 32);
	_txtC->setAnchorPoint(cocos2d::Point(0, 0));
	_txtC->setPosition(cocos2d::Point(10, 45));
	_txtC->setVisible(false);
	addChild(_txtC);

	sdkbox::PluginSdkboxPlay::setListener(this);
	sdkbox::PluginSdkboxPlay::init();
	schedule(schedule_selector(LeaderboardScene::scroll));

	return true;
}

void LeaderboardScene::connect(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::signin();
}

void LeaderboardScene::disconnect(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::signout();
}

void LeaderboardScene::showLeaderboard(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::showLeaderboard("Hall Of Fame");
}

void LeaderboardScene::showAllLeaderboards(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::showAllLeaderboards();
}

void LeaderboardScene::showAchievements(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::showAchievements();
}

void LeaderboardScene::achievement_craftsman(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::unlockAchievement("craftsman");
}

void LeaderboardScene::achievement_hunter(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::unlockAchievement("hunter");
}

void LeaderboardScene::achievement_ten_games(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::unlockAchievement("ten-games");
}

void LeaderboardScene::achievement_incremental(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::incrementAchievement("incremental", 1);
}

void LeaderboardScene::getMyScore(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::getMyScore("Hall Of Fame", ALL_TIME, GLOBAL);
}

void LeaderboardScene::loadAchievements(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::loadAchievements(true);
}

void LeaderboardScene::send_score(cocos2d::CCObject *sender) {
	sdkbox::PluginSdkboxPlay::submitScore("Hall Of Fame", score);
}

void LeaderboardScene::increment(cocos2d::CCObject* sender) {
	sdkbox::PluginSdkboxPlay::incrementAchievement("incremental", 10);
}

void LeaderboardScene::setSteps(cocos2d::CCObject* sender) {
	sdkbox::PluginSdkboxPlay::setSteps("incremental", 50);
}

void LeaderboardScene::revealHidden(cocos2d::CCObject* sender) {
	sdkbox::PluginSdkboxPlay::reveal("hidden");
}


void LeaderboardScene::onScoreSubmitted(const std::string& leaderboard_name, int score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday) {
	CCLOG("on score %d submitted to leaderboard:%s", score, leaderboard_name.c_str());
	CCLOG("all time hi %d", maxScoreAllTime ? 1 : 0);
	CCLOG("weekly hi %d", maxScoreWeek ? 1 : 0);
	CCLOG("daily hi %d", maxScoreToday ? 1 : 0);
}

void LeaderboardScene::onIncrementalAchievementUnlocked(const std::string& achievement_name) {
	CCLOG("incremental achievement %s unlocked.", achievement_name.c_str());
}

void LeaderboardScene::onIncrementalAchievementStep(const std::string& achievement_name, int step) {
	CCLOG("incremental achievent %s step: %d", achievement_name.c_str(), step);
}

void LeaderboardScene::onAchievementUnlocked(const std::string& achievement_name, bool newlyUnlocked) {
	CCLOG("achievement %s unlocked (new %d)", achievement_name.c_str(), newlyUnlocked ? 1 : 0);
}

void LeaderboardScene::onConnectionStatusChanged(int connection_status) {
	CCLOG("connection status change: %d", connection_status);

	char str[256];
	sprintf(str, "Connection status: %d. Is Connected: %d", connection_status, sdkbox::PluginSdkboxPlay::isConnected() ? 1 : 0);

	_txtStat->setString(str);

	if (connection_status == 1000) {
		std::string sstr = PluginSdkboxPlay::getPlayerId() +
			":'" +
			PluginSdkboxPlay::getPlayerAccountField("name") +
			"(" +
			PluginSdkboxPlay::getPlayerAccountField("display_name") +
			")'";

		_txtC->setString(sstr);
	}
}

void LeaderboardScene::onAchievementsLoaded(bool reload_forced, const std::string& json_achievements_info) {
	CCLOG("load achievements fr: %d, metadata: %s", reload_forced ? 1 : 0, json_achievements_info.c_str());
}

void LeaderboardScene::onSetSteps(const std::string& name, int steps) {
	CCLOG("achievement set steps: %s, steps: %d", name.c_str(), steps);
}

void LeaderboardScene::onSetStepsError(const std::string& name, int steps, int error_code, const std::string& error_description) {
	CCLOG("achievement set steps error: %s, steps: %d. %d:%s", name.c_str(), steps, error_code, error_description.c_str());
}

void LeaderboardScene::onReveal(const std::string& name) {
	CCLOG("achievement revealed: %s", name.c_str());
}

void LeaderboardScene::onRevealError(const std::string& name, int error_code, const std::string& error_description) {
	CCLOG("achievement reveal error: %s. %d:%s", name.c_str(), error_code, error_description.c_str());
}

void LeaderboardScene::onAchievementUnlockError(const std::string& achievement_name, int error_code, const std::string& error_description) {
	CCLOG("achievement %s unlock error. %d:%s", achievement_name.c_str(), error_code, error_description.c_str());
}

void LeaderboardScene::onMyScore(const std::string& leaderboard_name, int time_span, int collection_type, long score) {
	CCLOG("get my score in leaderboard %s time scope %d collection scope %d = %ld", leaderboard_name.c_str(), time_span, collection_type, score);
}

void LeaderboardScene::onMyScoreError(const std::string& leaderboard_name, int time_span, int collection_type, int error_code, const std::string& error_description) {

	CCLOG("get my score error %d:%s", error_code, error_description.c_str());
}

void LeaderboardScene::onPlayerCenteredScores(const std::string& leaderboard_name,
	int time_span,
	int collection_type,
	const std::string& json_with_score_entries)  {

}

void LeaderboardScene::onPlayerCenteredScoresError(const std::string& leaderboard_name,
	int time_span,
	int collection_type,
	int error_code,
	const std::string& error_description) {

}

void LeaderboardScene::onIncrementalAchievementStepError(const std::string& name, int steps, int error_code, const std::string& error_description) {
	CCLOG("incremental achievent step error: %s step: %d. %d:%s", name.c_str(), steps, error_code, error_description.c_str());
}

//scrolling
void LeaderboardScene::scroll(float dt) {

	auto health_angle = logo->getRotation();
	health_angle += 0.6f;
	auto background_angle = logo->getRotation();
	background_angle -= 0.3f;
	logo->setRotation(health_angle);
	logo->setRotation(background_angle);
}

#endif