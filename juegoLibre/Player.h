#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void jump();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	
	void loseLife();
	bool isDead();
	bool isDying();
	void setDead();
	void setDying();
	int lifes = 3;
	int shootsAvailable = 5;
	int invulnerableTime = 0;
	
	bool onAir;
	int orientation;
	int state;
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aJumpingRight;
	Animation* aJumpingLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* aKnifeRight;
	Animation* aKnifeLeft;
	Animation* aDyingRight;
	Animation* aDyingLeft;

	Animation* animation; // Referencia a la animaci�n mostrada
	Audio* audioShoot;
	Audio* audioKnife;
	int shootCadence = 30;
	int shootTime = 0;
};

