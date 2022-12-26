#ifndef __PURCHASES_SCENE_H__
#define __PURCHASES_SCENE_H__

#include "cocos2d.h"
#include "PluginIAP/PluginIAP.h"
class PurchasesScene : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(PurchasesScene);

	// a selector callback
	void onRequestIAP(cocos2d::Ref* sender);
	void onRestoreIAP(cocos2d::Ref* sender);
	void onIAP(cocos2d::Ref* sender);

	void updateIAP(const std::vector<sdkbox::Product>& products);
	virtual void onInitialized(bool ok) override;
	virtual void onSuccess(sdkbox::Product const& p) override;
	virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
	virtual void onCanceled(sdkbox::Product const& p) override;
	virtual void onRestored(sdkbox::Product const& p) override;
	virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
	virtual void onProductRequestFailure(const std::string &msg) override;
	void onRestoreComplete(bool ok, const std::string &msg) override;

	int _coinCount;
	cocos2d::Label* _txtCoin;

	//cocos2d::Menu * _iapMenu;
	std::vector<sdkbox::Product> _products;
};

#endif // __PURCHASES_SCENE_H__
