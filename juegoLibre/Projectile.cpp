#include "Projectile.h"

Projectile::Projectile(string filename,float x, float y, Game* game) :
	Actor(filename, x, y-10, 30, 30, game) {
	vx = 9;
	vy = -1; // La gravedad inicial es 1
}

void Projectile::update() {
	vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla 
}
