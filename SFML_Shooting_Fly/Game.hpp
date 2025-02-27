#ifndef GAME_HPP
#define GAME_HPP

//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Audio.hpp>
//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Network.hpp>
//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/System.hpp>
//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <sstream>
// #include <random>
#define ENEMY_COOL 8.f
#define FPS 60
#define WIDTH 1280
#define HEIGHT 1024

#include "Enemy.hpp"
// class Enemy;
#include "Champ.hpp"
class Champ;

enum ENUMTYPE
{
	DEFAULT = 0,
	TYPE1 = 1,
	TYPE2 = 2
};

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
	std::vector<Enemy<sf::RectangleShape> >		enemies;
	Enemy<sf::RectangleShape>					enemy;
	// Enemy<sf::ConvexShape>					CONVEXenemy; // error
	// std::vector<Enemy<sf::CircleShape> >		SIRCLEenemies;
	// Enemy<sf::CircleShape>					SIRCLEenemy;

	sf::RectangleShape		enemyShape;

	Champ					*champ;

	// game
	float					spawnTime;
	unsigned int			score;
	unsigned int			highScore;
	bool					isOver;
	bool					isCrush;
	bool					isHome;
	uint8_t					homeNum;

	// game resource
	sf::Font				font;
	sf::Text				uiText;
	sf::Texture				texture;
	sf::Sprite				space;

	void initWindow();
	void initData();
	void initFonts();
	void initSpace();
	void initGame();

public:
	Game();
	virtual ~Game();

	void pollEvents();
	void update();
	void updateMousePos();
	void updateUiText();
	void updateSpace();
	void render();
	void renderUiText(sf::RenderTarget &target);
	void renderHome();
	void gameOver();

	void spawnEnemies();
	void enemiesMove(); // enemy class

	const bool isOpen() const;
};

#endif
