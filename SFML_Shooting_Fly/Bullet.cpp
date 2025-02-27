#include "Bullet.hpp"

Bullet::Bullet(const sf::Vector2f &po, float sizeW, float sizeH):
	Hp(1)
{
	if (this->texture.loadFromFile("Textures/spaceship.png") == false) {
		std::cout << "Fail to load Bullet texture\n";
		exit(1);
	}
	this->sprite.setTexture(this->texture);
	this->originColor = this->sprite.getColor();
	this->sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
	this->sprite.setScale(0.05, 0.05);
	sf::FloatRect sp = this->sprite.getLocalBounds();
	this->sprite.setPosition(
		po.x, po.y - 20
		// po.x + (sizeW) / 2.f, po.y + (sizeH) / 2.f
		// po.x - (sp.width * sprite.getScale().x + sizeW) / 2, po.y - ((sp.height * sprite.getScale().y + sizeH) * 4) / 5
	);

	this->shape.setSize(sf::Vector2f(sizeW, sizeH));
	this->shape.setOrigin((sf::Vector2f)shape.getSize() / 2.f);
	this->shape.setPosition(
		po.x, po.y - 20
	);
	std::cout << shape.getSize().x << ", " << shape.getSize().y << "\n";
	this->shape.setFillColor(sf::Color::Red);
	this->shape.setOutlineColor(sf::Color::Yellow);
	this->shape.setOutlineThickness(1.f);
}
Bullet::~Bullet() {}

void Bullet::update() {
	this->moveBullet();
}

sf::RectangleShape const &Bullet::getShape() {
	return this->shape;
}
const int Bullet::getHp() {
	return this->Hp;
}
const bool Bullet::isDie() {
	if (this->Hp <= 0) {
		return true;
	} else {
		return false;
	}
}
const bool Bullet::isWindowOut() {
		if (this->shape.getPosition().y <= 0) {
			return true;
		} else {
			return false;
		}
	}
const sf::Color &Bullet::getOriginColor() {
	return this->originColor;
}

void Bullet::setHp(unsigned int hp) {
		this->Hp = hp;
}
void Bullet::changeColor(sf::Color color) {
	this->shape.setFillColor(color);
	this->sprite.setColor(color);
}

void Bullet::reduceHp(unsigned int deal) {
	this->Hp -= deal;
}

void Bullet::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
	// target.draw(this->sprite);
}
void Bullet::moveBullet() {
	this->shape.move(0.f, -BULLETSPEED);
	this->sprite.move(0.f, -BULLETSPEED);
}

const bool Bullet::checkIn(sf::Vector2f &po) {
	if (this->shape.getGlobalBounds().contains(po) == true) {
		return true;
	}
	else
		return false;
}