#pragma once

#include "Actor.h"


class Projectile : public Actor
{
public:
	Projectile(string filename, float x, float y, Game* game);
	void update();

	long lifetime = LONG_MAX;
	int const minLifetimeToBePainted = 2;

};

