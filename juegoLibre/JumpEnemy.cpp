#include "JumpEnemy.h"

JumpEnemy::JumpEnemy(float x, float y, Game* game)
	: Enemy("res/asteroide.png", x, y, game) {

	state = game->stateMoving;
	aMovingLeft = new Animation("res/Salvaje/salvaje_izquierda.png", width, height,
		240, 54, 6, 4, true, game);
	aDyingLeft = new Animation("res/Salvaje/salvaje_muerte.png", width, height,
		360, 54, 6, 6, false, game);
	aMovingRight = new Animation("res/Salvaje/enemyrunright.png", width, height,
		256, 64, 6, 4, true, game);
	aDyingRight = new Animation("res/Salvaje/enemydieright.png", width, height,
		384, 64, 6, 6, false, game);
	
	animation = aMovingLeft;

	vx = 2;
	vxIntelligence = -2;
	vx = vxIntelligence;
}

void JumpEnemy::update() {

	jump();

	// Actualizar la animación
	bool endAnimation = animation->update();

	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}


	if (state == game->stateMoving) {
		if (orientation == game->orientationRight) {
			animation = aMovingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aMovingLeft;
		}
	}
	if (state == game->stateDying) {
		if (orientation == game->orientationRight) {
			animation = aDyingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aDyingLeft;
		}
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

void JumpEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void JumpEnemy::jump() {
	if (jumpInterval == 0 && state != game->stateDying) {
		vy = -16;
		jumpInterval = 120;
	}

	jumpInterval--;
}
