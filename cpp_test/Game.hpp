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
// #include <random>

#include "Enemy.hpp"
class Enemy;
#include "Champ.hpp"
class Champ;

#define ENEMY_COOL 10.f
#define FPS 60

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
	float					spawnTime;
	Champ					champ;

	// game resource
	sf::Font				font;

	void initWindow();
	void initData();
	void initFonts();

public:
	Game();
	virtual ~Game();

	void pollEvents();
	void update();
	void updateMousePos();
	void render();

	void spawnEnemies();
	void enemiesMove(); // enemy class

	const bool isOpen() const;
};

#endif