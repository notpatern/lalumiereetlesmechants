#include "GameManager.h"

GameManager::GameManager() : m_missilesPool{6,12}
{

}

void GameManager::Update(float deltaTime)
{
	m_missilesPool.Update(deltaTime);
}
