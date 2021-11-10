#pragma once

#include "Actor.h"
#include "Animation.h" 
#include "Projectile.h"

class Enemy : public Actor
{
public:
	Enemy(string filename, float x, float y, Game* game);

	virtual void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	virtual void update();
	virtual Projectile* shoot();
	void impacted(); // Recibe impacto y pone animación de morir
	float vxIntelligence;
	int state;
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

	int enemyShootCadence = 30;

};
