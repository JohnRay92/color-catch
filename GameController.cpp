#include "GameController.h"

USING_NS_CC;

#define EXTND "-ND"
#define EXTHD "-HD"
#define EXTQHD "-QHD"

GameController *GameController::Instance = NULL;
GameController *GameController::Instantiate()
{
	if (!Instance)
	{
		Instance = new GameController;
	}
	return Instance;
}

float GameController::GetScaleX()
{
	return m_scaleX;
}

float GameController::GetScaleY()
{
	return m_scaleY;
}

float GameController::GetScaleFactor()
{
	return m_scaleFactor;
}

void GameController::SetUpScaleFactors()

{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_extension = (char*)EXTHD;
	m_scaleFactor = 1.0;

	if (visibleSize.width * visibleSize.height > 960 * 640)
	{
		m_scaleFactor = 2.0;
		m_extension = (char*)EXTQHD;
	}

	if (visibleSize.width * visibleSize.height < 960 * 640)
	{
		m_scaleFactor = 0.5;
		m_extension = (char*)EXTND;
	}

	m_scaleX = visibleSize.width / (960 * m_scaleFactor);
	m_scaleY = visibleSize.height / (640 * m_scaleFactor);
}


void GameController::GetFileName(char *Array, const int len, const char *name, const char *fileExt) {
	memset(Array, 0, sizeof(char) * len);
	sprintf(Array, "%s%s%s", name, m_extension, fileExt);
}

