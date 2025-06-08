#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    enum class State {
        MainMenu,
        Settings,
        Game,
        Exit
    };

    Menu();
    void handleInput(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);
    State getState() const { return currentState; }
    
    // Settings getters
    float getMusicVolume() const { return musicVolume; }
    float getSoundVolume() const { return soundVolume; }
    bool getFullscreen() const { return fullscreen; }

private:
    void initializeMainMenu();
    void initializeSettings();
    void updateMenuText();

    State currentState;
    std::size_t selectedItem;
    std::vector<sf::Text> menuItems;
    sf::Font font;
    
    // Settings
    float musicVolume;
    float soundVolume;
    bool fullscreen;
    
    // Background
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    
    // Title
    sf::Text titleText;
};
