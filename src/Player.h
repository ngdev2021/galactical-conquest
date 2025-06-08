#pragma once
#include <SFML/Graphics.hpp>
#include "Common.h"

class Player {
public:
    Player();
    void handleInput();
    void update();
    sf::Sprite& getSprite() { return sprite; }
    sf::Sprite* getDualSprite() { return hasDualFighter ? &dualSprite : nullptr; }
    bool canShoot() const { return shootClock.getElapsedTime().asSeconds() >= shootCooldown; }
    void resetShootClock() { shootClock.restart(); }
    int getHealth() const { return health; }
    void damage(int amount);
    bool isAlive() const { return health > 0; }
    void capture() { isCaptured = true; }
    bool isCaptureState() const { return isCaptured; }
    void rescue();
    bool hasDual() const { return hasDualFighter; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getDualPosition() const { return dualSprite.getPosition(); }

private:
    void updateDualFighter();

    sf::Sprite sprite;
    sf::Sprite dualSprite;
    sf::Clock shootClock;
    float shootCooldown = 0.25f; // Time between shots in seconds
    int health = 3; // Player starts with 3 health points
    bool isCaptured = false;
    bool hasDualFighter = false;
    float dualOffset = 30.f; // Offset between main and dual fighter
};
