#include "NormalEnemy.h"
NormalEnemy::NormalEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo.png", x, y, game) {

	state = game->stateMoving;

	aDyingRight = new Animation("res/Soldado Enemigo/Derecha/animacion_derrota_derecha.png", width, height,
		550, 50, 6, 11, false, game);
	aDyingLeft = new Animation("res/Soldado Enemigo/Izquierda/animacion_derrota_izquierda.png", width, height,
		550, 50, 6, 11, false, game);

	aMovingRight = new Animation("res/Soldado Enemigo/Derecha/animacion_corre_derecha.png", width, height,
		200, 50, 6, 4, true, game);
	aMovingLeft = new Animation("res/Soldado Enemigo/Izquierda/animacion_corre_izquierda.png", width, height,
		200, 50, 6, 4, true, game);
	animation = aMovingLeft;

	vx = 2;
	vxIntelligence = -2;
	vx = vxIntelligence;

}

void NormalEnemy::update() {
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

void NormalEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

Projectile* NormalEnemy::shoot() {
	return NULL;
}


