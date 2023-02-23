#ifndef CHAMP_HPP
# define CHAMP_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>

#include "Bullet.hpp"

#define COOLTIME 10

class Champ
{
private:
	sf::RectangleShape	shape;
	unsigned int		MaxHp;
	int					Hp;
	unsigned			sizeW;
	unsigned			sizeH;
	unsigned			gameW;
	unsigned			gameH;
	float				frontLimit;
	std::vector<Bullet *> bullets;
	unsigned			bulletCool;

	sf::Texture			texture;
	sf::Sprite			sprite;
	sf::Color			originColor;

	Champ();

	//

public:
	Champ(int wid, int hei, float sizeW = 24.f, float sizeH = 24.f);
	~Champ();
	void update();

	sf::RectangleShape const &getShape();
	const int getHp();
	const sf::Color &getOriginColor();
	const std::vector<Bullet *> getBullets(); // 이러면 private 쓰는 의미가 있을까?
	const bool isDie();

	void setHp(unsigned int hp);
	void setXY(float x, float y);
	void changeColor(sf::Color color);
	void cleanBullet();

	void draw(sf::RenderTarget &target);

	void move(float x, float y);
	void reduceHp(int deal);
	const bool checkIn(sf::Shape const &shape);
	const bool checkInBullet(sf::Shape const &shape);
	void checkBulletOut();
	void fire();
	void reset();
};

#endif