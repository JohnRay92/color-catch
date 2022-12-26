#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define SDK
#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ColorBall.h"
#include "ColorWheel.h"

#ifdef SDK
#include "PluginChartboost/PluginChartboost.h"
#include "PluginAdMob/PluginAdMob.h"
#endif

static std::string kHomeBanner = "home";
static std::string kGameOverAd = "gameover";

static std::function<void(const std::string &)> showText = nullptr;

#ifdef SDK

class IMListener : public sdkbox::AdMobListener {
public:
	virtual void adViewDidReceiveAd(const std::string &name) {
		log("admob adViewDidReceiveAd %s: ", name.c_str());
		sdkbox::PluginAdMob::show(kHomeBanner);
		//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
	}
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
		log("admob adViewDidFailToReceiveAdWithError %s: ", msg.c_str());
		//if (showText) showText(StringUtils::format("%s name=%s, msg=%s", __FUNCTION__, name.c_str(), msg.c_str()));
	}
	virtual void adViewWillPresentScreen(const std::string &name) {
		log("admob adViewWillPresentScreen %s: ", name.c_str());
		//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
	}
	virtual void adViewDidDismissScreen(const std::string &name) {
		log("admob adViewDidDismissScreen %s: ", name.c_str());
		//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
	}
	virtual void adViewWillDismissScreen(const std::string &name) {
		//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));

		//if (name == "gameover") {
		//sdkbox::PluginAdMob::cache(kHomeBanner);
		//sdkbox::PluginAdMob::cache(kGameOverAd);
		//}
		log("admob adViewWillDismissScreen %s: ", name.c_str());
	}
	virtual void adViewWillLeaveApplication(const std::string &name) {
		//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
		log("admob adViewWillLeaveApplication %s: ", name.c_str());
	}
};
#endif

#ifdef SDK
class GameScene : public cocos2d::Layer , public sdkbox::ChartboostListener
{
#else
class GameScene : public cocos2d::Layer
	{
#endif
private:
	//user default data
	//CCUserDefault *userdef = CCUserDefault::sharedUserDefault();

	//Game labels
	cocos2d::Label * m_scoreLabel;
	cocos2d::Label * m_bestLabel;
	cocos2d::Label * m_highScoreLabel_str;
	cocos2d::Label * m_exitLabel;
	cocos2d::Label * m_healthLabel;
	cocos2d::Label * m_gameOverLabel;


	//Game buttons
	cocos2d::ui::Button* exit;
	cocos2d::ui::Button* play;
	cocos2d::ui::Button* pause;
	cocos2d::ui::Button* PlayAgain;

	//pause screen
	cocos2d::Sprite * pause_scene;
	

	//vector of balls
	cocos2d::Vector<ColorBall*> m_balls;

	//color wheels
	ColorWheel* _colorWheel_left;
	ColorWheel* _colorWheel_right;

	//sprite health
	cocos2d::Sprite* spritehealth;
	cocos2d::Sprite* gameover;

	//physics
	cocos2d::PhysicsWorld *gameWorld;

	//game speed properties 
	double m_lastRightSpawnTime{ 1.0 };
	double m_nextRightSpawnTime{ 1.0 };
	double m_lastleftSpawnTime{ 1.0 };
	double m_nextleftSpawnTime{ 1.0 };
	double n{ 2.4 }; //time multiplier ( with speed ) must be increamented by 0.2
	unsigned int game_speed{ 240 };
	double m_gameTime{ 0.0 };

	//sound
	bool m_soundflag_Taken{ false };
	bool m_soundflag_Missed{ false };

	//background
	cocos2d::Sprite* background1;
	cocos2d::Sprite* background2;

	//color ball properties
	int m_color = RED;
	
	//game controls
	bool m_gameOver{ false };
	bool m_acceptTouches{ false };

	//game methods
	void gameUpdate(float dt);
	void SpawnBallPair();
	void SpawnLeftOrRight(bool isLeft);
	ColorBall *getNextBall();
	void SpawnABall(bool isLeft, int color);
	void SpawnNewBalls();
	void SetRightSpawnTime();
	void SetleftSpawnTime();
	void gameCheck(float dt);
	cocos2d::Label * AddLabel(const float fontSize, const char *text, const cocos2d::Vec2 anchor, const cocos2d::Vec2 position, Color3B color, int index=100);

public:
	//public member variables
	char score_str[20] ;// score string 
	char health_str[20];// health string 
	char best_str[20];// best score string 
	int m_score{ 0 };// score of the current game
	int m_health{ 2 };// health of the current game
	int m_best{ 0 };; // *************** to be modified ************
	int score_index1{ 0 }; // score index for speed update
	int score_index2{ 0 };
	int m_leftColor{ 0 };//RED to be edited
	int m_rightColor{ 0 };//RED to be edited
	float wheel_width{ 10.0f }; //width of color wheel
	float WIDTH{ 0.0f };//************** to be revised *****************
	int max_tokens{ 2 };// for health tokens will increase by watching ad

	void scroll(float dt);
	
	void animate_scaleup(Node* node);
	void animate_scaledown(Node* node);

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	//physics methods 
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world){ gameWorld = world; };
	bool onContactBegin(cocos2d::PhysicsContact &contact);

	//sound methods
	void PlaySoundEffect_Taken();
	void PlaySoundEffect_Missed();
	void ClearSoundFlag_Taken(){ m_soundflag_Taken = false; };
	void SetSoundFlag_Taken(){ m_soundflag_Taken = true; };

	void ClearSoundFlag_Missed(){ m_soundflag_Missed = false; };
	void SetSoundFlag_Missed(){ m_soundflag_Missed = true; };

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	//chartboost
#ifdef SDK


	void onChartboostCached(const std::string& name)
	{
		CCLOG("onChartboostCached: %s", name.c_str());
	}

	bool onChartboostShouldDisplay(const std::string& name)
	{
		CCLOG("onChartboostShouldDisplay: %s", name.c_str());
		return true;
	}

	void onChartboostDisplay(const std::string& name)
	{
		CCLOG("onChartboostDisplay: %s", name.c_str());
	}

	void onChartboostDismiss(const std::string& name)
	{
		CCLOG("onChartboostDismiss: %s", name.c_str());
	}

	void onChartboostClose(const std::string& name)
	{
		CCLOG("onChartboostClose: %s", name.c_str());
	}

	void onChartboostClick(const std::string& name)
	{
		CCLOG("onChartboostClick: %s", name.c_str());
	}

	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}

	void onChartboostReward(const std::string& name, int reward)
	{
		CCLOG("onChartboostReward: %s, %d", name.c_str(), reward);
	}

	void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e)
	{
		CCLOG("onChartboostFailedToLoad: %s, %d", name.c_str(), static_cast<int>(e));
	}

	void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e)
	{
		CCLOG("onChartboostFailToRecordClick: %s, %d", name.c_str(), static_cast<int>(e));
	}

	void onChartboostConfirmation()
	{
		CCLOG("onChartboostConfirmation");
	}

	void onChartboostCompleteStore()
	{
		CCLOG("onChartboostCompleteStore");
	}

#endif

};

#endif // __SHARE_SCENE_H__
