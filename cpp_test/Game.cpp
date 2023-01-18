#include "Game.hpp"

// private
void Game::initWindow() {
	this->videoMode.width = 800;
	this->videoMode.height = 600;
	this->window = new sf::RenderWindow(this->videoMode, "test", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(FPS); // frame
}
void Game::initData() {
	srand(static_cast<unsigned>(time(NULL)));
	this->window = nullptr;

	this->spawnTime = ENEMY_COOL;
}
void Game::initFonts() {
	this->font.loadFromFile("./Fonts/test.ttf")
}

// public
Game::Game() {
	this->initData();
	this->initWindow();
}
Game::~Game() {
	delete this->window;
}

void Game::updateMousePos() {
	this->mouse = sf::Mouse::getPosition(*this->window);

	this->mousePosView = this->window->mapPixelToCoords(this->mouse);
}

void Game::pollEvents() {
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;

		default:
			break;
		}
	}
}

void Game::update() {
	this->pollEvents();
	this->updateMousePos();
	this->champ.update();

	this->spawnTime -= 1.f;
	if (this->spawnTime <= 0) {
		this->spawnTime = ENEMY_COOL;
		this->spawnEnemies();
	}

	// std::cout << "mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << "\n";
	std::cout << "mouse pos: " << this->mouse.x << " " << this->mouse.y << ", spTime: " << this->spawnTime << "\n";
}
void Game::render() {
	/*
		clear old frame
		render objects
		display frame in window 
	*/
	this->window->clear(); // sf::Color(0xff, 0, 0, 0xff)

	// Draw game Object
	for (int i=0; i < enemies.size(); i++) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (this->enemies[i].checkIn(this->mousePosView) == true) {
				this->enemies[i].setDie(true);
				continue;
			}
		}
		if (this->champ.checkIn(enemies[i].getShape()) == true) {
			this->champ.reduceHp(1);
		}
		// this->window->draw(enemies[i].getShape());
		enemies[i].draw(*this->window);
	}
	this->champ.draw(*this->window);

	this->window->display();


	this->enemiesMove();

	std::vector<Enemy>::iterator it = enemies.begin();
	for (; it != enemies.end(); it++) {
		if (it->getDie() == true) {
			enemies.erase(it);
			break ;
		}
	}
}

void Game::spawnEnemies() {
	// static_cast<float>(rand() % static_cast<int>((this->window->getSize().y - this->enemyShape.getSize().y)))
	this->enemyShape.setPosition(
		static_cast<float>(rand() % static_cast<int>((this->window->getSize().x - this->enemyShape.getSize().x))),
		0.f
	);
	this->enemyShape.setFillColor(sf::Color::Red);
	this->enemyShape.setSize(sf::Vector2f(70.f, 70.f));
	this->enemyShape.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemyShape.setOutlineColor(sf::Color::Magenta);
	this->enemyShape.setOutlineThickness(5.f);
	this->enemy.setShape(this->enemyShape);

	this->enemies.push_back(this->enemy);
}

void Game::enemiesMove() { // enemy class
	for (int i=0; i<this->enemies.size(); i++) {
		this->enemies[i].dropEnemy(2);
	}
}


const bool Game::isOpen() const {
	return this->window->isOpen();
}