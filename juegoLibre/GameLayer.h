#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "NormalEnemy.h"
#include "ShootEnemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"

#include "Audio.h"
#include "Space.h" // importar
#include "Item.h"

#include "Pad.h"

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	Space* space;
	float scrollX;
	float scrollY;
	int mapWidth;
	list<Tile*> tiles;
	
	list<Tile*> destructibles;
	int destructionTime = 9;

	Audio* audioBackground;
	Text* textPoints;
	int points;

	Text* textPointsRecolectable;
	int pointsRecolectable;
	Item* recolectable;

	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	list<Enemy*> enemies;
	list<Tile*> recolectables;
	list<Projectile*> projectiles;
	Tile* cup; // Elemento de final de nivel
	
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	// Elementos de interfaz
	Actor* buttonJump;
	Actor* buttonShoot;
	Pad* pad; 

	list<Projectile*> enemyProjectiles;

};

