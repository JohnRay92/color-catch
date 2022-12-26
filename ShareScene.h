#ifndef __SHARE_SCENE_H__
#define __SHARE_SCENE_H__

#include "cocos2d.h"
#define SDK

#ifdef SDK
#include "PluginAdColony/PluginAdColony.h"
#endif

#ifdef SDK
class ShareScene : public cocos2d::Layer, public sdkbox::AdColonyListener
{
#else
class ShareScene : public cocos2d::Layer
{
#endif
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(ShareScene);

//private:
#ifdef SDK
	void createTestMenu();
	void onShowVideo(cocos2d::Ref* sender);
	void onShowV4vc(cocos2d::Ref* sender);

	void onAdColonyChange(const sdkbox::AdColonyAdInfo& info, bool available);
	void onAdColonyReward(const sdkbox::AdColonyAdInfo& info,
		const std::string& currencyName, int amount, bool success);
	void onAdColonyStarted(const sdkbox::AdColonyAdInfo& info);
	void onAdColonyFinished(const sdkbox::AdColonyAdInfo& info);
	void back(cocos2d::Ref* sender);
#endif
	cocos2d::Label *_status;
	cocos2d::Label *_reward;
	cocos2d::Label *_rewarded;
};

#endif // __SHARE_SCENE_H__
