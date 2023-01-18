#include "Enemy.hpp"

Enemy::Enemy(): die(false) {
	std::cout << "ememy spawned!!\n";
	this->shape.setPosition(
		0.f, 0.f
	);
	this->shape.setSize(sf::Vector2f(10.f, 10.f));
	this->shape.setScale(sf::Vector2f(0.5f, 0.5f));
	this->shape.setFillColor(sf::Color::Cyan);
	this->shape.setOutlineColor(sf::Color::Red);
	this->shape.setOutlineThickness(5.f);
}
Enemy::~Enemy() {
	std::cout << "removed enemy\n";
}
sf::RectangleShape const &Enemy::getShape() {
	return this->shape;
}
const bool &Enemy::getDie() {
	return this->die;
}

void Enemy::dropEnemy(float const &_y) {
	sf::Vector2f po = this->shape.getPosition();
	this->shape.setPosition(po.x, po.y + _y);
}
void Enemy::setShape(sf::RectangleShape const &sh) { //
	this->shape = sh;
}
void Enemy::setDie(const bool &d) {
	this->die = d;
}

const bool Enemy::checkIn(sf::Vector2f &po) {
	if (this->shape.getGlobalBounds().contains(po) == true) {
		return true;
	}
	else
		return false;
}
void Enemy::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
}