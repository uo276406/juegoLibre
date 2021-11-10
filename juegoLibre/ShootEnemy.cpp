#include "ShootEnemy.h"

ShootEnemy::ShootEnemy(float x, float y, Game* game)
	: Enemy("res/asteroide.png", x, y, game) {

	state = game->stateMoving;

	animation = new Animation("res/asteroide.png", width, height,
		50, 51, 1, 1, false, game);

	vx = 3;
	vxIntelligence = -3;
	vx = vxIntelligence;


}

void ShootEnemy::update() {
	if (state == game->stateDying) {
		state = game->stateDead;
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
	cadence--;
}

void ShootEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

Projectile* ShootEnemy::shoot() {
	//state = game->stateShooting;
	Projectile* projectile = new Projectile("res/disparo_enemigo.png", x, y, game);
	if (this->vx < 0) {
		projectile->vx = projectile->vx * -1; // Invertir
	}
	else {
		projectile->vx = projectile->vx * 1;
	}
	return projectile;
	
}