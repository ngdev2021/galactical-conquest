CXX=g++
CXXFLAGS=-std=c++17 -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

# For macOS specific frameworks
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework Cocoa
endif

SRC=src/main.cpp src/Game.cpp src/Menu.cpp src/Player.cpp src/Enemy.cpp src/Bullet.cpp src/TextureManager.cpp src/Settings.cpp
MM_SRC=src/MacOSBridge.mm

all: galactical-conquest

galactical-conquest: $(SRC) $(MM_SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(MM_SRC) -o galactical-conquest $(LDFLAGS)

clean:
	rm -f galactical-conquest
