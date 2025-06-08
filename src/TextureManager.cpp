#include "TextureManager.h"
#include <iostream>

sf::Texture TextureManager::emptyTexture;
bool TextureManager::initialized = false;
std::map<std::string, sf::Texture> TextureManager::textureMap;
std::mutex TextureManager::textureMutex;

// Font static members
sf::Font TextureManager::defaultFont;
bool TextureManager::fontInitialized = false;
std::map<std::string, sf::Font> TextureManager::fontMap;
std::mutex TextureManager::fontMutex;

sf::Texture& TextureManager::getEmptyTexture() {
    std::lock_guard<std::mutex> lock(textureMutex);
    
    if (!initialized) {
        // Create a 1x1 white pixel image
        sf::Image img;
        img = sf::Image(sf::Vector2u(1, 1), sf::Color::White);
        
        try {
            if (!emptyTexture.loadFromImage(img)) {
                std::cerr << "Failed to create empty texture!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception creating empty texture: " << e.what() << std::endl;
        }
        
        initialized = true;
    }
    return emptyTexture;
}

sf::Texture& TextureManager::getTexture(const std::string& filename) {
    std::lock_guard<std::mutex> lock(textureMutex);
    
    // Check if texture is already loaded
    auto it = textureMap.find(filename);
    if (it != textureMap.end()) {
        return it->second;
    }
    
    // Load the texture
    sf::Texture& texture = textureMap[filename];
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        textureMap.erase(filename);
        return getEmptyTexture();
    }
    
    std::cout << "Loaded texture: " << filename << std::endl;
    return texture;
}

void TextureManager::clearTextures() {
    std::lock_guard<std::mutex> lock(textureMutex);
    textureMap.clear();
}

sf::Font& TextureManager::getFont() {
    std::lock_guard<std::mutex> lock(fontMutex);
    
    if (!fontInitialized) {
        // Try to load a system font
        if (!defaultFont.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
            std::cerr << "Failed to load default font! Trying fallback fonts..." << std::endl;
            
            // Try some common fallback fonts
            const std::vector<std::string> fallbackPaths = {
                "/System/Library/Fonts/Helvetica.ttc",
                "/Library/Fonts/Arial.ttf",
                "assets/fonts/arial.ttf",
                "C:/Windows/Fonts/arial.ttf"
            };
            
            bool loaded = false;
            for (const auto& path : fallbackPaths) {
                if (defaultFont.openFromFile(path)) {
                    std::cout << "Loaded fallback font: " << path << std::endl;
                    loaded = true;
                    break;
                }
            }
            
            if (!loaded) {
                std::cerr << "Failed to load any font! Text will not display correctly." << std::endl;
            }
        } else {
            std::cout << "Loaded default font" << std::endl;
        }
        
        fontInitialized = true;
    }
    
    return defaultFont;
}

sf::Font& TextureManager::getFont(const std::string& filename) {
    std::lock_guard<std::mutex> lock(fontMutex);
    
    // Check if font is already loaded
    auto it = fontMap.find(filename);
    if (it != fontMap.end()) {
        return it->second;
    }
    
    // Load the font
    sf::Font& font = fontMap[filename];
    if (!font.openFromFile(filename)) {
        std::cerr << "Failed to load font: " << filename << std::endl;
        fontMap.erase(filename);
        return getFont(); // Return default font as fallback
    }
    
    std::cout << "Loaded font: " << filename << std::endl;
    return font;
}
