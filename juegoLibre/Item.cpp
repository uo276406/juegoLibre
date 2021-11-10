#include "Item.h"

Item::Item(float x, float y, Game* game) {
	this->x = x;
	this->y = y;
	idle = new Animation("res/recolectable.png",
		32, 32 ,256, 32, 0, 8, true, game);
	idle->currentFrame = 2;
	update();

}

void Item::update() {
	
	idle->update();
}

void Item::draw() {
	idle->draw(x, y);
}
