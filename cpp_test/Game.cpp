#include "Game.hpp"

// private
void Game::initWindow() {
	this->videoMode.width = WIDTH;
	this->videoMode.height = HEIGHT;
	this->window = new sf::RenderWindow(this->videoMode, "test", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(FPS); // frame
}
void Game::initData() {
	srand(static_cast<unsigned>(time(NULL)));
	this->window = nullptr;
}
void Game::initGame() {
	this->score = 0;
	this->isOver = false;
	this->isCrush = false;
	this->champ->setHp(100);
	this->spawnTime = ENEMY_COOL;
	while (enemies.size() != 0) {
		enemies.erase(enemies.begin());
	}
	this->enemies.clear();
}
void Game::initFonts() {
	if (this->font.loadFromFile("Fonts/sansation.ttf") == false) {
		std::cout << "Error::fail to get fonts\n";
	} else {
		this->uiText.setFont(this->font);
		this->uiText.setCharacterSize(35);
		this->uiText.setFillColor(sf::Color::White);
		this->uiText.setString("NONE");
	}
}

// public
Game::Game() {
	this->champ = new Champ(WIDTH, HEIGHT);
	this->initData();
	this->initGame();
	this->initWindow();
	this->initFonts();

}
Game::~Game() {
	delete this->window;
	delete this->champ;
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
	if (this->isOver == true) {
		return ;
	}
	this->pollEvents();
	this->updateMousePos();
	this->champ->update();
	this->updateUiText();

	this->score++;
	this->spawnTime -= 1.f;
	if (this->spawnTime <= 0) {
		this->spawnTime = ENEMY_COOL;
		this->spawnEnemies();
	}

	// std::cout << "mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << "\n";
	// std::cout << "mouse pos: " << this->mouse.x << " " << this->mouse.y << ", spTime: " << this->spawnTime << "\n";
}
void Game::render() {
	/*
		clear old frame
		render objects
		display frame in window 
	*/
	if (this->isOver == true) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == true) {
			this->isOver = false;
		}
	} else {
		this->window->clear(); // sf::Color(0xff, 0, 0, 0xff)

		// Draw game Object
		this->champ->draw(*this->window);
		bool crushCount = false;
		for (int i=0; i < enemies.size(); i++) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (this->enemies[i].checkIn(this->mousePosView) == true) {
					this->enemies[i].setDie(true);
				}
			} else if (enemies[i].isWindowOut(this->window->getSize().y) == true) {
				this->enemies[i].setDie(true);
			}
			if (this->champ->isDie() == true) {
				this->gameOver();
				this->isOver = true;
				return;
			} else if (this->champ->checkIn(enemies[i].getShape()) == true) {
				this->champ->reduceHp(1);
				crushCount = true;
				this->champ->changeColor(sf::Color::Red);
			}
			// this->window->draw(enemies[i].getShape());
			enemies[i].draw(*this->window);
		}
		if (crushCount == false) {
			this->champ->changeColor(sf::Color::Cyan);
		}
		this->renderUiText(*this->window);

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
}
void Game::updateUiText() {
	std::stringstream ss;

	ss << "HP : " << this->champ->getHp() << "\nSCORE: " << this->score;

	this->uiText.setString(ss.str());
}
void Game::renderUiText(sf::RenderTarget &target) {
	target.draw(this->uiText);
}

void Game::gameOver() {
	std::stringstream ss;
	ss << "GAME OVER\nYour Score: " << this->score << "\n\nPress Space..";
	this->uiText.setString(ss.str());
	this->window->clear();
	this->window->draw(this->uiText);
	this->window->display();
	this->initGame();
}


void Game::spawnEnemies() {
	// static_cast<float>(rand() % static_cast<int>((this->window->getSize().y - this->enemyShape.getSize().y)))
	this->enemyShape.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemyShape.getSize().x + 10)),
		0.f
	);
	this->enemyShape.setFillColor(sf::Color::Red);
	this->enemyShape.setSize(sf::Vector2f(70.f, 70.f));
	this->enemyShape.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemyShape.setOutlineColor(sf::Color::Magenta);
	this->enemyShape.setOutlineThickness(5.f);
	this->enemy.setShape(this->enemyShape);
	this->enemy.setFallSpeed(rand() % 5 + 5);

	this->enemies.push_back(this->enemy);
}

void Game::enemiesMove() { // enemy class
	for (int i=0; i<this->enemies.size(); i++) {
		this->enemies[i].dropEnemy();
	}
}


const bool Game::isOpen() const {
	return this->window->isOpen();
}