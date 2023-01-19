#include "Enemy.hpp"

Enemy::Enemy(): die(false), fallSpeed(1) {
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

void Enemy::dropEnemy() {
	this->shape.move(0.f, this->fallSpeed);
}
void Enemy::setShape(sf::RectangleShape const &sh) { //
	this->shape = sh;
}
void Enemy::setDie(const bool &d) {
	this->die = d;
}
void Enemy::setFallSpeed(int speed) {
	this->fallSpeed = speed;
}

const bool Enemy::checkIn(sf::Vector2f &po) {
	if (this->shape.getGlobalBounds().contains(po) == true) {
		return true;
	}
	else
		return false;
}
const bool Enemy::isWindowOut(unsigned int const &height) {
	if (this->shape.getPosition().y >= height) {
		return true;
	} else {
		return false;
	}
}

void Enemy::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
}