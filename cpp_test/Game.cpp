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
		exit(1);
	} else {
		this->uiText.setFont(this->font);
		this->uiText.setCharacterSize(35);
		this->uiText.setFillColor(sf::Color::White);
		this->uiText.setString("NONE");
	}
}
void Game::initSpace() {
	if (this->texture.loadFromFile("Textures/Space.png") == false) {
		std::cout << "Error::fail to get space\n";
		exit(1);
	} else {
		this->space.setTexture(this->texture);
		this->space.setScale(1.5f, 1.5f);
		// this->space.setRotation(90.f);
		sf::FloatRect FR = this->space.getLocalBounds();
		// this->space.setPosition(WIDTH / 2, HEIGHT - FR.height * this->space.getScale().y);
		this->space.setPosition(
			(WIDTH - FR.width * this->space.getScale().x) / 2,
			-FR.height * this->space.getScale().y / 2
		);
	}
}

// public
Game::Game(): isHome(true) {
	this->champ = new Champ(WIDTH, HEIGHT);
	this->initData();
	this->initGame();
	this->initWindow();
	this->initFonts();
	this->initSpace();
}
Game::~Game() {
	delete this->window;
	delete this->champ;
}


void Game::pollEvents() {
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			system("leaks a.out");
			break;
		
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			else if (this->ev.key.code == sf::Keyboard::Z)
				this->champ->fire();
			break;

		default:
			break;
		}
	}
}

void Game::updateMousePos() {
	this->mouse = sf::Mouse::getPosition(*this->window);

	this->mousePosView = this->window->mapPixelToCoords(this->mouse);
}
void Game::update() {
	if (this->isOver == true) {
		return ;
	}
	this->pollEvents();
	this->updateMousePos();
	this->champ->update();
	this->updateUiText();
	this->updateSpace();

	if (this->isHome == false) {
		this->score++;
		this->spawnTime -= 1.f;
		if (this->spawnTime <= 0) {
			this->spawnTime = ENEMY_COOL;
			this->spawnEnemies();
		}
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
	if (this->isHome == true) {
		renderHome();
	} else if (this->isOver == true) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == true) {
			this->isOver = false;
		}
	} else {
		this->window->clear(); // sf::Color(0xff, 0, 0, 0xff)

		// Draw game Object
		this->window->draw(this->space);
		this->champ->draw(*this->window);
		bool crushCount = false;
		for (int i=0; i < enemies.size(); i++) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (this->enemies[i].checkIn(this->mousePosView) == true) {
					this->enemies[i].setDie(true);
				}
			} else if (enemies[i].isWindowOut(this->window->getSize().y) == true ||
						enemies[i].getHp() <= 0) {
				this->enemies[i].setDie(true);
			}
			if (this->champ->isDie() == true) {
				this->gameOver();
				this->isOver = true;
				return;
			} else if (this->champ->checkIn(enemies[i].getShape()) == true) {
				if (enemies[i].getType() == true) {
					this->champ->reduceHp(1);
					crushCount = true;
					this->champ->changeColor(sf::Color::Red);
				} else {
					enemies[i].setDie(true);
					this->champ->reduceHp(-5);
				}
			}
			if (this->champ->checkInBullet(enemies[i].getShape()) == true) {
				// removed bullet
				this->enemies[i].reduceHp(2);
			}
			// this->window->draw(enemies[i].getShape());
			enemies[i].draw(*this->window);
		}
		if (crushCount == false) {
			this->champ->changeColor(this->champ->getOriginColor());
		}
		
		this->renderUiText(*this->window);

		this->window->display();


		this->enemiesMove();

		std::vector<Enemy<sf::RectangleShape> >::iterator it = enemies.begin();
		for (; it != enemies.end(); it++) {
			if (it->isDie() == true) {
				enemies.erase(it);
				break ;
			}
		}
	}
}
void Game::renderHome() {
	std::stringstream ss;

	ss << "START\nSOMETHING";

	this->uiText.setPosition(0.f, 0.f);
	this->uiText.setString(ss.str());
	sf::FloatRect bounds = this->uiText.getLocalBounds();
	this->uiText.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 2);
	this->window->clear();
	this->window->draw(this->uiText);
	this->window->display();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->isHome = false;
	}
}
void Game::updateUiText() {
	std::stringstream ss;

	ss << "HP : " << this->champ->getHp() << "\nSCORE: " << this->score;

	this->uiText.setPosition(0.f, 0.f);
	this->uiText.setString(ss.str());
}
void Game::updateSpace() {
	sf::FloatRect FR = this->space.getLocalBounds();
	if (this->space.getPosition().y >= 0) {
		sf::FloatRect FR = this->space.getLocalBounds();
		this->space.setPosition(
			(WIDTH - FR.width * this->space.getScale().x) / 2,
			-FR.height * this->space.getScale().y / 2
		);
	}
	this->space.move(0.f, 0.8f);
}
void Game::renderUiText(sf::RenderTarget &target) {
	target.draw(this->uiText);
}

void Game::gameOver() {
	std::stringstream ss;
	ss << "GAME OVER\nYour Score: " << this->score << "\n\nPress Space..";
	this->uiText.setString(ss.str());
	sf::FloatRect bounds = this->uiText.getLocalBounds();
	this->uiText.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 2);
	this->window->clear();
	this->window->draw(this->uiText);
	this->window->display();
	this->initGame();
}


void Game::spawnEnemies() {
	// static_cast<float>(rand() % static_cast<int>((this->window->getSize().y - this->enemyShape.getSize().y)))
	int R = rand();
	this->enemy.setEnemy(35.f, 35.f, this->window->getSize().x);

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