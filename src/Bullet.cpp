#include "Bullet.h"

Bullet::Bullet(float x, float y)
    : sprite(TextureManager::getEmptyTexture()) {
    if (!texture.loadFromFile("assets/bullet.png")) {
        throw std::runtime_error("Failed to load bullet texture!");
    }
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
}

void Bullet::update() {
    sprite.move(sf::Vector2f(0.f, -10.f));
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}
