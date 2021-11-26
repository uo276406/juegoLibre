#include "Grenade.h"

Grenade::Grenade(float x, float y, Game* game) :
	Actor("", x, y - 10, 30, 30, game) {
	vx = 6;
	vy = -10; // La gravedad inicial es 1

	audioExplode = new Audio("res/explosion_11.wav", false);

	animation = new Animation("res/Bomba/animacion_bomba_explosion.png",
		width, height, 525, 75, 6, 7, false, game);
	
	state = game->stateExploding;
}

bool Grenade::isOverlap(Actor* actor) {
	bool overlap = false;
	if (actor->x - actor->width / 2 <= x + 4*width / 2
		&& actor->x + actor->width / 2 >= x - 0.5*width / 2
		&& actor->y + actor->height / 2 >= y - 0.5*height / 2
		&& actor->y - actor->height / 2 <= y + 4*height / 2) {

		overlap = true;
	}
	return overlap;
}

void Grenade::update() {
	if (state == game->stateExploding) {
		bool endAnimation = animation->update();
		if (collisionDown == true) {
			onAir = false;
		}
		else {
			onAir = true;
		}
		if (vy == 0 && !onAir) {
			vx = 0;
			onAir = false;
		}

		//Ha explotado
		if (endAnimation) {
			audioExplode->play();
			state = game->stateDead;
		}
	}
	
}

void Grenade::draw(float scrollX, float scrollY) {
	Actor::draw();

	animation->draw(x - scrollX, y - scrollY);
}
