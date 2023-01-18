#ifndef ENEMY_HPP
# define ENEMY_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
private:
	float	spownTime;
	bool	die;
	sf::RectangleShape	shape;

public:
	Enemy();
	~Enemy();
	sf::RectangleShape const &getShape();
	bool const	&getDie();

	void dropEnemy(float const &_y);
	void setShape(sf::RectangleShape const &sh);
	void setDie(const bool &die);

	const bool checkIn(sf::Vector2f &po);
	void draw(sf::RenderTarget &target);
};

#endif