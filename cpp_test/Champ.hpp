#ifndef CHAMP_HPP
# define CHAMP_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>

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
	const bool isDie();

	void setHp(unsigned int hp);
	void changeColor(sf::Color color);

	void draw(sf::RenderTarget &target);

	void move(float x, float y);
	void reduceHp(unsigned int deal);
	const bool checkIn(sf::Shape const &shape);
};

#endif