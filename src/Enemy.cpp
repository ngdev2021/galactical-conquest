#include "Enemy.h"
#include "TextureManager.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(float x, float y, EnemyType type)
    : sprite(TextureManager::getEmptyTexture()),
      alive(true),
      inFormation(false),
      attacking(false),
      hasCapture(false),
      type(type),
      formationPos(x, y),
      velocity(0.f, 0.f),
      attackTime(0.f),
      attackAngle(0.f) {
    
    std::string texturePath;
    switch(type) {
        case EnemyType::BOSS:
            texturePath = "assets/boss_enemy.png";
            break;
        case EnemyType::CHALLENGING:
            texturePath = "assets/challenge_enemy.png";
            break;
        default:
            texturePath = "assets/enemy.png";
            break;
    }
    
    // Use the TextureManager to load the texture
    sprite.setTexture(TextureManager::getTexture(texturePath));
    sprite.setPosition(sf::Vector2f(x, -50.f)); // Start above screen
    
    // Debug output
    std::cout << "Enemy created at position: " << x << ", " << -50.f << 
              " with texture: " << texturePath << std::endl;
}

void Enemy::update() {
    if (!inFormation && !attacking) {
        moveToFormation();
    } else if (attacking) {
        updateAttackPattern();
    }
}

void Enemy::moveToFormation() {
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f direction = formationPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 2.0f) {
        sprite.setPosition(formationPos);
        inFormation = true;
        return;
    }
    
    direction /= distance; // Normalize
    sprite.move(direction * 3.0f);
}

void Enemy::startAttack() {
    if (inFormation) {
        attacking = true;
        inFormation = false;
        attackTime = 0.f;
        attackAngle = 0.f;
        velocity = sf::Vector2f(0.f, 0.f);
    }
}

void Enemy::updateAttackPattern() {
    const float PI = 3.14159f;
    attackTime += 0.016f; // Assuming 60 FPS
    
    if (type == EnemyType::BOSS && hasCapture) {
        // Boss with captured ship moves in a figure-8 pattern
        float scale = 100.f;
        attackAngle += 0.02f;
        velocity.x = scale * std::sin(2 * attackAngle);
        velocity.y = scale * std::sin(attackAngle);
    } else {
        // Regular enemies swoop down in a curved pattern
        velocity.x = 200.f * std::sin(attackTime);
        velocity.y = 150.f * std::cos(attackTime * 0.5f);
    }
    
    sprite.move(velocity * 0.016f);
    
    // Check if enemy is off screen
    sf::Vector2f pos = sprite.getPosition();
    if (pos.y > 600.f || pos.y < -50.f || pos.x < -50.f || pos.x > 850.f) {
        if (hasCapture) {
            // Boss with capture returns to formation
            attacking = false;
        } else {
            // Other enemies are destroyed
            alive = false;
        }
    }
}

int Enemy::getPointValue() const {
    switch(type) {
        case EnemyType::BOSS:
            return hasCapture ? 1000 : 500;
        case EnemyType::CHALLENGING:
            return 300;
        default:
            return 100;
    }
}
