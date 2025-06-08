#include "Settings.h"
#include "TextureManager.h"
#include <iostream>

Settings::Settings()
    : visible(false)
    , musicVolume(50.f)
    , soundVolume(50.f)
    , fullscreen(false)
    , selectedItem(0)
    , font(TextureManager::getFont())
    , titleText(font, "Settings", 36) {
    
    // Setup background
    background.setSize({600.f, 400.f});
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2.f);
    background.setPosition({100.f, 100.f});
    
    // Setup title
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    
    // Center the title
    auto titleBounds = titleText.getLocalBounds();
    titleText.setPosition({
        background.getPosition().x + background.getSize().x / 2.f - titleBounds.size.x / 2.f,
        background.getPosition().y + 20.f
    });
    
    initializeMenuItems();
}

void Settings::show(sf::RenderWindow& window) {
    visible = true;
    initializeMenuItems();
}

bool Settings::handleInput(const sf::Event& event) {
    if (!visible) return false;
    
    if (event.is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        if (keyEvent->scancode == sf::Keyboard::Scancode::Escape) {
            visible = false;
            return true;
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Up) {
            if (selectedItem > 0) {
                selectedItem--;
                initializeMenuItems();
                return true;
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Down) {
            if (selectedItem < menuItems.size() - 1) {
                selectedItem++;
                initializeMenuItems();
                return true;
            }
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Left) {
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
                default:
                    return false;
            }
            
            initializeMenuItems();
            if (onSettingsChanged) {
                onSettingsChanged();
            }
            return true;
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Right) {
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
                default:
                    return false;
            }
            
            initializeMenuItems();
            if (onSettingsChanged) {
                onSettingsChanged();
            }
            return true;
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Enter ||
                 keyEvent->scancode == sf::Keyboard::Scancode::Space) {
            if (selectedItem == menuItems.size() - 1) { // Close
                visible = false;
                return true;
            }
        }
    }
    
    return false;
}

void Settings::update() {
    // Nothing to update here
}

void Settings::render(sf::RenderWindow& window) {
    if (!visible) return;
    
    window.draw(background);
    window.draw(titleText);
    
    for (const auto& item : menuItems) {
        window.draw(item);
    }
}

void Settings::initializeMenuItems() {
    menuItems.clear();
    
    std::vector<std::string> items = {
        "Music Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%",
        "Sound Volume: " + std::to_string(static_cast<int>(soundVolume)) + "%",
        "Fullscreen: " + std::string(fullscreen ? "On" : "Off"),
        "Close"
    };
    
    float verticalSpacing = 50.f;
    float startY = background.getPosition().y + 100.f;
    
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], 24);
        text.setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::White);
        
        // Center each menu item
        auto bounds = text.getLocalBounds();
        text.setPosition({
            background.getPosition().x + background.getSize().x / 2.f - bounds.size.x / 2.f,
            startY + i * verticalSpacing
        });
        
        menuItems.push_back(text);
    }
}
