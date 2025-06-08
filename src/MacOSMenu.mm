#include "MacOSMenu.h"

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>

void setupMacOSMenu(const std::string& appName) {
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
                                                         action:nil
                                                  keyEquivalent:@","];
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
                                                      action:nil
                                               keyEquivalent:@"n"];
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
}

#else

void setupMacOSMenu(const std::string& appName) {
    // Do nothing on non-macOS platforms
}

#endif
