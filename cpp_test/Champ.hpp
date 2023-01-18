#ifndef CHAMP_HPP
# define CHAMP_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>

class Champ
{
private:
	sf::RectangleShape	shape;
	unsigned int		MaxHp;
	unsigned int		Hp;

public:
	Champ();
	~Champ();
	void update();

	sf::RectangleShape const &getShape();
	void draw(sf::RenderTarget &target);

	void move(float x, float y);
	void reduceHp(unsigned int deal);
	bool checkIn(sf::Shape const &shape);
};

#endif