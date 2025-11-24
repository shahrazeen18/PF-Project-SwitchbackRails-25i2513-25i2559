#include "app.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>

// ============================================================================
// APP.CPP - Implementation of SFML application for sprite transformations
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;

// Sprite data - simple arrays
static sf::Texture g_textures[5];
static sf::Sprite g_sprites[5];
static bool g_loaded[5];
static const int NUM_SPRITES = 5;
static const int NUM_ROWS = 3;
static const int NUM_COLS = NUM_SPRITES;

// Grid rendering parameters
static const float SPRITE_SIZE = 40.0f;
static float g_spacingX = 60.0f;
static float g_spacingY = 80.0f;
static float g_offsetX = 50.0f;
static float g_offsetY = 50.0f;

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
bool initializeApp() {
    // Create window
    g_window = new sf::RenderWindow(sf::VideoMode(400, 350), "SFML Sprite Transformations");
    if (!g_window) {
        return false;
    }
    
    g_window->setFramerateLimit(60);
    
    // Load sprites and scale to 40x40px
    for (int i = 0; i < NUM_SPRITES; i++) {
        char path[50];
        sprintf(path, "Sprites/%d.png", i + 1);
        
        if (g_textures[i].loadFromFile(path)) {
            g_sprites[i].setTexture(g_textures[i]);
            
            // Get texture size and calculate scale to make it 40x40px
            sf::Vector2u texSize = g_textures[i].getSize();
            float scaleX = SPRITE_SIZE / texSize.x;
            float scaleY = SPRITE_SIZE / texSize.y;
            g_sprites[i].setScale(scaleX, scaleY);
            
            // Set origin to center for rotations
            g_sprites[i].setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
            g_loaded[i] = true;
        } else {
            std::cerr << "Failed to load: " << path << std::endl;
            g_loaded[i] = false;
        }
    }
    
    return true;
}

// ----------------------------------------------------------------------------
// RENDER FUNCTIONS FOR EACH TRANSFORMATION TYPE
// ----------------------------------------------------------------------------
void renderRow0_Original(int row) {
    // Row 0: Original images (no transformation)
    for (int col = 0; col < NUM_COLS; col++) {
        if (g_loaded[col]) {
            float x = g_offsetX + SPRITE_SIZE/2 + col * g_spacingX;
            float y = g_offsetY + SPRITE_SIZE/2 + row * g_spacingY;
            g_sprites[col].setPosition(x, y);
            g_sprites[col].setRotation(0);
            
            // Reset to base scale (40x40px)
            sf::Vector2u texSize = g_textures[col].getSize();
            float baseScaleX = SPRITE_SIZE / texSize.x;
            float baseScaleY = SPRITE_SIZE / texSize.y;
            g_sprites[col].setScale(baseScaleX, baseScaleY);
            g_sprites[col].setColor(sf::Color::White);
            g_window->draw(g_sprites[col]);
        }
    }
}

void renderRow1_Zoom(int row) {
    // Row 1: Zoomed images (2x scale)
    for (int col = 0; col < NUM_COLS; col++) {
        if (g_loaded[col]) {
            float x = g_offsetX + SPRITE_SIZE/2 + col * g_spacingX;
            float y = g_offsetY + SPRITE_SIZE/2 + row * g_spacingY;
            g_sprites[col].setPosition(x, y);
            g_sprites[col].setRotation(0);
            
            // Apply 2x zoom
            sf::Vector2u texSize = g_textures[col].getSize();
            float baseScaleX = SPRITE_SIZE / texSize.x;
            float baseScaleY = SPRITE_SIZE / texSize.y;
            g_sprites[col].setScale(baseScaleX * 2.0f, baseScaleY * 2.0f);
            g_sprites[col].setColor(sf::Color::White);
            g_window->draw(g_sprites[col]);
        }
    }
}

void renderRow2_Rotation(int row) {
    // Row 2: Rotated images (45 degrees)
    for (int col = 0; col < NUM_COLS; col++) {
        if (g_loaded[col]) {
            float x = g_offsetX + SPRITE_SIZE/2 + col * g_spacingX;
            float y = g_offsetY + SPRITE_SIZE/2 + row * g_spacingY;
            g_sprites[col].setPosition(x, y);
            g_sprites[col].setRotation(45.0f);
            
            // Reset to base scale (40x40px)
            sf::Vector2u texSize = g_textures[col].getSize();
            float baseScaleX = SPRITE_SIZE / texSize.x;
            float baseScaleY = SPRITE_SIZE / texSize.y;
            g_sprites[col].setScale(baseScaleX, baseScaleY);
            g_sprites[col].setColor(sf::Color::White);
            g_window->draw(g_sprites[col]);
        }
    }
}

// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
void runApp() {
    while (g_window->isOpen()) {
        sf::Event event;
        while (g_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                g_window->close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    g_window->close();
                }
            }
        }
        
        // Clear window
        g_window->clear(sf::Color(30, 30, 40));
        
        // Render 3 rows with different transformations
        renderRow0_Original(0);
        renderRow1_Zoom(1);
        renderRow2_Rotation(2);
        
        // Display
        g_window->display();
    }
}

// ----------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
void cleanupApp() {
    if (g_window) {
        delete g_window;
        g_window = nullptr;
    }
}
