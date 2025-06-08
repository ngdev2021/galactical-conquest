#include "Player.h"
#include "TextureManager.h"
#include <iostream>

Player::Player()
    : sprite(TextureManager::getTexture("assets/player.png")),
      dualSprite(TextureManager::getTexture("assets/player.png")),
      health(3),
      shootCooldown(0.f),
      hasDualFighter(false),
      isCaptured(false),
      dualOffset(30.f) {
    
    // Set initial position
    sprite.setPosition(sf::Vector2f(400.f, 500.f));
    
    // Debug output
    std::cout << "Player initialized at position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
}

void Player::handleInput() {
    if (!isAlive() || isCaptured) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        sprite.move(sf::Vector2f(-5.f, 0.f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        sprite.move(sf::Vector2f(5.f, 0.f));
    }

    // Keep player within screen bounds
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u size = sprite.getTexture().getSize();
    sf::Vector2f scale = sprite.getScale();

    float maxX = hasDualFighter ? 800 - (size.x * scale.x) - dualOffset : 800 - (size.x * scale.x);

    if (pos.x < 0) {
        sprite.setPosition(sf::Vector2f(0.f, pos.y));
    } else if (pos.x + (size.x * scale.x) > maxX) {
        sprite.setPosition(sf::Vector2f(maxX, pos.y));
    }

    if (hasDualFighter) {
        updateDualFighter();
    }
}

void Player::update() {
    if (isCaptured) {
        // When captured, the ship moves up off screen
        sprite.move(sf::Vector2f(0.f, -2.f));
        if (sprite.getPosition().y < -50.f) {
            // Ship is fully captured
            health--;
            if (health <= 0) {
                // Game over if no lives left
                return;
            }
            // Reset position for next life
            sprite.setPosition(sf::Vector2f(400.f, 500.f));
            isCaptured = false;
        }
    }
}

void Player::updateDualFighter() {
    sf::Vector2f mainPos = sprite.getPosition();
    dualSprite.setPosition(sf::Vector2f(mainPos.x + dualOffset, mainPos.y));
}

void Player::damage(int amount) {
    if (!isCaptured) {
        if (hasDualFighter) {
            hasDualFighter = false; // Lose dual fighter first
        } else {
            health = std::max(0, health - amount);
        }
    }
}

void Player::rescue() {
    if (isCaptured) {
        isCaptured = false;
        hasDualFighter = true;
        sprite.setPosition(sf::Vector2f(400.f, 500.f));
        updateDualFighter();
    }
}
