#include "Game.h"
#include "MacOSBridge.h"

int main() {
    // Create the game instance
    Game game;
    
    // Setup native macOS menu bar with callbacks
    MacOSBridge::initialize("Galactical Conquest");
    
    // Set up menu callbacks
    MacOSBridge::setNewGameCallback([&game]() { game.newGame(); });
    MacOSBridge::setPreferencesCallback([&game]() { game.showPreferences(); });
    
    // Run the game
    game.run();
    return 0;
}
