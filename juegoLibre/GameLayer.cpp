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

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	pointsRecolectable = 0;
	textPointsRecolectable = new Text("hola", WIDTH * 0.92, HEIGHT * 0.14, game);
	textPointsRecolectable->content = to_string(pointsRecolectable);

	background = new Background("res/jungle_background.jpg", WIDTH * 0.5, HEIGHT * 0.5, 0, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	recolectable = new Item(WIDTH * 0.85, HEIGHT * 0.15, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	recolectables.clear();
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	destructibles.clear();

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
	case 'C': {
		cup = new Tile("res/copa.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup); // Realmente no hace falta
		break;
	}
	case 'R': {
		Tile* recolectable = new Tile("res/icono_recolectable.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		recolectable->y = recolectable->y - recolectable->height / 2;
		recolectables.push_back(recolectable);
		space->addDynamicActor(recolectable); 
		break;
	}
	case 'W': {
		Tile* destructible = new Tile("res/bloque_metal.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		destructible->y = destructible->y - destructible->height / 2;
		destructibles.push_back(destructible);
		space->addStaticActor(destructible); 
		break;
	}
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
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
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
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
	
	}
	else if (controlMoveY < 0) {
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


void GameLayer::update() {

	// Nivel superado
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		init();
	}


	// Jugador se cae
	if (player->y > HEIGHT + 80) {
		init();
	}

	space->update();
	background->update();
	player->update();

	for (auto const& enemy : enemies) {
		enemy->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}


	for (auto const& enemy : enemies) {

		if (player->vy > 0 && player->y < enemy->y
			&& player->isOverlap(enemy)
			&& player->lifes > 0 && enemy->state != game->stateDead) {

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
				
			if (player->lifes <= 0) {
				init();
				return;
			}
		}
	}


	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false || projectile->vx == 0) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	//Colisiones, Player - Recolectable
	list<Tile*> deleteRecolectables;
	for (auto const& recolectable : recolectables) {
		if (player->isOverlap(recolectable)) {
			bool rInList = std::find(deleteRecolectables.begin(),
				deleteRecolectables.end(),
				recolectable) != deleteRecolectables.end();

			if (!rInList) {
				deleteRecolectables.push_back(recolectable);
			}


			pointsRecolectable++;
			if (pointsRecolectable % 4 == 0) {
				textPointsRecolectable->content = to_string(pointsRecolectable/4);
				this->recolectable->idle->currentFrame = 2;
			}

			this->recolectable->update();

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
	}

	//Tiles destructibles
	list<Tile*> deleteDestructibles;
	for (auto const& destructible : destructibles) {

		if ((player->y - destructible->y) < 10 && 
			abs(player->x - destructible->x) < 10 &&
			destructionTime % 10 == 0) {

			space->addDynamicActor(destructible);
			space->removeStaticActor(destructible);

			bool dInList = std::find(deleteDestructibles.begin(),
				deleteDestructibles.end(),
				destructible) != deleteDestructibles.end();

			if (!dInList) {
				deleteDestructibles.push_back(destructible);
			}

			destructionTime = 10;
		}
	}

	destructionTime--;

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

	for (auto const& delRecolectable : deleteRecolectables) {
		recolectables.remove(delRecolectable);
		space->removeDynamicActor(delRecolectable);
	}
	deleteRecolectables.clear();

	for (auto const& delDestructible : deleteDestructibles) {
		destructibles.remove(delDestructible);
		space->removeDynamicActor(delDestructible);
	}
	deleteDestructibles.clear();

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

	for (auto const& recolectable : recolectables) {
		recolectable->draw(scrollX, scrollY);
	}

	for (auto const& destructible : destructibles) {
		destructible->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	cup->draw(scrollX, scrollY);
	player->draw(scrollX, scrollY);
	
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	backgroundPoints->draw();
	textPoints->draw();

	textPointsRecolectable->draw();
	recolectable->draw();

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
		}

	}

}

