#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) {

	onAir = false;
	orientation = game->orientationRight;
	state = game->stateMoving;
	audioShoot = new Audio("res/disparo.wav", false);
	audioKnife = new Audio("res/cuchillo_desenfundar.wav", false);
	audioReload = new Audio("res/Reload_Gun.wav", false);

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

	aDyingRight = new Animation("res/Soldado Pistola/Derecha/animacion_derrota_derecha.png", width, height,
		1150, 50, 6, 23, false, game);
	aDyingLeft = new Animation("res/Soldado Pistola/Izquierda/animacion_derrota_izquierda.png", width, height,
		1150, 50, 6, 23, false, game);

	aKnifeRight = new Animation("res/Soldado Cuchillo/Derecha/animacion_ataque_derecha.png", width, height,
		300, 50, 6, 6, false, game);
	aKnifeLeft = new Animation("res/Soldado Cuchillo/Izquierda/animacion_ataque_izquierda.png", width, height,
		300, 50, 6, 6, false, game);

	animation = aIdleRight;

}

void Player::update() {
	//Se terminan las vidas pasa a morir
	if (lifes == 0) {
		state = game->stateDying;
	}
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

		if (state == game->stateDying) {
			state = game->stateDead;
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
			if (shootsAvailable > 0) {
				animation = aShootingRight;
			}
			else {
				animation = aKnifeRight;
			}
		}
		if (orientation == game->orientationLeft) {
			if (shootsAvailable > 0) {
				animation = aShootingLeft;
			}
			else {
				animation = aKnifeLeft;
			}
				
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
		if (shootsAvailable > 0) {
			audioShoot->play();
			shootsAvailable--;
			aShootingLeft->currentFrame = 0; //"Rebobinar" aniamción
			aShootingRight->currentFrame = 0; //"Rebobinar" aniamción
		}
		else {
			audioKnife->play();
			aKnifeLeft->currentFrame = 0; //"Rebobinar" aniamción
			aKnifeRight->currentFrame = 0; //"Rebobinar" aniamción
		}
		shootTime = shootCadence;
		Projectile* projectile;

		if (orientation == game->orientationLeft) {
			projectile = new Projectile("res/Soldado Pistola/Izquierda/bala.png", x, y, game);
			projectile->vx = projectile->vx * -1; // Invertir
		}
		else {
			projectile = new Projectile("res/Soldado Pistola/Derecha/bala.png", x, y, game);
		}
		//Si usa cuchillo
		if (shootsAvailable <= 0) {
			projectile->lifetime = 3;
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

bool Player::isDead() {
	return state == game->stateDead;
}

bool Player::isDying() {
	return state == game->stateDying;
}

void Player::setDead() {
	state = game->stateDead;
}

void Player::setDying() {
	state = game->stateDying;
}

void Player::reload() {
	this->shootsAvailable = reloadingValue;
	this->grenadesAvailable = 2;
	audioReload->play();
}

Grenade* Player::throwGrenade() {
	if (state != game->stateShooting &&
		state != game->stateJumping && vx == 0) {
		if (grenadesAvailable > 0) {
			Grenade* grenade = new Grenade(x, y, game);
			grenade->onAir = true;

			if (orientation == game->orientationLeft) {
				grenade->vx = grenade->vx * -1;
			}
		
			grenadesAvailable--;
			cout << grenadesAvailable;

			return grenade;
		}
		
	}
	else {
		return NULL;
	}
}
