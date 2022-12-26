////////////////////////////////////////////////////////////////////
//                                                                //
//                   WORKING RELEASE                              //
//                                                                //
////////////////////////////////////////////////////////////////////
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"SimpleAudioEngine.h"
#include "MenuScene.h"

#define SDK
USING_NS_CC;
//#define GODMODE

using namespace cocos2d; 
using namespace ui;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
extern Size visibleSize;
extern int g_reward;
extern bool scene_paused; //pause
extern bool ad_token; //pause
extern int games_played;
const char *m_BEST_SCORE_ = "key3";//high score key
bool ADkiller;


int effect_vol = 5;//effect volume
int background_vol = 5;//back ground volume


Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object, create with physics 
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	//if (!Layer::init())
	//{
		//return false;
	//}
	auto bg = cocos2d::LayerColor::create(Color4B(20, 20, 20, 255));
	this->addChild(bg);
	//Chartboost
#ifdef SDK
	if (!ADkiller)
	{
		sdkbox::PluginAdMob::setListener(new IMListener());
		sdkbox::PluginChartboost::setListener(this);
		sdkbox::PluginChartboost::cache(sdkbox::CB_Location_Default);
		sdkbox::PluginChartboost::cache(sdkbox::CB_Location_LevelComplete);
		sdkbox::PluginAdMob::cache(kHomeBanner);
		sdkbox::PluginAdMob::cache(kGameOverAd);
		sdkbox::PluginAdMob::show(kHomeBanner);
	}
#endif
	/*
	init game properties
	*/
	ClearSoundFlag_Missed();
	effect_vol = CCUserDefault::sharedUserDefault()->getIntegerForKey("key6");//effect volume settings
	background_vol = CCUserDefault::sharedUserDefault()->getIntegerForKey("key9");

	SimpleAudioEngine::getInstance()->setEffectsVolume(effect_vol / 5.0f);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(background_vol / 5.0f);

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("15 Fever.m4a", true);
	scene_paused = false;
	m_gameOver = false;

	m_lastleftSpawnTime = 0;
	m_nextleftSpawnTime = 0.2;
	m_lastRightSpawnTime = 0;
	m_nextRightSpawnTime = 0.2;
	CCUserDefault::sharedUserDefault()->getIntegerForKey("key12");
	ADkiller = CCUserDefault::sharedUserDefault()->getBoolForKey("key15");
	m_health += g_reward + CCUserDefault::sharedUserDefault()->getIntegerForKey("key12");
	g_reward = 0; //reward consumed
	m_balls = Vector<ColorBall*>{40}; // vector of balls
	//game over sprite
	gameover = cocos2d::Sprite::create("Game Over.png");
	gameover->setAnchorPoint(Vec2(0.5, 0.5));
	gameover->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.6));
	gameover->setScale(1.25f);
	gameover->setVisible(false);
	this->addChild(gameover, 1000);
	//create color wheels

	//left color wheel
	_colorWheel_left = ColorWheel::createWithFileName("Color Wheel.png");
	_colorWheel_left->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.78, visibleSize.height* 0.15));
	_colorWheel_left->setLeft(); //set current wheel as left wheel
	_colorWheel_left->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point
	_colorWheel_left->initOptions(this);
	_colorWheel_left->enableTouch();
	
	addChild(_colorWheel_left, 1);

	//right color wheel
	_colorWheel_right = ColorWheel::createWithFileName("Color Wheel.png");
	_colorWheel_right->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.22, visibleSize.height* 0.15));
	_colorWheel_right->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point
	_colorWheel_right->initOptions(this);
	_colorWheel_right->enableTouch();

	addChild(_colorWheel_right, 1);

	_colorWheel_right->addEvents();
	_colorWheel_left->addEvents();

	/*
	screen sprites
	*/
	spritehealth = cocos2d::Sprite::create("life_wheel.344.png");
	spritehealth->setAnchorPoint(Vec2(0.5, 0.5));// Anchor Point
	spritehealth->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.61));
	spritehealth->setScale(1.2f);
	this->addChild(spritehealth, lightIndex);

	background1 = cocos2d::Sprite::create("360blue.png");
	background1->setAnchorPoint(Vec2(0.5, 0.5));
	background1->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height/1.61));
	background1->setScale(1.1f);
	background1->setOpacity(140);
	this->addChild(background1, 0);

	/*
	screen buttons
	*/
	pause_scene = Sprite::create("pause.jpg");
	pause_scene->setAnchorPoint(Vec2(0.5, 0.5));
	pause_scene->setScale(2.0f);
	pause_scene->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2.25));
	pause_scene->setOpacity(0);
	this->addChild(pause_scene, 999);


	play = Button::create("play (1).png");
	play->setAnchorPoint(Vec2(0.5, 0.5));
	play->setScale(0.55f);
	play->setVisible(false);
	this->addChild(play, 1000);


	exit = Button::create("x.png");
	exit->setAnchorPoint(Vec2(0.5, 0.5));
	exit->setScale(0.55f);
	exit->setVisible(false);
	this->addChild(exit, 1000);

	play->setPosition(cocos2d::Vec2(visibleSize.width - (visibleSize.width*0.65), (visibleSize.height / 1.95) - 160));
	exit->setPosition(cocos2d::Vec2(visibleSize.width - (visibleSize.width*0.35), (visibleSize.height / 1.95) - 160));

	play->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			

			if (m_gameOver)
			{
				//Director::getInstance()->popScene();
				auto newGameScene = GameScene::createScene();
				Director::getInstance()->replaceScene(newGameScene);
				Director::getInstance()->resume();
			}
			else
			{
				scene_paused = false;
				pause->setVisible(true);
				pause_scene->setOpacity(0);
				exit->setVisible(false);
				play->setVisible(false);
				_colorWheel_left->enableTouch();
				_colorWheel_right->enableTouch();
				Director::getInstance()->resume();
			}
		}
	});
	exit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
			//auto newScene = MenuScene::createScene();
			//this->removeAllChildrenWithCleanup(true);
			//Director::getInstance()->startAnimation();
			
			Director::getInstance()->popScene();
		}
	});




	pause = Button::create("pause-button-grey-icon-17922.png", "red-pause-button-icon-17925.png", "pause-button-grey-icon-17922.png");
	pause->setScale(0.2f);
	pause->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.2));
	this->addChild(pause, 100);

	pause->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{

		if (type == Widget::TouchEventType::ENDED)
		{
#ifdef SDK
			//chartboost
			if (!ADkiller)
			{
				CCLOG("sdkbox::PluginChartboost::show(sdkbox::CB_Location_Default);");
				sdkbox::PluginChartboost::show("Default");
			}
#endif
			_colorWheel_left->disableTouch();
			_colorWheel_right->disableTouch();
			pause_scene->setOpacity(100);
			exit->setVisible(true);
			play->setVisible(true);
			pause->setVisible(false);
			Director::getInstance()->pause();
			// if you use SimpleAudioEngine, it must resume here
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
	});
	/* game labels
	including score, health, best, and game over
	*/
	m_best = CCUserDefault::sharedUserDefault()->getIntegerForKey(m_BEST_SCORE_);
	sprintf(score_str, "%d", m_score);
	sprintf(health_str, "%d", m_health);
	sprintf(best_str, "%d", m_best);

	m_highScoreLabel_str = AddLabel(42.0f, "BEST", cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.30), Color3B::WHITE);
	m_bestLabel = AddLabel(42.0f, best_str, cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, (visibleSize.height / 1.35) - 30), Color3B::WHITE);

	m_scoreLabel = AddLabel(42.0f, "SCORE", cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.90), Color3B::WHITE,5000);
	m_scoreLabel = AddLabel(42.0f, score_str, cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, (visibleSize.height / 1.95) - 30),Color3B::WHITE,5000);
	m_healthLabel = AddLabel(42.0f, health_str, cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.6), Color3B::RED);
	
	m_gameOverLabel = AddLabel(80.0f, "GAME OVER", cocos2d::Vec2(0.5f, 0.5f), cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.6), Color3B::RED,5000);
	m_gameOverLabel->setVisible(false);



	/*detect contact between balls and color wheel*/
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	schedule(schedule_selector(GameScene::gameUpdate));
	schedule(schedule_selector(GameScene::scroll));
	return true;
}



void GameScene::gameUpdate(float dt) {
	
	//Rect rect = Rect(0, 0, visibleSize.width, visibleSize.height);
	//Background();
	if (scene_paused)
	{
		if (!m_gameOver)
		{
			
			_colorWheel_left->disableTouch();
			_colorWheel_right->disableTouch();
			pause_scene->setOpacity(100);
			exit->setVisible(true);
			play->setVisible(true);
			pause->setVisible(false);
			Director::getInstance()->pause();
			// if you use SimpleAudioEngine, it must resume here
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		scene_paused = false;
	}
	//if (m_soundflag_Taken)
	//{
		//PlaySoundEffect_Taken();
		//ClearSoundFlag_Taken();
	//}
	//if (m_soundflag_Missed)
	//{
		//PlaySoundEffect_Missed();
		//ClearSoundFlag_Missed();
	//}
	//sprintf(score_str, "%d", m_score); //update score label
	//sprintf(health_str, "%d", m_health); //update health label
	//m_scoreLabel->setString(score_str);
	//m_healthLabel->setString(health_str);
	if (m_gameOver == false) {
		if ((m_score >= (score_index1 + 20)) && game_speed < 460 )
		{
			for (auto ball : m_balls)
			{
				if (ball->State == ACTIVEBALL) 
				{
					ball->IncreaseSpeed(game_speed + 10);
				}
			}
			game_speed += 10;
			n += 0.1;
			score_index1 = m_score;

		}
		m_gameTime += dt;
		m_lastleftSpawnTime += dt;
		m_lastRightSpawnTime += dt;
		if (m_lastleftSpawnTime >= m_nextleftSpawnTime)
		{
			m_color = (rand() + BLUE) % 100;
			SetleftSpawnTime();
			SpawnABall(true, m_color);
		}
		if (m_lastRightSpawnTime >= m_nextRightSpawnTime)
		{
			m_color = (rand() + GREEN) % 100;
			SetRightSpawnTime();
			SpawnABall(false, m_color);
		}
	}
}


void GameScene::SetRightSpawnTime() {
	m_lastRightSpawnTime = 0.15;
	m_nextRightSpawnTime = (float)(rand() % BallSpawnTimeVariance) / (10) + (BallSpawnMinTime / (1.8*n));
}

void GameScene::SetleftSpawnTime() {
	m_lastleftSpawnTime = 0.1;
	m_nextleftSpawnTime = (float)(rand() % BallSpawnTimeVariance) / (10) + (BallSpawnMinTime / (1.8*n));
}

ColorBall *GameScene::getNextBall() {

	for (auto ball : m_balls)
	{
		if (ball->State == INACTIVEBALL) {
			ball->setBallSpeed(game_speed);
			return ball;
		}
	}
	char FileName[] = "blue.png";
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	ColorBall *newBall = ColorBall::createWithFileName(FileName);
	newBall->Initialise(game_speed, visibleSize.height);
	this->addChild(newBall, ColorBallIndex);
	m_balls.pushBack(newBall);
	return newBall;
}

void GameScene::SpawnABall(bool isLeft, int color) {
	//auto rotateTo = RotateBy::create(2.0f, 90.0f);
	//spritehealth->runAction(rotateTo);

	ColorBall *Cball = getNextBall();
	//auto size = Director::getInstance()->getWinSize();
	if (isLeft)
	{

		    Cball->setName("ball");
			if (color >= 0 && color < 30)
			{
				Cball->setTexture("red.png");
				Cball->SetColorEnum(RED);
			}
			else if (color >= 30 && color < 60)
			{
				Cball->setTexture("blue.png");
				Cball->SetColorEnum(BLUE);
			}
			else if (color >= 60 && color < 75)
			{
				Cball->setTexture("red_bomb.png");
				Cball->SetColorEnum(BLUE);
				Cball->setName("bomb");
			}
			else if (color >= 75 && color < 90)
			{
				Cball->setTexture("blue_bomb.png");
				Cball->SetColorEnum(RED);
				Cball->setName("bomb");
			}
			else if (color >= 90 && color < 94)
			{///
				if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 4)
				{
					Cball->setTexture("blue_heart.png");
					Cball->SetColorEnum(BLUE_HEART);
					score_index2 = m_score;
					++max_tokens;
				}
				else
				{
					Cball->setTexture("blue.png");
					Cball->SetColorEnum(BLUE);
				}

			}
			else if (color >= 94 && color < 97)
			{
				if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 4)
				{
					Cball->setTexture("blue_heart.png");
					Cball->SetColorEnum(BLUE_HEART);
					score_index2 = m_score;
					++max_tokens;
				}
				else
				{
					Cball->setTexture("blue_bomb.png");
					Cball->SetColorEnum(RED);
					Cball->setName("bomb");
				}

			}
			else
			{
				if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 6)
				{
					Cball->setTexture("red_heart.png");
					Cball->SetColorEnum(RED_HEART);
					score_index2 = m_score;
					++max_tokens;
				}
				else
				{
					Cball->setTexture("red_bomb.png");
					Cball->SetColorEnum(BLUE);
					Cball->setName("bomb");
				}
			}
			Cball->m_left = true;
			Cball->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.78, visibleSize.height* 0.6));
			Cball->Start();
		}
	else
	{
		Cball->setName("ball");
		if (color >= 0 && color < 30)
		{
			Cball->setTexture("red.png");
			Cball->SetColorEnum(RED);
		}
		else if (color >= 30 && color < 60)
		{
			Cball->setTexture("blue.png");
			Cball->SetColorEnum(BLUE);
		}
		else if (color >= 60 && color < 75)
		{
			Cball->setTexture("red_bomb.png");
			Cball->SetColorEnum(BLUE);
			Cball->setName("bomb");
		}
		else if (color >= 75 && color < 90)
		{
			Cball->setTexture("blue_bomb.png");
			Cball->SetColorEnum(RED);
			Cball->setName("bomb");
		}
		else if (color >= 90 && color < 94)
		{///
			if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 4)
			{
				Cball->setTexture("blue_heart.png");
				Cball->SetColorEnum(BLUE_HEART);
				score_index2 = m_score;
				++max_tokens;
			}
			else
			{
				Cball->setTexture("blue.png");
				Cball->SetColorEnum(BLUE);
			}

		}
		else if (color >= 94 && color < 97)
		{
			if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 4)
			{
				Cball->setTexture("red_heart.png");
				Cball->SetColorEnum(RED_HEART);
				score_index2 = m_score;
				++max_tokens;
			}
			else
			{
				Cball->setTexture("red.png");
				Cball->SetColorEnum(RED);
			}

		}
		else
		{
			if ((m_score >= score_index2 + 60) && max_tokens <= 4 && m_health < 4)
			{
				Cball->setTexture("red_heart.png");
				Cball->SetColorEnum(RED_HEART);
				score_index2 = m_score;
				++max_tokens;
			}
			else
			{
				Cball->setTexture("blue.png");
				Cball->SetColorEnum(BLUE);
			}
		}
		Cball->m_left = false;
		Cball->setPosition(Vec2(visibleSize.width - visibleSize.width * 0.22, visibleSize.height* 0.6));
		Cball->Start();
	}
}


// this function handels contact between ball and color wheel
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode(); //physics node A
	auto nodeB = contact.getShapeB()->getBody()->getNode(); //physics node B

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == nodeB->getTag()) //check if the ball's color and the wheel's color are identical
		{

				if (nodeA->getName() == "ball" || nodeA->getName() == "bomb") //if nodeA is the ball
				{

					if (nodeA->getName() == "ball")
					{
						nodeA->setOpacity(0); //hide the ball
						animate_scaleup(nodeB);
						++m_score; //increase score
						sprintf(score_str, "%d", m_score); //update score label
						m_scoreLabel->setString(score_str); // set string to current score label
						PlaySoundEffect_Taken(); //play sound effect
						//SetSoundFlag_Taken();//play sound effect
					}
				}
				else
				{
					if (nodeB->getName() == "ball")
					{
						nodeB->setOpacity(0);//hide the ball
						animate_scaleup(nodeA);
						++m_score; //increase score
						sprintf(score_str, "%d", m_score); //update score label
						m_scoreLabel->setString(score_str); // set string to current score label
						PlaySoundEffect_Taken(); //play sound effect
						//SetSoundFlag_Taken();//play sound effect
					}
				}
				

		}
		else if (nodeA->getTag() % 4 == nodeB->getTag() % 4)//check if heart ball was taken
		{
			++m_health; //increase health
			sprintf(health_str, "%d", m_health); //update health label
			m_healthLabel->setString(health_str); // set string to current health label
			PlaySoundEffect_Taken();
			if (nodeA->getName() == "ball")
			{
				nodeA->setOpacity(0); //hide the ball
				animate_scaleup(nodeB);
			}
			else
			{
				nodeB->setOpacity(0);//hide the ball
				animate_scaleup(nodeA);
			}
		}
		else
		{
			if (nodeB->getName() == "ball" || nodeB->getName() == "bomb")
			{
				if (nodeB->getTag() == RED_HEART
					|| nodeB->getTag() == BLUE_HEART)
				{
					nodeB->setOpacity(90);
				}
				else
				{
					--m_health;
					sprintf(health_str, "%d", m_health); //update health label
					m_healthLabel->setString(health_str); // set string to current health label
					PlaySoundEffect_Missed();
					//SetSoundFlag_Missed();//play sound effect
					nodeB->setOpacity(90);
					animate_scaledown(nodeA);
				}
			}
			else
			{
				if (nodeA->getTag() == RED_HEART
					|| nodeA->getTag() == BLUE_HEART)
				{
					nodeA->setOpacity(90);
				}
				else
				{
					--m_health;
					sprintf(health_str, "%d", m_health); //update health label
					m_healthLabel->setString(health_str); // set string to current health label
					PlaySoundEffect_Missed();
					//SetSoundFlag_Missed();//play sound effect
					nodeA->setOpacity(90);
					animate_scaledown(nodeB);
				}
			}


			if (m_health <= 0)
			{
#ifndef GODMODE
				++ games_played;
				//PlaySoundEffect_Missed();
				m_gameOver = true;
				/*
				check for high score
				*/
				if (m_best < m_score)
				{
					CCUserDefault::sharedUserDefault()->setIntegerForKey(m_BEST_SCORE_, m_score);
					CCUserDefault::sharedUserDefault()->flush();
				}
				gameover->setVisible(true);
				m_bestLabel->setVisible(false);
				m_highScoreLabel_str->setVisible(false);
				m_healthLabel->setVisible(false);
				spritehealth->setVisible(false);
				_colorWheel_left->disableTouch();
				_colorWheel_right->disableTouch();
				pause_scene->setOpacity(200);
				exit->setVisible(true);
				play->setVisible(true);
				pause->setVisible(false);
				Director::getInstance()->pause();

#ifdef SDK
				if (!ADkiller)
				{
					if (games_played % 6 == 0)
					{
						//chartboost
						CCLOG("sdkbox::PluginChartboost::show(sdkbox::CB_Location_Default);");
						if (sdkbox::PluginAdMob::isAvailable(kGameOverAd))
						{
							sdkbox::PluginAdMob::show(kGameOverAd);
						}
						else
						{
							sdkbox::PluginChartboost::show("Default");
						}
					}
					else if (games_played % 3 == 0)
					{
						//chartboost
						CCLOG("sdkbox::PluginChartboost::show(sdkbox::CB_Location_Default);");
						if (sdkbox::PluginChartboost::isAvailable("Default"))
						{
							sdkbox::PluginChartboost::show("Default");
						}
						else
						{
							sdkbox::PluginAdMob::show(kGameOverAd);
						}
					}
				}
				
				
#endif
#endif

			}
		}
		//m_scoreLabel->setString(score_str);
		//m_healthLabel->setString(health_str);

	}
	return false;
}

//add label method
Label * GameScene::AddLabel(const float fontSize, const char *text, const cocos2d::Vec2 anchor, const cocos2d::Vec2 position, Color3B color, int index)
{
	Label *newLabel = Label::createWithTTF(text, FontName, fontSize);
	newLabel->setAnchorPoint(anchor);
	newLabel->setPosition(position);
	newLabel->setColor(color);
	newLabel->getShadowOffset();
	this->addChild(newLabel, index); //INDEX modify
	return newLabel;
}

//scrolling
void GameScene::scroll(float dt) {

	//if (m_soundflag_Taken)
	//{
		//PlaySoundEffect_Taken();
		//ClearSoundFlag_Taken();
	//}
	//if (m_soundflag_Missed)
	//{
		//PlaySoundEffect_Missed();
		//ClearSoundFlag_Missed();
	//}
	//sprintf(score_str, "%d", m_score); //update score label
	//sprintf(health_str, "%d", m_health); //update health label
	//m_scoreLabel->setString(score_str); // set string to current score label
	//m_healthLabel->setString(health_str); // set string to current health label

	auto health_angle = spritehealth->getRotation();
	health_angle += 0.6f;
	auto background_angle = background1->getRotation();
	background_angle -= 0.1f + (n/10);
	spritehealth->setRotation(health_angle);
	background1->setRotation(background_angle);
	int RR = background1->getRotation();
	if (abs(RR) > 360)
		RR = RR % 360;
	if (RR < 0)
		RR += 360;
	if (RR >= 0 && RR < 180)
	{
		background1->setTexture("360blue.png");
	}
	else if (RR >= 180 && RR < 360)
	{
		background1->setTexture("360red.png");
	}


	background1->setScale(1.25f);
}

void GameScene::PlaySoundEffect_Taken()
{
	SimpleAudioEngine::getInstance()->playEffect("Hyper GoldenDays Clave.wav", false, 1.0f, 1.0f, 1.0f);
}

void GameScene::PlaySoundEffect_Missed()
{
	SimpleAudioEngine::getInstance()->playEffect("Hyper DoubleBarrel Danger FX.wav", false, 1.0f, 1.0f, 1.0f);
}

void GameScene::animate_scaleup(Node* node)
{
	auto scale = node->getScale();
	auto scaleup = ScaleTo::create(0.1f, wheelscalemax); // make scale up and down animation for the wheel
	auto scaledown = ScaleTo::create(0.1f, scale);
	// create a sequence with the actions and callbacks
	auto posseq = Sequence::create(scaleup, scaledown, nullptr);

	// run it
	node->runAction(posseq);
}

void GameScene::animate_scaledown(Node* node)
{
	auto scale = node->getScale();
	auto scaleupnegative = ScaleTo::create(0.2f, wheelscalemin);// make scale up and down animation for the wheel
	auto scaledown = ScaleTo::create(0.1f, scale);
	// create a sequence with the actions and callbacks
	auto negseq = Sequence::create(scaleupnegative, scaledown, nullptr);
	// run it
	node->runAction(negseq);
}




