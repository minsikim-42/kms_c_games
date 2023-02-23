#include "Champ.hpp"

Champ::Champ(int wid, int hei, float sizeW, float sizeH):
	MaxHp(10), Hp(10), gameW(wid), gameH(hei), frontLimit(0), bulletCool(0)//(hei * 3) / 4)
{
	if (this->texture.loadFromFile("Textures/spaceship.png") == false) {
		std::cout << "Fail to load Champ texture\n";
		exit(1);
	}
	std::cout << "texture size: " << this->texture.getSize().x * 0.15f << ", " << this->texture.getSize().y * 0.15f << "\n";
	this->sprite.setTexture(this->texture);
	sf::FloatRect sp = this->sprite.getLocalBounds();
	this->sprite.setOrigin(sp.width / 2.f, (sp.height) * 2.f / 3.f);
	this->sprite.setScale(0.15f, 0.15f);
	this->originColor = this->sprite.getColor();
	this->sprite.setPosition(
		(wid) / 2.f, (hei) * 4.f / 5.f
		// (wid - sp.width * sprite.getScale().x + sizeW) / 2, ((hei - sp.height * sprite.getScale().y + sizeH) * 4) / 5
	);
	std::cout << "sprite po: " << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";
	std::cout << "sp size: " << sp.width << ", " << sp.height << "\n";

	this->shape.setSize(sf::Vector2f(sizeW, sizeH));
	this->shape.setOrigin((sf::Vector2f)shape.getSize() / 2.f);
	this->shape.setPosition(
		wid / 2.f, (hei * 4.f) / 5.f
	);
	std::cout << "shape po: " << shape.getPosition().x << ", " << shape.getPosition().y << "\n";
	std::cout << "shape po: " << shape.getSize().x << ", " << shape.getSize().y << "\n";
	this->shape.setFillColor(sf::Color::Cyan);
	this->shape.setOutlineColor(sf::Color::Green);
	this->shape.setOutlineThickness(10.f);
}
Champ::~Champ() {
	while (this->bullets.size() != 0) {
		delete this->bullets[0];
		this->bullets.erase(this->bullets.begin());
	}
	this->bullets.clear();
}

void Champ::update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->move(-3.f, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->move(3.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->move(0.f, -3.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->move(0.f, 3.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		this->fire();
	for (int i=0; i < this->bullets.size(); i++) {
		this->bullets[i]->moveBullet();
	}
	this->checkBulletOut();
	// std::cout << this->bullets.size() << "\n";

	this->bulletCool++;
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
const sf::Color &Champ::getOriginColor() {
	return this->originColor;
}
const std::vector<Bullet *> Champ::getBullets() {
	return this->bullets;
}

void Champ::setHp(unsigned int hp) {
	if (hp >= this->MaxHp) {
		this->Hp = this->MaxHp;
	} else {
		this->Hp = hp;
	}
}
void Champ::setXY(float x, float y) {
	this->sprite.setPosition(x, y);
	this->shape.setPosition(x, y);
}
void Champ::changeColor(sf::Color color) {
	this->shape.setFillColor(color);
	this->sprite.setColor(color);
}
void Champ::cleanBullet() {
	while (this->bullets.size() != 0) {
		delete this->bullets[0];
		this->bullets.erase(this->bullets.begin());
	}
	this->bullets.clear();
}

void Champ::draw(sf::RenderTarget &target) {
	target.draw(this->shape);
	target.draw(this->sprite);
	for (int i=0; i < this->bullets.size(); i++) {
			this->bullets[i]->draw(target);
	}
}

void Champ::move(float x, float y) {
	if (this->shape.getPosition().x + x + this->shape.getSize().x + 20 < this->gameW && // >
		this->shape.getPosition().x + x > 20 && // <
		this->shape.getPosition().y + y + this->shape.getSize().y < this->gameH && // v
		this->shape.getPosition().y + y > this->frontLimit) // ^
	{
		this->shape.move(x, y);
		this->sprite.move(x, y);
	}
}
void Champ::reduceHp(int deal) {
	if (this->Hp >= this->MaxHp + deal) {
		this->Hp = this->MaxHp;
	} else {
		this->Hp -= deal;
	}
}
const bool Champ::checkIn(sf::Shape const &sh) {
	if (this->shape.getGlobalBounds().intersects(sh.getGlobalBounds()) == true) {
		return true;
	} else {
		return false;
	}
}
const bool Champ::checkInBullet(sf::Shape const &sh) {
	for (int i=0; i < this->bullets.size(); i++) {
		if (this->bullets[i]->getShape().getGlobalBounds().intersects(sh.getGlobalBounds()) == true) {
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
			return true;
		}
	}
	return false;
}
void Champ::checkBulletOut() {
	for (int i=0; i < this->bullets.size(); i++) {
		if (this->bullets[i]->isWindowOut() == true) {
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
			i--;
		}
	}
}
void Champ::fire() {
	if (this->bulletCool >= COOLTIME) {
		this->bulletCool = 0;
		Bullet *b = new Bullet(this->shape.getPosition());
		bullets.push_back(b);
	}
}

void Champ::reset() {
	this->bulletCool = 0;
	this->Hp = MaxHp;
}