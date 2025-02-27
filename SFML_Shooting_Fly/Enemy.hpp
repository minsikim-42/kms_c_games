#ifndef ENEMY_HPP
# define ENEMY_HPP

//#include </opt/homebrew/Cellar/sfml/2.5.1_2/include/SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>

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
	bool	type;
	int		Hp;

	sf::Texture	texture;
	sf::Texture	healTexture;
	sf::Sprite	sprite;
	sf::Sprite	healSprite;

public:
	Enemy<T>(): die(false), fallSpeed(1), rotateSpeed(0), type(true), Hp(10) {
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

		if (this->healTexture.loadFromFile("Textures/wrench.png") == false) {
			std::cout << "Error::Enemy Cannot find Textures/wrench.png\n";
			exit(1);
		}
		this->healSprite.setOrigin((sf::Vector2f)healTexture.getSize() / 2.f);
		this->healSprite.setTexture(this->healTexture);
		this->healSprite.setScale(0.06f, 0.06f);
	}
	~Enemy<T>() {}
	T const &getShape() {
		return this->shape;
	}
	sf::Sprite const &getSprite() {
		return this->sprite;
	}
	bool const	&isDie() {
		return this->die;
	}
	bool const &getType() {
		return this->type;
	}
	const int &getHp() {
		return this->Hp;
	}

	void dropEnemy() {
		this->shape.move(0.f, this->fallSpeed);

		if (this->type == true) {
			this->sprite.move(0.f, this->fallSpeed);
			this->sprite.rotate(this->rotateSpeed);
		} else {
			this->healSprite.move(0.f, this->fallSpeed);
			this->healSprite.rotate(this->rotateSpeed);
		}
	}
	void setEnemy(float const &W, float const &H, float const &winX) { //

		int R = rand();
		float x = static_cast<float>(R % static_cast<int>(winX - W + 10));
		float rate = (R % 10 + 10) / 15.f;
		
		if (R % 99 == 0) { // big Meteor
			this->type = true;
			this->Hp = 50;
			this->sprite.setScale(0.05f, 0.05f);
			sf::FloatRect FR = this->sprite.getLocalBounds();

			this->shape.setPosition(x, -H * 4);
			this->shape.setSize(sf::Vector2f(W * 4, H * 4));
			this->shape.setFillColor(sf::Color::Red);

			this->fallSpeed = (1);
			this->rotateSpeed = pow(this->fallSpeed, 1.5f) / 3.f;
			this->sprite.setScale((sf::Vector2f)sprite.getScale() * 5.f);
			this->sprite.setPosition(x + (W * 4) / 2, -(H * 4) / 2);
		} else if (R % 20 != 0) { // Meteor
			this->Hp = 10;
			this->type = true;
			this->sprite.setScale(0.06f, 0.06f);
			sf::FloatRect FR = this->sprite.getLocalBounds();

			this->shape.setPosition(x, -H * rate);
			this->shape.setSize(sf::Vector2f(W * rate, H * rate));
			this->shape.setFillColor(sf::Color::Red);

			this->fallSpeed = (R % 5 + 5);
			this->rotateSpeed = pow(this->fallSpeed - 3, 2.f) / 5.f;
			this->sprite.setScale((sf::Vector2f)sprite.getScale() * rate);
			this->sprite.setPosition(x + (W * rate) / 2, -(H * rate) / 2);
		} else { // heal
			this->Hp = 10;
			this->type = false;
			this->healSprite.setScale(0.06f, 0.06f);
			sf::FloatRect FR = this->healSprite.getLocalBounds();

			this->shape.setPosition(x, 0.f);
			this->shape.setSize(sf::Vector2f(W * rate, H * rate));
			this->shape.setFillColor(sf::Color::Red);

			this->fallSpeed = (R % 5 + 5);
			this->rotateSpeed = pow(this->fallSpeed - 3, 2.f) / 6.f;
			this->healSprite.setScale((sf::Vector2f)healSprite.getScale() * rate);
			this->healSprite.setPosition(x + (W * rate) / 2, (H * rate) / 2);
		}

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
		this->healSprite.setRotation(speed);
	}
	void reduceHp(int deal) {
		this->Hp -= deal;
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
		if (type == true) {
			target.draw(this->sprite);
		} else {
			target.draw(this->healSprite);
		}
	}
};

#endif
