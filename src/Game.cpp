#include "Game.h"
#include "TextureManager.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({800, 600}), "Galactical Conquest"),
      bgY(0.f),
      player(),
      menu(),
      gameState(GameState::MENU),
      stage(1),
      score(0),
      highScore(0),
      // Initialize sprite with empty texture
      bgSprite(TextureManager::getEmptyTexture()),
      // Initialize text objects with font in initializer list
      scoreText(TextureManager::getFont(), "Score: ", 24),
      healthText(TextureManager::getFont(), "Health: ", 24),
      gameOverText(TextureManager::getFont(), "Game Over! Press Enter to continue", 40),
      stageText(TextureManager::getFont(), "", 32) {
    
    // Load background texture using TextureManager
    bgTexture = TextureManager::getTexture("assets/background.png");
    
    // Initialize sprite with texture
    bgSprite = sf::Sprite(bgTexture);
    bgSprite.setPosition(sf::Vector2f(0.f, -600.f));
    
    // Create a second background sprite for scrolling
    sf::Sprite bgSprite2(bgTexture);
    bgSprite2.setPosition(sf::Vector2f(0.f, 0.f));
    
    // Load font and setup text elements
    gameFont = TextureManager::getFont();
    
    // Setup HUD text positions and colors
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));
    scoreText.setFillColor(sf::Color::White);
    
    healthText.setPosition(sf::Vector2f(10.f, 40.f));
    healthText.setFillColor(sf::Color::White);
    
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(sf::Vector2f(400.f - gameOverText.getLocalBounds().size.x / 2.f, 250.f));
    
    stageText.setFillColor(sf::Color::White);
    
    std::cout << "Game constructor completed successfully" << std::endl;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::newGame() {
    // Reset game state
    gameState = GameState::PLAYING;
    stage = 1;
    score = 0;
    
    // Clear existing enemies and bullets
    enemies.clear();
    bullets.clear();
    
    // Reset player
    player = Player();
    
    // Start a new stage
    startNewStage();
    
    std::cout << "New game started!" << std::endl;
}

void Game::showPreferences() {
    // Show the settings dialog
    settings.show(window);
}

void Game::showAboutPanel() {
    // Show about information using MacOSBridge
    #ifdef __APPLE__
    #include "MacOSBridge.h"
    MacOSBridge::showAboutPanel(
        "Galactical Conquest",
        "Version 1.0",
        " 2025 Galactical Games",
        "A Galaga-style space shooter game created with SFML 3.0"
    );
    #endif
}

void Game::handleEvents() {
    if (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }
        
        // Handle settings dialog events first if it's visible
        if (settings.isVisible()) {
            if (settings.handleInput(*event)) {
                // Event was handled by settings dialog
                return;
            }
        }
        
        if (gameState == GameState::MENU) {
            menu.handleInput(*event);
            
            if (menu.getState() == Menu::State::Game) {
                gameState = GameState::PLAYING;
                startNewStage();
            }
            else if (menu.getState() == Menu::State::Exit) {
                window.close();
            }
        }
        else if (gameState == GameState::PLAYING || gameState == GameState::CHALLENGING) {
            // Handle player input via keyboard state instead of event
            // The Player class doesn't take an event parameter
            player.handleInput();
        }
        else if (gameState == GameState::GAME_OVER) {
            if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->scancode == sf::Keyboard::Scancode::Enter) {
                    gameState = GameState::MENU;
                    // Reset menu to main menu
                    menu = Menu();
                }
            }
        }
    }
}

void Game::update() {
    switch (gameState) {
        case GameState::MENU:
            menu.update();
            return;

        case GameState::GAME_OVER:
            return;

        case GameState::PLAYING:
            if (!player.isAlive()) {
                std::cout << "Player died! Transitioning to GAME_OVER state" << std::endl;
                gameState = GameState::GAME_OVER;
                return;
            }
            updateFormation();
            break;

        case GameState::CHALLENGING:
            if (!player.isAlive()) {
                std::cout << "Player died! Transitioning to GAME_OVER state" << std::endl;
                gameState = GameState::GAME_OVER;
                return;
            }
            updateChallenging();
            break;
    }

    // Common updates for both PLAYING and CHALLENGING states
    player.update();

    // Handle input and shooting
    player.handleInput();
    if (player.canShoot() && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        // Main ship bullet
        sf::Vector2f playerPos = player.getSprite().getPosition();
        sf::Vector2u playerSize = player.getSprite().getTexture().getSize();
        bullets.emplace_back(playerPos.x + playerSize.x/2, playerPos.y);

        // Dual fighter bullet
        if (player.hasDual()) {
            sf::Vector2f dualPos = player.getDualPosition();
            bullets.emplace_back(dualPos.x + playerSize.x/2, dualPos.y);
        }
        player.resetShootClock();
    }

    // Update bullets and remove off-screen ones
    for (auto& bullet : bullets) bullet.update();
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.getSprite().getPosition().y < -10.f; }),
        bullets.end());

    // Update enemies
    for (auto& enemy : enemies) enemy.update();

    // Start random attacks in formation mode
    if (gameState == GameState::PLAYING && 
        attackClock.getElapsedTime().asSeconds() > 1.0f && 
        !enemies.empty()) {
        int randomIndex = rand() % enemies.size();
        if (enemies[randomIndex].isInFormation()) {
            enemies[randomIndex].startAttack();
            attackClock.restart();
        }
    }

    checkCollisions();

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](Enemy& e) { return !e.isAlive(); }),
        enemies.end()
    );

    // Scroll background
    bgY += 1.f;
    if (bgY >= 600.f) bgY = 0.f;
    bgSprite.setPosition(sf::Vector2f(0.f, bgY - 600.f));
}

void Game::render() {
    window.clear();
    
    // Draw background
    window.draw(bgSprite);
    
    if (gameState == GameState::MENU) {
        menu.render(window);
    }
    else {
        // Draw player
        window.draw(player.getSprite());
        if (player.hasDual()) {
            window.draw(*player.getDualSprite());
        }
        
        // Draw enemies
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                window.draw(enemy.getSprite());
            }
        }
        
        // Draw bullets
        for (const auto& bullet : bullets) {
            bullet.draw(window);
        }
        
        // Draw HUD
        renderHUD();
        
        if (gameState == GameState::GAME_OVER) {
            window.draw(gameOverText);
        }
    }
    
    // Draw settings dialog on top if visible
    if (settings.isVisible()) {
        settings.render(window);
    }
    
    window.display();
}

void Game::spawnFormation() {
    formationSlots.clear();
    enemies.clear();
    enemiesInFormation = 0;

    // Create formation grid
    const int rows = 5;
    const int cols = 8;
    const float spacing = 50.f;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            FormationSlot slot;
            slot.position = sf::Vector2f(
                formationOffsetX + col * spacing,
                formationOffsetY + row * spacing
            );
            slot.occupied = true;

            // Assign enemy types
            if (row == 0) {
                slot.type = EnemyType::BOSS; // Top row is bosses
            } else if (row < 3) {
                slot.type = EnemyType::CHALLENGING; // Middle rows are challenging
            } else {
                slot.type = EnemyType::BASIC; // Bottom rows are basic
            }

            formationSlots.push_back(slot);

            // Create enemy
            enemies.emplace_back(
                -50.f + (rand() % 100), // Random start position off-screen
                -50.f,
                slot.type
            );
            enemies.back().setFormationPosition(slot.position);
        }
    }
}

void Game::spawnChallengingStage() {
    enemies.clear();
    enemiesInFormation = 0;

    // Create rows of challenging enemies that move in patterns
    const int challengingEnemies = 20;
    for (int i = 0; i < challengingEnemies; i++) {
        enemies.emplace_back(
            static_cast<float>(i * 40),
            -50.f,
            EnemyType::CHALLENGING
        );
    }

    gameState = GameState::CHALLENGING;
    stageClock.restart();
}

void Game::updateFormation() {
    // Count enemies in formation
    enemiesInFormation = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isInFormation()) enemiesInFormation++;
    }

    // Check if all enemies are defeated
    if (enemies.empty()) {
        stage++;
        startNewStage();
        return;
    }
}

void Game::updateChallenging() {
    // Check if challenging stage is complete
    if (stageClock.getElapsedTime().asSeconds() > 30.f || enemies.empty()) {
        stage++;
        startNewStage();
        return;
    }
}

void Game::checkCollisions() {
    // Bullet collisions
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
        bool bulletHit = false;
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;

            // Check if bullet intersects with enemy using proper bounding box collision
            if (bulletIt->getSprite().getGlobalBounds()
                .contains(enemy.getSprite().getPosition())) {
                
                std::cout << "Bullet hit enemy!" << std::endl;
                
                if (enemy.hasCapturedShip()) {
                    // Rescued captured ship
                    std::cout << "Rescued captured ship!" << std::endl;
                    player.rescue();
                    updateScore(1000);
                }

                enemy.kill();
                bulletHit = true;
                updateScore(enemy.getPointValue());
                break;
            }
        }
        if (bulletHit) {
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    // Enemy-Player collisions
    if (player.isAlive() && !player.isCaptureState()) {
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;

            // Check if player intersects with enemy using proper bounding box collision
            sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
            sf::FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();
            if (playerBounds.contains(enemy.getSprite().getPosition()) || 
                enemyBounds.contains(player.getSprite().getPosition())) {
                
                std::cout << "Player collided with enemy!" << std::endl;
                
                if (enemy.getType() == EnemyType::BOSS && enemy.canCapture()) {
                    std::cout << "Boss captured player!" << std::endl;
                    handleCapture(enemy);
                } else {
                    std::cout << "Player damaged by enemy!" << std::endl;
                    player.damage(1);
                    std::cout << "Player health after damage: " << player.getHealth() << std::endl;
                }
                break;
            }
        }
    }
}

void Game::handleCapture(Enemy& boss) {
    player.capture();
    boss.setCapturedShip(true);
}

void Game::startNewStage() {
    std::cout << "Starting new stage: " << stage << std::endl;
    
    if (stage % 3 == 0) {
        // Every third stage is a challenging stage
        std::cout << "Spawning challenging stage" << std::endl;
        spawnChallengingStage();
        gameState = GameState::CHALLENGING;
    } else {
        std::cout << "Spawning formation stage" << std::endl;
        spawnFormation();
        gameState = GameState::PLAYING;
    }
    
    // Update stage text
    stageText.setString("Stage " + std::to_string(stage));
    // Center the stage text
    stageText.setPosition(sf::Vector2f(400.f - stageText.getLocalBounds().size.x / 2.f, 300.f));
    
    std::cout << "Player health: " << player.getHealth() << std::endl;
    std::cout << "Enemies spawned: " << enemies.size() << std::endl;
}

void Game::applySettings() {
    // Apply settings from the settings dialog
    float musicVolume = settings.getMusicVolume();
    float soundVolume = settings.getSoundVolume();
    bool fullscreen = settings.isFullscreen();
    
    // Apply fullscreen setting
    if (fullscreen) {
        // In SFML 3.0, fullscreen is handled differently
        auto fullscreenMode = sf::VideoMode::getFullscreenModes()[0];
        window.create(fullscreenMode, "Galactical Conquest", sf::Style::fullscreen);
    } else {
        window.create(sf::VideoMode({800, 600}), "Galactical Conquest");
    }
    
    // TODO: Apply music and sound volume settings when audio is implemented
    stage = 1;
    stageClock.restart();
    attackClock.restart();
    gameTimer.restart();

    enemies.clear();
    bullets.clear();
    formationSlots.clear();

    player = Player();

    // Position and style HUD elements
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));
    healthText.setPosition(sf::Vector2f(10.f, 40.f));
    stageText.setPosition(sf::Vector2f(10.f, 70.f));
    
    // Center the game over text
    gameOverText.setPosition(sf::Vector2f(400.f, 300.f));
}

void Game::updateScore(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
}

void Game::renderHUD() {
    scoreText.setString("Score: " + std::to_string(score) + "\nHigh Score: " + std::to_string(highScore));
    healthText.setString("Lives: " + std::to_string(player.getHealth()));
    stageText.setString("Stage " + std::to_string(stage));
    
    window.draw(scoreText);
    window.draw(healthText);
    window.draw(stageText);

    // Show stage transition text
    if (gameState == GameState::CHALLENGING) {
        sf::Text challengeText(gameFont, "Challenging Stage!", 40);
        // Center the challenge text horizontally
        challengeText.setPosition(sf::Vector2f(400.f, 50.f));
        window.draw(challengeText);
    }
}

