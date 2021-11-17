#include "JumpEnemy.h"

JumpEnemy::JumpEnemy(float x, float y, Game* game)
	: Enemy("res/asteroide.png", x, y, game) {

	state = game->stateMoving;
	aMoving = new Animation("res/Salvaje/salvaje_izquierda.png", width, height,
		240, 54, 6, 4, true, game);
	aDying = new Animation("res/Salvaje/salvaje_muerte.png", width, height,
		360, 54, 6, 6, false, game);
	
	animation = aMoving;

	vx = 2;
	vxIntelligence = -2;
	vx = vxIntelligence;
}

void JumpEnemy::update() {

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

void JumpEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}
