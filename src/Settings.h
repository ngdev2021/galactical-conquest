#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

/**
 * Settings dialog for the game
 */
class Settings {
public:
    /**
     * Constructor
     */
    Settings();
    
    /**
     * Show the settings dialog
     * @param window The render window to draw on
     */
    void show(sf::RenderWindow& window);
    
    /**
     * Handle input events
     * @param event The SFML event to handle
     * @return true if the event was handled, false otherwise
     */
    bool handleInput(const sf::Event& event);
    
    /**
     * Update the settings dialog
     */
    void update();
    
    /**
     * Render the settings dialog
     * @param window The render window to draw on
     */
    void render(sf::RenderWindow& window);
    
    /**
     * Check if the settings dialog is visible
     * @return true if the settings dialog is visible, false otherwise
     */
    bool isVisible() const { return visible; }
    
    /**
     * Set the callback for when settings are changed
     * @param callback Function to call when settings are changed
     */
    void setOnSettingsChanged(std::function<void()> callback) { onSettingsChanged = callback; }
    
    /**
     * Get the music volume
     * @return Music volume (0-100)
     */
    float getMusicVolume() const { return musicVolume; }
    
    /**
     * Get the sound volume
     * @return Sound volume (0-100)
     */
    float getSoundVolume() const { return soundVolume; }
    
    /**
     * Check if fullscreen mode is enabled
     * @return true if fullscreen mode is enabled, false otherwise
     */
    bool isFullscreen() const { return fullscreen; }
    
private:
    bool visible;
    float musicVolume;
    float soundVolume;
    bool fullscreen;
    
    sf::Font font;
    sf::RectangleShape background;
    sf::Text titleText;
    
    std::vector<sf::Text> menuItems;
    int selectedItem;
    
    std::function<void()> onSettingsChanged;
    
    void initializeMenuItems();
};
