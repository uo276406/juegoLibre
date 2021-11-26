#include "Actor.h"
#include "Enemy.h"


Enemy::Enemy(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 36, 40, game) {

	state = game->stateMoving;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;

}

void Enemy::update() {

}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}


void Enemy::draw(float scrollX, float scrollY) {
}

Projectile* Enemy::shoot() {
	return NULL;
}


