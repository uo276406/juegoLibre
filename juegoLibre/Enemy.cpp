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
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}


	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
	}
	else {
		vx = 0;
	}



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


