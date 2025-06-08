#include "Menu.h"
#include "TextureManager.h"
#include <stdexcept>
#include <iostream>

Menu::Menu()
    : currentState(State::MainMenu)
    , selectedItem(0)
    , musicVolume(50.f)
    , soundVolume(50.f)
    , fullscreen(false)
    , bgSprite(TextureManager::getEmptyTexture())
    , titleText(TextureManager::getFont(), "Galactical Conquest", 72) {
    
    // Use TextureManager to load font
    font = TextureManager::getFont();
    
    // Use TextureManager to load background texture
    bgTexture = TextureManager::getTexture("assets/background.png");
    
    // Initialize sprite with the texture
    bgSprite = sf::Sprite(bgTexture);
    
    std::cout << "Menu background texture loaded" << std::endl;

    // Set title text style
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    
    // Center the title
    auto titleBounds = titleText.getLocalBounds();
    titleText.setPosition({400.f - titleBounds.size.x / 2.f, 100.f});
    
    std::cout << "Menu initialized successfully" << std::endl;

    initializeMainMenu();
}

void Menu::initializeMainMenu() {
    menuItems.clear();
    selectedItem = 0;

    std::vector<std::string> items = {
        "Play Game",
        "Settings",
        "Exit"
    };

    float verticalSpacing = 80.f;
    float startY = 300.f;

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], 36);
        text.setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::White);
        
        // Center each menu item
        text.setPosition({400.f - text.getLocalBounds().size.x / 2.f, startY + i * verticalSpacing});
        
        menuItems.push_back(text);
    }
}

void Menu::initializeSettings() {
    menuItems.clear();
    selectedItem = 0;

    std::vector<std::string> items = {
        "Music Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%",
        "Sound Volume: " + std::to_string(static_cast<int>(soundVolume)) + "%",
        "Fullscreen: " + std::string(fullscreen ? "On" : "Off"),
        "Back"
    };

    float verticalSpacing = 80.f;
    float startY = 300.f;

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], 36);
        text.setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::White);
        
        // Center each menu item
        text.setPosition({400.f - text.getLocalBounds().size.x / 2.f, startY + i * verticalSpacing});
        
        menuItems.push_back(text);
    }
}

void Menu::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        if (keyEvent->scancode == sf::Keyboard::Scancode::Up) {
            if (selectedItem > 0) {
                selectedItem--;
                updateMenuText();
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Down) {
            if (selectedItem < menuItems.size() - 1) {
                selectedItem++;
                updateMenuText();
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Enter ||
                 keyEvent->scancode == sf::Keyboard::Scancode::Space) {
            if (currentState == State::MainMenu) {
                switch (selectedItem) {
                    case 0: // Play
                        currentState = State::Game;
                        break;
                    case 1: // Settings
                        currentState = State::Settings;
                        initializeSettings();
                        break;
                    case 2: // Exit
                        currentState = State::Exit;
                        break;
                }
            }
            else if (currentState == State::Settings) {
                if (selectedItem == menuItems.size() - 1) { // Back
                    currentState = State::MainMenu;
                    initializeMainMenu();
                }
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Left) {
            if (currentState == State::Settings) {
                switch (selectedItem) {
                    case 0: // Music Volume
                        musicVolume = std::max(0.f, musicVolume - 10.f);
                        break;
                    case 1: // Sound Volume
                        soundVolume = std::max(0.f, soundVolume - 10.f);
                        break;
                    case 2: // Fullscreen
                        fullscreen = !fullscreen;
                        break;
                }
                initializeSettings();
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Right) {
            if (currentState == State::Settings) {
                switch (selectedItem) {
                    case 0: // Music Volume
                        musicVolume = std::min(100.f, musicVolume + 10.f);
                        break;
                    case 1: // Sound Volume
                        soundVolume = std::min(100.f, soundVolume + 10.f);
                        break;
                    case 2: // Fullscreen
                        fullscreen = !fullscreen;
                        break;
                }
                initializeSettings();
            }
        }
    }
}

void Menu::updateMenuText() {
    for (size_t i = 0; i < menuItems.size(); ++i) {
        menuItems[i].setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::White);
    }
}

void Menu::update() {
    // Add any animations or effects here
}

void Menu::render(sf::RenderWindow& window) {
    window.draw(bgSprite);
    window.draw(titleText);
    
    for (const auto& item : menuItems) {
        window.draw(item);
    }
}
