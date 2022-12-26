#ifndef __LEADERBOARD_SCENE_H__
#define __LEADERBOARD_SCENE_H__

#define SDK

#ifdef SDK
#include "cocos2d.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"

class LeaderboardScene : public cocos2d::Layer, public sdkbox::SdkboxPlayListener
{
protected:
	cocos2d::Label * _txtStat;
	cocos2d::Label * _txtC;

private:
	cocos2d::Sprite * logo;
public:
	void scroll(float dt);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LeaderboardScene);

	void runtests(int fd, const std::string& args);

	void connect(cocos2d::CCObject *sender);
	void disconnect(cocos2d::CCObject *sender);

	void showLeaderboard(cocos2d::CCObject *sender);
	void showAllLeaderboards(cocos2d::CCObject *sender);
	void showAchievements(cocos2d::CCObject *sender);
	void getMyScore(cocos2d::CCObject *sender);
	void loadAchievements(cocos2d::CCObject *sender);

	void achievement_hunter(cocos2d::CCObject* sender);
	void achievement_craftsman(cocos2d::CCObject* sender);
	void achievement_ten_games(cocos2d::CCObject* sender);
	void achievement_incremental(cocos2d::CCObject* sender);

	void send_score(cocos2d::CCObject* sender);

	void increment(cocos2d::CCObject* sender);
	void setSteps(cocos2d::CCObject* sender);
	void revealHidden(cocos2d::CCObject* sender);


	virtual void onConnectionStatusChanged(int connection_status);
	virtual void onScoreSubmitted(const std::string& leaderboard_name, int score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday);
	virtual void onIncrementalAchievementUnlocked(const std::string& achievement_name);
	virtual void onIncrementalAchievementStep(const std::string& achievement_name, int step);
	virtual void onIncrementalAchievementStepError(const std::string& name, int steps, int error_code, const std::string& error_description);
	virtual void onAchievementUnlocked(const std::string& achievement_name, bool newlyUnlocked);
	virtual void onAchievementUnlockError(const std::string& achievement_name, int error_code, const std::string& error_description);
	virtual void onAchievementsLoaded(bool reload_forced, const std::string& json_achievements_info);
	virtual void onSetSteps(const std::string& name, int steps);
	virtual void onSetStepsError(const std::string& name, int steps, int error_code, const std::string& error_description);
	virtual void onReveal(const std::string& name);
	virtual void onRevealError(const std::string& name, int error_code, const std::string& error_description);
	virtual void onMyScore(const std::string& leaderboard_name, int time_span, int collection_type, long score);
	virtual void onMyScoreError(const std::string& leaderboard_name, int time_span, int collection_type, int error_code, const std::string& error_description);
	virtual void onPlayerCenteredScores(const std::string& leaderboard_name,
		int time_span,
		int collection_type,
		const std::string& json_with_score_entries);
	virtual void onPlayerCenteredScoresError(const std::string& leaderboard_name,
		int time_span,
		int collection_type,
		int error_code,
		const std::string& error_description);
};
#endif
#endif // __LEADERBOARD_SCENE_H__
