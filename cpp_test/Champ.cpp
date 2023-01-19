#include "Champ.hpp"

Champ::Champ(int wid, int hei, float sizeW, float sizeH):
	MaxHp(100), Hp(100), gameW(wid), gameH(hei), frontLimit((hei * 3) / 4)
{
	this->shape.setPosition(
		wid / 2, (hei * 4) / 5
	);
	this->shape.setSize(sf::Vector2f(sizeW, sizeH));
	// this->shape.setScale(sf::Vector2f(0.5f, 0.5f)); // size 1/2 ** attention to move
	this->shape.setFillColor(sf::Color::Cyan);
	this->shape.setOutlineColor(sf::Color::Green);
	this->shape.setOutlineThickness(10.f);
}
Champ::~Champ() {}

void Champ::update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->move(-3.f, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->move(3.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->move(0.f, -3.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->move(0.f, 3.f);
}

sf::RectangleShape const &Champ::getShape() {
	return this->shape;
}
const int Champ::getHp() {
	return this->Hp;
}
const bool Champ::isDie() {
	if (this->Hp <= 0) {
		return true;
	} else {
		return false;
	}
}

void Champ::setHp(unsigned int hp) {
	this->Hp = hp;
}
void Champ::changeColor(sf::Color color) {
	this->shape.setFillColor(color);
}
void Champ::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
}

void Champ::move(float x, float y) {
	if (this->shape.getPosition().x + x + this->shape.getSize().x + 20 < this->gameW && // >
		this->shape.getPosition().x + x > 20 && // <
		this->shape.getPosition().y + y + this->shape.getSize().y < this->gameH && // v
		this->shape.getPosition().y + y > this->frontLimit) // ^
	{
		this->shape.move(x, y);
	}
}
void Champ::reduceHp(unsigned int deal) {
	this->Hp -= deal;
}
const bool Champ::checkIn(sf::Shape const &sh) {
	if (this->shape.getGlobalBounds().intersects(sh.getGlobalBounds()) == true) {
		return true;
	} else {
		return false;
	}
}
