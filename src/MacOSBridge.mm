#include "MacOSBridge.h"

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>

// Static callback storage
static std::function<void()> gNewGameCallback = nullptr;
static std::function<void()> gPreferencesCallback = nullptr;

// Objective-C class to handle menu actions
@interface MenuActionHandler : NSObject
+ (void)newGame:(id)sender;
+ (void)showPreferences:(id)sender;
+ (void)showAboutPanel:(id)sender;
@end

@implementation MenuActionHandler
+ (void)newGame:(id)sender {
    if (gNewGameCallback) {
        gNewGameCallback();
    }
}

+ (void)showPreferences:(id)sender {
    if (gPreferencesCallback) {
        gPreferencesCallback();
    }
}

+ (void)showAboutPanel:(id)sender {
    // About panel is handled by MacOSBridge::showAboutPanel
}
@end

// Global variable to track initialization state
static bool g_initialized = false;

void MacOSBridge::initialize(const std::string& appName) {
    if (g_initialized) return;
    
    // Get the shared application instance
    NSApplication* app = [NSApplication sharedApplication];
    
    // Create the main menu
    NSMenu* mainMenu = [[NSMenu alloc] init];
    [NSApp setMainMenu:mainMenu];
    
    // Application menu (first menu)
    NSMenuItem* appMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:appMenuItem];
    
    NSMenu* appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];
    
    // About item
    NSString* aboutTitle = [NSString stringWithFormat:@"About %@", [NSString stringWithUTF8String:appName.c_str()]];
    NSMenuItem* aboutMenuItem = [[NSMenuItem alloc] initWithTitle:aboutTitle
                                                         action:@selector(orderFrontStandardAboutPanel:)
                                                  keyEquivalent:@""];
    [aboutMenuItem setTarget:NSApp];
    [appMenu addItem:aboutMenuItem];
    
    // Separator
    [appMenu addItem:[NSMenuItem separatorItem]];
    
    // Preferences item
    NSMenuItem* prefsMenuItem = [[NSMenuItem alloc] initWithTitle:@"Preferences..."
                                                         action:@selector(showPreferences:)
                                                  keyEquivalent:@","];
    [prefsMenuItem setTarget:[MenuActionHandler class]];
    [appMenu addItem:prefsMenuItem];
    
    // Separator
    [appMenu addItem:[NSMenuItem separatorItem]];
    
    // Services menu
    NSMenuItem* servicesMenuItem = [[NSMenuItem alloc] init];
    [servicesMenuItem setTitle:@"Services"];
    [appMenu addItem:servicesMenuItem];
    NSMenu* servicesMenu = [[NSMenu alloc] init];
    [servicesMenuItem setSubmenu:servicesMenu];
    [NSApp setServicesMenu:servicesMenu];
    
    // Separator
    [appMenu addItem:[NSMenuItem separatorItem]];
    
    // Hide app
    NSString* hideTitle = [NSString stringWithFormat:@"Hide %@", [NSString stringWithUTF8String:appName.c_str()]];
    NSMenuItem* hideMenuItem = [[NSMenuItem alloc] initWithTitle:hideTitle
                                                        action:@selector(hide:)
                                                 keyEquivalent:@"h"];
    [hideMenuItem setTarget:NSApp];
    [appMenu addItem:hideMenuItem];
    
    // Hide others
    NSMenuItem* hideOthersMenuItem = [[NSMenuItem alloc] initWithTitle:@"Hide Others"
                                                             action:@selector(hideOtherApplications:)
                                                      keyEquivalent:@"h"];
    [hideOthersMenuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand | NSEventModifierFlagOption];
    [hideOthersMenuItem setTarget:NSApp];
    [appMenu addItem:hideOthersMenuItem];
    
    // Show all
    NSMenuItem* showAllMenuItem = [[NSMenuItem alloc] initWithTitle:@"Show All"
                                                          action:@selector(unhideAllApplications:)
                                                   keyEquivalent:@""];
    [showAllMenuItem setTarget:NSApp];
    [appMenu addItem:showAllMenuItem];
    
    // Separator
    [appMenu addItem:[NSMenuItem separatorItem]];
    
    // Quit
    NSString* quitTitle = [NSString stringWithFormat:@"Quit %@", [NSString stringWithUTF8String:appName.c_str()]];
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                       action:@selector(terminate:)
                                                keyEquivalent:@"q"];
    [quitMenuItem setTarget:NSApp];
    [appMenu addItem:quitMenuItem];
    
    // File menu
    NSMenuItem* fileMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:fileMenuItem];
    
    NSMenu* fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    [fileMenuItem setSubmenu:fileMenu];
    
    // New Game
    NSMenuItem* newMenuItem = [[NSMenuItem alloc] initWithTitle:@"New Game"
                                                      action:@selector(newGame:)
                                               keyEquivalent:@"n"];
    [newMenuItem setTarget:[MenuActionHandler class]];
    [fileMenu addItem:newMenuItem];
    
    // Separator
    [fileMenu addItem:[NSMenuItem separatorItem]];
    
    // Close window
    NSMenuItem* closeMenuItem = [[NSMenuItem alloc] initWithTitle:@"Close"
                                                        action:@selector(performClose:)
                                                 keyEquivalent:@"w"];
    [fileMenu addItem:closeMenuItem];
    
    // Window menu
    NSMenuItem* windowMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:windowMenuItem];
    
    NSMenu* windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    [windowMenuItem setSubmenu:windowMenu];
    
    // Minimize window
    NSMenuItem* minimizeMenuItem = [[NSMenuItem alloc] initWithTitle:@"Minimize"
                                                           action:@selector(performMiniaturize:)
                                                    keyEquivalent:@"m"];
    [windowMenu addItem:minimizeMenuItem];
    
    // Zoom window
    NSMenuItem* zoomMenuItem = [[NSMenuItem alloc] initWithTitle:@"Zoom"
                                                       action:@selector(performZoom:)
                                                keyEquivalent:@""];
    [windowMenu addItem:zoomMenuItem];
    
    // Separator
    [windowMenu addItem:[NSMenuItem separatorItem]];
    
    // Bring all to front
    NSMenuItem* frontMenuItem = [[NSMenuItem alloc] initWithTitle:@"Bring All to Front"
                                                        action:@selector(arrangeInFront:)
                                                 keyEquivalent:@""];
    [frontMenuItem setTarget:NSApp];
    [windowMenu addItem:frontMenuItem];
    
    // Set as Window menu
    [NSApp setWindowsMenu:windowMenu];
    
    // Help menu
    NSMenuItem* helpMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:helpMenuItem];
    
    NSMenu* helpMenu = [[NSMenu alloc] initWithTitle:@"Help"];
    [helpMenuItem setSubmenu:helpMenu];
    
    // Help item
    NSString* helpTitle = [NSString stringWithFormat:@"%@ Help", [NSString stringWithUTF8String:appName.c_str()]];
    NSMenuItem* helpDocMenuItem = [[NSMenuItem alloc] initWithTitle:helpTitle
                                                          action:nil
                                                   keyEquivalent:@"?"];
    [helpMenu addItem:helpDocMenuItem];
    
    // Set as Help menu
    [NSApp setHelpMenu:helpMenu];
    
    g_initialized = true;
}

void MacOSBridge::setNewGameCallback(std::function<void()> callback) {
    gNewGameCallback = callback;
}

void MacOSBridge::setPreferencesCallback(std::function<void()> callback) {
    gPreferencesCallback = callback;
}

void MacOSBridge::showAboutPanel(
    const std::string& appName,
    const std::string& version,
    const std::string& copyright,
    const std::string& credits
) {
    NSMutableDictionary* options = [NSMutableDictionary dictionary];
    
    if (!appName.empty()) {
        [options setObject:[NSString stringWithUTF8String:appName.c_str()] 
                    forKey:@"ApplicationName"];
    }
    
    if (!version.empty()) {
        [options setObject:[NSString stringWithUTF8String:version.c_str()] 
                    forKey:@"Version"];
    }
    
    if (!copyright.empty()) {
        [options setObject:[NSString stringWithUTF8String:copyright.c_str()] 
                    forKey:@"Copyright"];
    }
    
    if (!credits.empty()) {
        [options setObject:[NSString stringWithUTF8String:credits.c_str()] 
                    forKey:@"ApplicationVersion"];
    }
    
    [NSApp orderFrontStandardAboutPanelWithOptions:options];
}

#else // Non-macOS implementation

bool MacOSBridge::initialized = false;

void MacOSBridge::initialize(const std::string& appName) {
    // Do nothing on non-macOS platforms
    initialized = true;
}

void MacOSBridge::setNewGameCallback(std::function<void()> callback) {
    // Do nothing on non-macOS platforms
}

void MacOSBridge::setPreferencesCallback(std::function<void()> callback) {
    // Do nothing on non-macOS platforms
}

void MacOSBridge::showAboutPanel(
    const std::string& appName,
    const std::string& version,
    const std::string& copyright,
    const std::string& credits
) {
    // Do nothing on non-macOS platforms
}

#endif
