#include "Champ.hpp"

Champ::Champ(): MaxHp(100), Hp(100) {
	this->shape.setPosition(
		100, 100
	);
	this->shape.setSize(sf::Vector2f(100.f, 100.f));
	this->shape.setScale(sf::Vector2f(0.5f, 0.5f));
	this->shape.setFillColor(sf::Color::Cyan);
	this->shape.setOutlineColor(sf::Color::Green);
	this->shape.setOutlineThickness(10.f);
}
Champ::~Champ() {}

void Champ::update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->shape.move(-3.f, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->shape.move(3.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->shape.move(0.f, -3.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->shape.move(0.f, 3.f);
}

sf::RectangleShape const &Champ::getShape() {
	return this->shape;
}
void Champ::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
}

void Champ::move(float x, float y) {
	this->shape.move(x, y);
}
void Champ::reduceHp(unsigned int deal) {
	this->Hp -= deal;
}
bool Champ::checkIn(sf::Shape const &sh) {
	if (this->shape.getGlobalBounds().intersects(sh.getGlobalBounds()) == true) {
		return true;
	} else {
		return false;
	}
}
