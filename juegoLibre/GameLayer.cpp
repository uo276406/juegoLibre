#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}


void GameLayer::init() {

	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

	space = new Space(1);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();

	//audioBackground = new Audio("res/jungla_ambiente.mp3", true);
	audioBackground = game->getAudioSound("res/jungla_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.1, game);
	textPoints->content = to_string(points);
	backgroundPoints = new Actor("res/copa.png",
		WIDTH * 0.85, HEIGHT * 0.1, 40, 32, game);

	background = new Background("res/jungle_background.jpg", WIDTH * 0.5, HEIGHT * 0.5, 0, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	mines.clear();
	resources.clear();
	grenades.clear();

	loadMap("res/" + to_string(game->currentLevel) + ".txt");
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	
	case 'E': {
		Enemy* enemy = new NormalEnemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'J': {
		Enemy* enemy = new JumpEnemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case '1': {
		player = new Player(x, y, game);

		initLifes(player);
		initShoots(player);
		initGrenades(player);

		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case 'M': {
		Mine* mine = new Mine(x, y, game);
		// modificación para empezar a contar desde el suelo.
		mine->y = mine->y - mine->height / 2;
		mines.push_back(mine);
		space->addDynamicActor(mine);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/musgo.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}

void GameLayer::initLifes(Player* player) {
	lifes = player->lifes;
	textLifes = new Text("hola", WIDTH * 0.72, HEIGHT * 0.1, game);
	textLifes->content = to_string(lifes);

	lifesIcon = new Actor("res/corazon.png",
		WIDTH * 0.65, HEIGHT * 0.1, 44, 36, game);
}

void GameLayer::initShoots(Player* player) {
	shoots = player->shootsAvailable;
	textShootsAvailable = new Text("hola", WIDTH * 0.48, HEIGHT * 0.1, game);
	textShootsAvailable->content = to_string(shoots);

	shootsIcon = new Actor("res/lanza.png",
		WIDTH * 0.42, HEIGHT * 0.1, 20, 20, game);
}

void GameLayer::initGrenades(Player* player) {
	numGrenades = player->grenadesAvailable;
	textGrenades = new Text("hola", WIDTH * 0.32, HEIGHT * 0.1, game);
	textGrenades->content = to_string(numGrenades);

	grenadesIcon = new Actor("res/Bomba/bomba.png",
		WIDTH * 0.24, HEIGHT * 0.1, 30, 30, game);
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Cambio automático de input
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}

		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}
	//procesar controles
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();

		shoots = player->shootsAvailable;
		textShootsAvailable->content = to_string(shoots);

		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}
	}
	//Lanzar granada
	if (controlGrenade) {
		Grenade* newGrenade = player->throwGrenade();
		numGrenades = player->grenadesAvailable;
		textGrenades->content = to_string(numGrenades);
		
		if (newGrenade != NULL) {
			space->addDynamicActor(newGrenade);
			grenades.push_back(newGrenade);
		}
		controlGrenade = false;
	}

	// Eje X
	if (controlMoveX > 0 && !player->isDying()) {
		player->moveX(1);
	}
	else if (controlMoveX < 0 && !player->isDying()) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0 && !player->isDying()) {
	
	}
	else if (controlMoveY < 0 && !player->isDying()) {
		player->jump();
	}
	else {

	}
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
	
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}

	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}

		}
		else {
			pad->clicked = false; // han sacado el ratón del pad
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}

	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

void GameLayer::generateResources() {
	newResouceTime--;
	if (newResouceTime <= 0 && resources.size() <= 2) {

		int rX = rand() % (((int) scrollX + mapWidth) - (int) scrollX);

		SupplyResource* resource = 
			new SupplyResource(rX, 0, game);
		resources.push_back(resource);
		space->addDynamicActor(resource);

		newResouceTime = 250;
	}
}


void GameLayer::update() {

	// Nivel superado
	if (points >= pointsToPassLevel) {
		points = 0;
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		init();
	}

	if (player->isDead()) {
		init();
	}

	// Jugador se cae
	if (player->y > HEIGHT + 80) {
		init();
	}

	space->update();
	background->update();
	player->update();

	generateResources();

	for (auto const& enemy : enemies) {
		enemy->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	for (auto const& grenade : grenades) {
		grenade->update();
	}

	for (auto const& mine : mines) {
		mine->update();
	}

	for (auto const& enemy : enemies) {

		if (player->vy > 0 && player->y < enemy->y
			&& player->isOverlap(enemy)
			&& player->lifes > 0 && enemy->state != game->stateDying) {

			enemy->impacted();
			player->lifes++;
			player->invulnerableTime = 0;
			points++;
			textPoints->content = to_string(points);
			player->onAir = false;
			player->jump();
		}

		if (player->isOverlap(enemy) && 
			enemy->state != game->stateDead &&
			enemy->state != game->stateDying) {
				player->loseLife();

				lifes = player->lifes;
				textLifes->content = to_string(lifes);
		}
	}

	for (auto const& mine : mines) {
		//Jugador overlapea con una mina
		if (mine->isOverlap(player) && mine->isIdle()) {
			mine->setExploding();
		}

		//Si sigue en contacto con la mina cuando explota se muere el enemigo
		if (mine->isOverlap(player) && mine->hasExploded()) {
			player->setDying();
		}
	}

	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<Mine*> deleteMines;
	list<SupplyResource*> deleteResources;
	list<Grenade*> deleteGrenades;

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false || 
			projectile->vx == 0 ||
			projectile->lifetime < projectile->minLifetimeToBePainted) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	//Latas fuera de rango
	for (auto const& resource : resources) {
		if (resource->isOverlap(player)) {

			player->reload();
			shoots = player->shootsAvailable;
			textShootsAvailable->content = to_string(shoots);
			numGrenades = player->grenadesAvailable;
			textGrenades->content = to_string(numGrenades);
			
			bool rInList = std::find(deleteResources.begin(),
				deleteResources.end(),
				resource) != deleteResources.end();

			if (!rInList) {
				deleteResources.push_back(resource);
			}
		}
	}

	for (auto const& mine : mines) {
		if (mine->hasExploded()) {
			bool mInList = std::find(deleteMines.begin(),
				deleteMines.end(),
				mine) != deleteMines.end();

			if (!mInList) {
				deleteMines.push_back(mine);
			}
		}
	}

	// Colisiones , Enemy - Projectile
	//				Player(saltando) - enemy

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile) && enemy->state != game->stateDying) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}


				enemy->impacted();
				points++;
				textPoints->content = to_string(points);


			}
		}
		for (auto const& grenade : grenades) {
			if (grenade->isOverlap(enemy) && enemy->state != game->stateDying
				&& grenade->state == game->stateDead) {
				bool gInList = std::find(deleteGrenades.begin(),
					deleteGrenades.end(),
					grenade) != deleteGrenades.end();

				if (!gInList) {
					deleteGrenades.push_back(grenade);
				}


				enemy->impacted();
				points++;
				textPoints->content = to_string(points);

			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& grenade : grenades) {
		if (grenade->state == game->stateDead) {
			bool gInList = std::find(deleteGrenades.begin(),
				deleteGrenades.end(),
				grenade) != deleteGrenades.end();

			if (!gInList) {
				deleteGrenades.push_back(grenade);
			}
		}
	}

	for (auto const& delRes : deleteResources) {
		resources.remove(delRes);
		space->removeDynamicActor(delRes);
	}
	deleteResources.clear();

	for (auto const& delMine : deleteMines) {
		mines.remove(delMine);
		space->removeDynamicActor(delMine);
	}
	deleteMines.clear();

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
	}
	deleteProjectiles.clear();

	for (auto const& delGrenade : deleteGrenades) {
		grenades.remove(delGrenade);
		space->removeDynamicActor(delGrenade);
	}
	deleteGrenades.clear();


	cout << "update GameLayer" << endl;
}

void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}

	// limite abajo
	if (player->y > HEIGHT * 0.3) {
		if (player->y - scrollY < HEIGHT * 0.3) {
			scrollY = player->y - HEIGHT * 0.3;
		}
	}

	// limite arriba
	if (player->y < mapWidth - HEIGHT * 0.3) {
		if (player->y - scrollY > HEIGHT * 0.7) {
			scrollY = player->y - HEIGHT * 0.7;
		}
	}
}


void GameLayer::draw() {
	calculateScroll();

	background->draw();
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		if(projectile->lifetime > projectile->minLifetimeToBePainted)
			projectile->draw(scrollX, scrollY);
	}

	for (auto const& grenade : grenades) {
		grenade->draw(scrollX, scrollY);
	}

	for (auto const& resource : resources) {
		resource->draw(scrollX, scrollY);
	}

	player->draw(scrollX, scrollY);
	
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	for (auto const& mine : mines) {
		mine->draw(scrollX, scrollY);
	}

	backgroundPoints->draw();
	textPoints->draw();

	textLifes->draw();
	lifesIcon->draw();

	textShootsAvailable->draw();
	shootsIcon->draw();

	textGrenades->draw();
	grenadesIcon->draw();

	//textPointsRecolectable->draw();
	//recolectable->draw();

	// HUD
	if (game->input == game->inputMouse) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}


	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		case SDLK_g: // granada
			controlGrenade = true;
			break;
		}
	}

	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		case SDLK_g: 
			controlGrenade = false;
			break;
		}

	}

}

