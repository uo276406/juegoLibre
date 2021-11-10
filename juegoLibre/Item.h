#pragma once

#include "Actor.h"
#include "Animation.h"

class Item
{
public:
	Item(float x, float y, Game* game);
	Animation* idle;

	float x;
	float y;

	void update();
	void draw();
};

