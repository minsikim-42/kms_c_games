#ifndef ENEMY_HPP
# define ENEMY_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
private:
	float	spownTime;
	bool	die;
	int		fallSpeed;
	sf::RectangleShape	shape;

public:
	Enemy();
	~Enemy();
	sf::RectangleShape const &getShape();
	bool const	&getDie();

	void dropEnemy();
	void setShape(sf::RectangleShape const &sh);
	void setDie(const bool &die);
	void setFallSpeed(int speed);

	const bool checkIn(sf::Vector2f &po);
	const bool isWindowOut(unsigned int const &height);
	void draw(sf::RenderTarget &target);
};

#endif