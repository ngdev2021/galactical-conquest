#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Menu.h"
#include "Settings.h"
#include <vector>

enum class GameState {
    MENU,
    PLAYING,
    CHALLENGING,
    GAME_OVER
};

class Game {
public:
    Game();
    void run();
    
    // Methods for macOS menu integration
    void newGame();
    void showPreferences();
    void showAboutPanel();

private:
    void handleEvents();
    void update();
    void render();
    void spawnFormation();
    void spawnChallengingStage();
    void updateFormation();
    void updateChallenging();
    void applySettings();
    void updateScore(int points);
    void renderHUD();
    void checkCollisions();
    void startNewStage();
    void handleCapture(Enemy& boss);

    sf::RenderWindow window;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    float bgY;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    sf::Clock stageClock;
    sf::Clock attackClock;
    sf::Clock gameTimer;

    Menu menu;
    Settings settings;
    GameState gameState;
    int stage = 1;
    int score = 0;
    int highScore = 0;
    int enemiesInFormation = 0;
    float formationWidth = 400.f;
    float formationHeight = 200.f;
    float formationOffsetX = 200.f;
    float formationOffsetY = 100.f;
    
    sf::Font gameFont;
    sf::Text scoreText;
    sf::Text healthText;
    sf::Text gameOverText;
    sf::Text stageText;

    struct FormationSlot {
        sf::Vector2f position;
        bool occupied;
        EnemyType type;
    };
    std::vector<FormationSlot> formationSlots;
};
