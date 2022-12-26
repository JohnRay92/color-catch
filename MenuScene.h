#ifndef __NewScene_SCENE_H__
#define __NewScene_SCENE_H__
#define SDK

#ifdef SDK
#include "PluginChartboost/PluginChartboost.h"
#include "PluginFacebook.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "PluginReview/PluginReview.h"
using namespace sdkbox;
#endif

#include "cocos2d.h"

#ifdef SDK
class MenuScene : public cocos2d::Layer, public sdkbox::ReviewListener 
{
#else
class MenuScene : public cocos2d::Layer
{
#endif
#ifdef SDK
protected:
	cocos2d::Label * _txtStat;
	cocos2d::Label * _txtC;
#endif
private:
	char txt[40];
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

	//animation
	cocos2d::Sprite* _ball_1;
	cocos2d::Sprite* _ball_2;
	cocos2d::Sprite* _ball_3;

	//animation methods
	void createAnimation(float dt);
#ifdef SDK
	void onShowReview(cocos2d::Ref* sender);
	void onDisplayAlert();
	void onDeclineToRate();
	void onRate();
	void onRemindLater();
#endif
};
static int high_score;

#endif // __NewScene_SCENE_H__