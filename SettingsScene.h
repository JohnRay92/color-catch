#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "GameProperties.h"

class SettingsScene : public cocos2d::Layer
{
private:
	//volume labels
	cocos2d::Label * m_effLabel;
	cocos2d::Label * m_BGLabel;
	cocos2d::Label * sounds;
	cocos2d::Label * music;

	//Game buttons
	cocos2d::ui::Button* exit;

	//strings
	char eff_str[2];
	char BG_str[2];

	cocos2d::Label * AddLabel(const float fontSize, const char *text, const cocos2d::Vec2 anchor, const cocos2d::Vec2 position, cocos2d::Color3B color, int index = 100);
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SettingsScene);
};

#endif // __SETTINGS_SCENE_H__

