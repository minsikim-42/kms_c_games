#ifndef ENEMY_HPP
# define ENEMY_HPP

#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

template <typename T>
class Enemy
{
private:
	float	spownTime;
	bool	die;
	int		fallSpeed;
	float	rotateSpeed;
	T		shape;
	sf::Texture	texture;
	sf::Sprite	sprite;

public:
	Enemy<T>(): die(false), fallSpeed(1), rotateSpeed(0) {
		this->shape.setPosition(
		0.f, 0.f
		);
		this->shape.setSize(sf::Vector2f(10.f, 10.f));
		this->shape.setFillColor(sf::Color::Cyan);
		this->shape.setOutlineColor(sf::Color::Red);
		this->shape.setOutlineThickness(5.f);

		if (this->texture.loadFromFile("Textures/meteor.png") == false) {
			std::cout << "Error::Enemy Cannot find Textures/meteor.png\n";
			exit(1);
		}
		this->sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		this->sprite.setTexture(this->texture);
		this->sprite.setScale(0.06f, 0.06f);
	}
	~Enemy<T>() {}
	T const &getShape() {
		return this->shape;
	}
	sf::Sprite const &getSprite() {
		return this->sprite;
	}
	bool const	&getDie() {
	return this->die;
	}

	void dropEnemy() {
		this->shape.move(0.f, this->fallSpeed);
		this->sprite.move(0.f, this->fallSpeed);

		this->sprite.rotate(this->rotateSpeed);
	}
	void setEnemy(float const &W, float const &H, float const &winX) { //
		this->sprite.setScale(0.06f, 0.06f);
		int R = rand();
		float x = static_cast<float>(R % static_cast<int>(winX - W + 10));
		float rate = (R % 10 + 10) / 15.f;

		this->shape.setPosition(x, 0.f);
		this->shape.setSize(sf::Vector2f(W * rate, H * rate));
		this->sprite.setScale((sf::Vector2f)sprite.getScale() * rate);
		this->shape.setFillColor(sf::Color::Red);

		this->fallSpeed = (R % 5 + 5);
		this->rotateSpeed = pow(this->fallSpeed, 1.5f) / 2.f;

		sf::FloatRect FR = this->sprite.getLocalBounds();
		this->sprite.setPosition(x + (W * rate) / 2, (H * rate) / 2);
	}
	void setDie(const bool &d) {
		this->die = d;
	}
	void setFallSpeed(int speed) {
		this->fallSpeed = speed;
	}
	void setRotateSpeed(int speed) {
		// this->rotateSpeed = speed;
		this->sprite.setRotation(speed);
	}

	const bool checkIn(sf::Vector2f &po) {
		if (this->shape.getGlobalBounds().contains(po) == true) {
			return true;
		}
		else
			return false;
	}
	const bool isWindowOut(unsigned int const &height) {
		if (this->shape.getPosition().y >= height) {
			return true;
		} else {
			return false;
		}
	}
	void draw(sf::RenderTarget &target) {
		// target.draw(this->shape);
		target.draw(this->sprite);
	}
};

#endif