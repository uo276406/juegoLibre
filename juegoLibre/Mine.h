#pragma once

#include "Actor.h"
#include "Animation.h"
#include "Audio.h"

class Mine : public Actor
{
public:
	Mine(float x, float y, Game* game);
	Animation* exploding;
	Animation* animation;
	Audio* audioExplode;

	int state;
	float x;
	float y;

	void update();
	void setExploding();
	bool isIdle();
	bool hasExploded();
	void draw(float scrollX, float scrollY);
	bool isOverlap(Actor* actor) override;

};

