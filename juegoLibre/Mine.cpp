#include "Mine.h"

Mine::Mine(float x, float y, Game* game) : 
		Actor("res/Mina/mina.png", x, y, 50, 30, game) {
	this->x = x;
	this->y = y;
	this->state = game->stateIdle;

	//audioExplode = new Audio("res/Explosion_11.wav", false);
	audioExplode = game->getAudioSound("res/Explosion_11.wav", false);

	animation = new Animation("res/Mina/mina.png",
		width, height, 50, 30, 0, 1, false, game);

	exploding = new Animation("res/Mina/animacion_mina_explota.png",
		width, height, 400, 30, 4, 8, false, game);

}

bool Mine::isOverlap(Actor* actor) {
	bool overlap = false;
	if (actor->x - actor->width / 2 <= (x + width / 2) + 4
		&& actor->x + actor->width / 2 >= (x - width / 2) - 4
		&& actor->y + actor->height / 2 >= (y - height / 2) - 4
		&& actor->y - actor->height / 2 <= (y + height / 2) + 4) {

		overlap = true;
	}
	return overlap;
}


void Mine::update() {

	if (game->stateExploding == state) {
		animation = exploding;
		audioExplode->play();
		state = game->stateDying;
	}

	bool endAnimation = animation->update();

	if (state == game->stateDying && endAnimation) {
		state = game->stateDead;
	}

}

void Mine::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Mine::setExploding() {
	state = game->stateExploding;
}

bool Mine::isIdle() {
	return state == game->stateIdle;
}

bool Mine::hasExploded() {
	return state == game->stateDead;
}
