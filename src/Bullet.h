#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Bullet {
public:
    Bullet(float x, float y);
    void update();
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const { return sprite; }
    sf::FloatRect getBounds() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
};
