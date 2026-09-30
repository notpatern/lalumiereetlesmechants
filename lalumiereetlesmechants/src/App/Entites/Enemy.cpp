#include "Enemy.h"

Enemy::Enemy() : Entities()
{
}

Enemy::Enemy(const Utilities::Vector2<int>& position, Sprite* sprite) : Entities(position, sprite)
{
}

Enemy::~Enemy()
{
	
}

void Enemy::Update()
{

}
