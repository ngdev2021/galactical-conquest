#pragma once
#include <SFML/Graphics.hpp>
#include "Common.h"

enum class EnemyType {
    BASIC,
    BOSS,
    CHALLENGING
};

class Enemy {
public:
    Enemy(float x, float y, EnemyType type);

    void update();
    void setFormationPosition(sf::Vector2f pos) { formationPos = pos; }
    const sf::Sprite& getSprite() const { return sprite; }
    sf::Sprite& getSprite() { return sprite; }
    bool isAlive() const { return alive; }
    void kill() { alive = false; }
    bool isInFormation() const { return inFormation; }
    void enterFormation() { inFormation = true; }
    void startAttack();
    bool isAttacking() const { return attacking; }
    bool canCapture() const { return type == EnemyType::BOSS && !hasCapture; }
    void setCapturedShip(bool captured) { hasCapture = captured; }
    bool hasCapturedShip() const { return hasCapture; }
    EnemyType getType() const { return type; }
    int getPointValue() const;

    const sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

private:
    void updateAttackPattern();
    void moveToFormation();

    sf::Sprite sprite;
    bool alive;
    bool inFormation;
    bool attacking;
    bool hasCapture;
    EnemyType type;
    sf::Vector2f formationPos;
    sf::Vector2f velocity;
    float attackTime;
    float attackAngle;
};
