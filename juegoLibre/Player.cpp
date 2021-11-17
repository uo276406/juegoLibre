#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) {

	onAir = false;
	orientation = game->orientationRight;
	state = game->stateMoving;
	audioShoot = new Audio("res/efecto_disparo.wav", false);
	aShootingRight = new Animation("res/Soldado Pistola/Derecha/animacion_disparo_derecha.png",
		width, height, 200, 50, 6, 4, false, game);
	aShootingLeft = new Animation("res/Soldado Pistola/Izquierda/animacion_disparo_izquierda.png",
		width, height, 200, 50, 6, 4, false, game);

	aJumpingRight = new Animation("res/Soldado Pistola/Derecha/animacion_salto_derecha.png",
		width, height, 200, 50, 6, 4, true, game);
	aJumpingLeft = new Animation("res/Soldado Pistola/Izquierda/animacion_salto_izquierda.png",
		width, height, 200, 50, 6, 4, true, game);
	aIdleRight = new Animation("res/Soldado Pistola/Derecha/animacion_quieto_derecha.png", width, height,
		500, 50, 6, 10, true, game);
	aIdleLeft = new Animation("res/Soldado Pistola/Izquierda/animacion_quieto_izquierda.png", width, height,
		500, 50, 6, 10, true, game);
	aRunningRight = new Animation("res/Soldado Pistola/Derecha/animacion_corre_derecha.png", width, height,
		200, 50, 6, 4, true, game);
	aRunningLeft = new Animation("res/Soldado Pistola/Izquierda/animacion_corre_izquierda.png", width, height,
		200, 50, 6, 4, true, game);

	animation = aIdleRight;

}


void Player::update() {
	// En el aire y moviéndose, PASA a estar saltando
	if (onAir && state == game->stateMoving) {
		state = game->stateJumping;
	}
	// No está en el aire y estaba saltando, PASA a moverse
	if (!onAir && state == game->stateJumping) {
		state = game->stateMoving;
	}


	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	bool endAnimation = animation->update();

	if (collisionDown == true) {
		onAir = false;
	}
	else {
		onAir = true;
	}


	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}

	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
		}
	}

	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	// Selección de animación basada en estados
	if (state == game->stateJumping) {
		if (orientation == game->orientationRight) {
			animation = aJumpingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aJumpingLeft;
		}
	}
	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
	}

	if (shootTime > 0) {
		shootTime--;
	}

}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {

	if (shootTime == 0 && state!=game->stateShooting &&
		state != game->stateJumping && vx==0) {
		state = game->stateShooting;
		audioShoot->play();
		aShootingLeft->currentFrame = 0; //"Rebobinar" aniamción
		aShootingRight->currentFrame = 0; //"Rebobinar" aniamción
		shootTime = shootCadence;
		Projectile* projectile;

		if (orientation == game->orientationLeft) {
			projectile = new Projectile("res/Soldado Pistola/Izquierda/bala.png", x, y, game);
			projectile->vx = projectile->vx * -1; // Invertir
		}
		else {
			projectile = new Projectile("res/Soldado Pistola/Derecha/bala.png", x, y, game);
		}
		return projectile;
	}
	else {
		return NULL;
	}
}

void Player::draw(float scrollX, float scrollY) {
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y- scrollY);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y- scrollY);

		}
	}
}

void Player::jump() {
	if (!onAir) {
		vy = -16;
		onAir = true;
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 100;
			// 100 actualizaciones 
		}
	}
}
