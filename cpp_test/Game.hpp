#ifndef GAME_HPP
#define GAME_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Audio.hpp>
#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Network.hpp>
#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/System.hpp>
#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <sstream>
// #include <random>
#define ENEMY_COOL 6.f
#define FPS 60
#define HEIGHT 600
#define WIDTH 800

#include "Enemy.hpp"
class Enemy;
#include "Champ.hpp"
class Champ;


/*
	class have to act as the game engine
	wrapper class
*/

class Game
{
private:
	sf::RenderWindow		*window;
	sf::Event				ev;
	sf::VideoMode			videoMode;

	// mouse position
	sf::Vector2i			mouse;
	sf::Vector2f			mousePosView;

	// game object
	std::vector<Enemy>		enemies;
	Enemy					enemy;
	sf::RectangleShape		enemyShape;
	Champ					*champ;

	// game
	float					spawnTime;
	unsigned int			score;
	bool					isOver;
	bool					isCrush;

	// game resource
	sf::Font				font;

	sf::Text				uiText;

	void initWindow();
	void initData();
	void initFonts();
	void initGame();

public:
	Game();
	virtual ~Game();

	void pollEvents();
	void update();
	void updateMousePos();
	void updateUiText();
	void render();
	void renderUiText(sf::RenderTarget &target);
	void gameOver();

	void spawnEnemies();
	void enemiesMove(); // enemy class

	const bool isOpen() const;
};

#endif