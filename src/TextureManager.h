#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <map>
#include <string>

class TextureManager {
public:
    static sf::Texture& getEmptyTexture();
    
    // Add methods to load and get textures by name
    static sf::Texture& getTexture(const std::string& filename);
    static void clearTextures(); // Call this on shutdown
    
    // Font management
    static sf::Font& getFont();
    static sf::Font& getFont(const std::string& filename);
    
private:
    static sf::Texture emptyTexture;
    static bool initialized;
    static std::map<std::string, sf::Texture> textureMap;
    static std::mutex textureMutex;
    
    // Font members
    static sf::Font defaultFont;
    static bool fontInitialized;
    static std::map<std::string, sf::Font> fontMap;
    static std::mutex fontMutex;
};
