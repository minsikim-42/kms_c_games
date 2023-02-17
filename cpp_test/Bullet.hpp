#ifndef BULLET_HPP
#define BULLET_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>

#define BULLETSPEED 10.f

class Bullet
{
private:
	sf::RectangleShape	shape;
	int					Hp;
	unsigned			sizeW;
	unsigned			sizeH;

	sf::Texture			texture;
	sf::Sprite			sprite;
	sf::Color			originColor;

	Bullet();

	//

public:
	Bullet(const sf::Vector2f &po, float sizeW = 12.f, float sizeH = 40.f);
	~Bullet();
	void update();

	sf::RectangleShape const &getShape();
	const int getHp();
	const sf::Color &getOriginColor();
	const bool isDie();
	const bool isWindowOut();

	void setHp(unsigned int hp);
	void changeColor(sf::Color color);


	void draw(sf::RenderTarget &target);
	void moveBullet();

	void reduceHp(unsigned int deal);
	const bool checkIn(sf::Vector2f &po);
};

#endif