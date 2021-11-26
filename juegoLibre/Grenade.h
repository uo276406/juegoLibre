#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Animation.h"

class Grenade : public Actor
{
public:
	Grenade(float x, float y, Game* game);
	
	void update();
	
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	bool isOverlap(Actor* actor) override;

	int state;

	Animation* animation; // Referencia a la animación mostrada
	Audio* audioExplode;

	bool onAir = false;

};

