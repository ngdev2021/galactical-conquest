#pragma once

#include <string>
#include <functional>

/**
 * Bridge class for macOS-specific functionality
 * This class provides a C++ interface to Objective-C/Cocoa functionality
 */
class MacOSBridge {
public:
    /**
     * Initialize the macOS bridge
     * @param appName The name of the application
     */
    static void initialize(const std::string& appName);
    
    /**
     * Set the callback for the New Game menu item
     * @param callback Function to call when New Game is selected
     */
    static void setNewGameCallback(std::function<void()> callback);
    
    /**
     * Set the callback for the Preferences menu item
     * @param callback Function to call when Preferences is selected
     */
    static void setPreferencesCallback(std::function<void()> callback);
    
    /**
     * Show the About panel with custom information
     * @param appName Application name
     * @param version Version string
     * @param copyright Copyright information
     * @param credits Credits text
     */
    static void showAboutPanel(
        const std::string& appName,
        const std::string& version,
        const std::string& copyright,
        const std::string& credits
    );
    
private:
    // No private members needed
};
