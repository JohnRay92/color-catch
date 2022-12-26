

#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#define GETSCALEX ( GameController::Instantiate()->GetScaleX() )
#define GETSCALEY ( GameController::Instantiate()->GetScaleY() )
#define GETSCALEFAC ( GameController::Instantiate()->GetScaleFactor() )

#define SCALEX(p) ( (p) * GETSCALEX)
#define SCALEY(p) ( (p) * GETSCALEY)

#define SCALEPOS(x,y) ( Vec2 (  GETSCALEX * (x) * GETSCALEFAC, GETSCALEY * (y) * GETSCALEFAC) )

#define SCALEFONT(p) ( (p) * SCALEY * GETSCALEFAC )

#define GETFILENAME(a,l,n,e) \
(GameController::Instantiate()->GetFileName(a,l,n,e))

#define SCALENODE_XY(n) \
	n->setScaleX(GETSCALEX); \
	n->setScaleY(GETSCALEY)

#define SCALENODE_Y(n) \
n->setScale(GETSCALEY)

#define SCALENODE_X(n) \
n->setScale(GETSCALEX)

#include "cocos2d.h"

class GameController
{
public:
	static GameController* Instantiate();
	void LateInit();

	float GetScaleX();
	float GetScaleY();
	float GetScaleFactor();

	void SetUpScaleFactors();
	void GetFileName(char* Array, const int len, const char * name, const char* file_extension);

private:
	GameController() {};
	GameController(GameController const&) {};
	GameController& operator=(GameController const&){};

	static GameController* Instance;
	float m_scaleX;
	float m_scaleY;
	float m_scaleFactor;
	char* m_extension;


	 
};

#endif 