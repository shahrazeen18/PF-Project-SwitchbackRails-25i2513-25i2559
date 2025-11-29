#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>

// ============================================================================
// APP.CPP - Implementation of SFML application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;

// View for camera (panning/zoom)
static sf::View g_camera;

// Simulation state
static bool g_isPaused = true; // Default to paused so user can see start state
static bool g_isStepMode = false;

// Mouse state
static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

// Grid rendering parameters
static float g_cellSize = 40.0f;
static float g_gridOffsetX = 50.0f;
static float g_gridOffsetY = 50.0f;

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// This function will initialize the SFML application window and resources.
// It creates a render window with a specified size and title, sets the
// framerate limit, attempts to load a font file for text rendering, and
// initializes the camera view. Returns true on success, false on failure.
// This should be called once at the start of the application before entering
// the main loop.
// ----------------------------------------------------------------------------
bool initializeApp() {
    // Creates the Window
    // We allocate the window on the heap as per the global pointer
    g_window = new sf::RenderWindow(sf::VideoMode(1200, 800), "Switchback Rails");
    (*g_window).setFramerateLimit(60);

    // Loads Font
    // Required for drawing text. If this fails, text won't show, but we return true to run.
    if (!g_font.loadFromFile("data/levels/arial.ttf")) {
        // Font failed to load. Ensure arial.ttf is in data/levels directory.
    }

    // Setup Camera
    g_camera = (*g_window).getDefaultView(); // Center the camera on the middle of the loaded level
    // We used global variables from simulation_state.h
    float centerX = (LevelNumCols * g_cellSize) / 2.0f;
    float centerY = (LevelNumRows * g_cellSize) / 2.0f;
    g_camera.setCenter(centerX, centerY);

    return true;
}

// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
// This function will run the main application loop. It handles event processing,
// simulation updates, and rendering. The loop continues while the window is open.
// It processes SFML events (window close, keyboard input, mouse input), updates
// the simulation at a fixed interval (2 ticks per second) when not paused,
// checks if the simulation is complete, and renders the current frame. Keyboard
// controls: SPACE to pause/resume, PERIOD to step one tick, ESC to exit. The
// loop exits when the window is closed or ESC is pressed.
// ----------------------------------------------------------------------------
void runApp() {
    sf::Clock updateClock;
    float timeSinceLastTick = 0.0f;
    const float TICK_RATE = 0.5f; // 2 ticks per second

    while ((*g_window).isOpen()) {
        // ====================================================================
        // 1. EVENT PROCESSING
        // ====================================================================
        sf::Event event;
        while ((*g_window).pollEvent(event)) {
            // Closes Window
            if (event.type == sf::Event::Closed) {
                (*g_window).close();
            }
            // Keyboard Input
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) (*g_window).close();
                if (event.key.code == sf::Keyboard::Space) g_isPaused = !g_isPaused;
                if (event.key.code == sf::Keyboard::Period) {
                    simulateOneTick();
                    timeSinceLastTick = 0.0f; // Reset timer so we don't double step
                }
            }
            // Mouse Wheel (Zoom)
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) g_camera.zoom(0.9f); // Zoom In
                else g_camera.zoom(1.1f); // Zoom Out
            }
            // Mouse Button Pressed
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Middle Click = Start Panm
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    g_isDragging = true;
                    g_lastMouseX = event.mouseButton.x;
                    g_lastMouseY = event.mouseButton.y;
                }
                // Handle Left or Right click
                else if (event.mouseButton.button == sf::Mouse::Left || 
                         event.mouseButton.button == sf::Mouse::Right) {
                    
                    // Gets Screen Pixels
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(*g_window);
                    // Converts to World Coords
                    sf::Vector2f worldPos = (*g_window).mapPixelToCoords(pixelPos, g_camera);
                    // Calculates Grid Index (r, c)
                    int c = (int)(worldPos.x / g_cellSize);
                    int r = (int)(worldPos.y / g_cellSize);

                    // Performs Action
                   if(r >=0 && r < LevelNumRows && c >= 0 && c < LevelNumCols)  {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        toggleSafetyTile(r, c); // Uses r and c
                    } 
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        toggleSwitchState(); // Changes switches
                    } 
                }
            }
        }
            // Mouse Button Released
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    g_isDragging = false;
                }
            }
            // Mouse Movement (Panning)
            else if (event.type == sf::Event::MouseMoved) {
                if (g_isDragging) {
                    int dx = event.mouseMove.x - g_lastMouseX;
                    int dy = event.mouseMove.y - g_lastMouseY;
                    
                    // Adjusts camera position based on zoom level for consistent feel
                    float zoomFactor = g_camera.getSize().x / (*g_window).getSize().x;
                    g_camera.move(-dx * zoomFactor, -dy * zoomFactor);
                    
                    g_lastMouseX = event.mouseMove.x;
                    g_lastMouseY = event.mouseMove.y;
                }
            }
        }

        // ====================================================================
        // 2. SIMULATION UPDATE
        // ====================================================================
        if (!g_isPaused) {
            timeSinceLastTick += updateClock.restart().asSeconds();
            if (timeSinceLastTick >= TICK_RATE) {
                simulateOneTick();
                timeSinceLastTick = 0.0f;
            }
        } else {
            updateClock.restart(); // Doesn't accumulate time while paused
        }

        // ====================================================================
        // 3. RENDERING
        // ====================================================================
        (*g_window).clear(sf::Color(20, 20, 20)); // Dark Grey Background
        (*g_window).setView(g_camera);

        // Loop through the grid
        for (int r = 0; r < LevelNumRows; ++r) {
            for (int c = 0; c < LevelNumCols; ++c) {
                // Access global grid array
                char tile = TheGrid[r][c];
                
                // Calculates position
                float x = c * g_cellSize;
                float y = r * g_cellSize;

                // Creates Rectangle for Tile
                sf::RectangleShape shape(sf::Vector2f(g_cellSize - 2.0f, g_cellSize - 2.0f));
                shape.setPosition(x + 1.0f, y + 1.0f);

                // Determines Color
                sf::Color color = sf::Color(50, 50, 50); // Default Ground
                
                if (tile == '-') color = sf::Color(100, 100, 100);      // Horizontal Track
                else if (tile == '|') color = sf::Color(100, 100, 100); // Vertical Track
                else if (tile == '+') color = sf::Color(120, 120, 120); // Intersection
                else if (tile == '/' || tile == '\\') color = sf::Color(100, 100, 100); // Curves
                else if (tile == 'S') color = sf::Color::Cyan;          // Spawn Point
                else if (tile == 'D') color = sf::Color::Magenta;       // Destination
                else if (tile == '=') color = sf::Color::Blue;          // Safety Tile
                else if (tile >= 'A' && tile <= 'Z') {
                    color = sf::Color(255, 200, 0); // Switches are Yellow
                }

                shape.setFillColor(color);
                (*g_window).draw(shape);

                // Draw Text (Overlay chars like 'A', 'S', 'D')
                if ((tile >= 'A' && tile <= 'Z') || tile == 'S' || tile == 'D') {
                    if (g_font.getInfo().family != "") {
                        sf::Text text;
                        text.setFont(g_font);
                        
                        // Create C-string manually
                        char strBuffer[2];
                        strBuffer[0] = tile;
                        strBuffer[1] = '\0';
                        text.setString(strBuffer);

                        text.setCharacterSize(20);
                        text.setFillColor(sf::Color::Black);
                        
                        // Center text
                        sf::FloatRect bounds = text.getLocalBounds();
                        text.setPosition(
                            x + (g_cellSize - bounds.width) / 2.0f,
                            y + (g_cellSize - bounds.height) / 3.0f
                        );
                        (*g_window).draw(text);
                    }
                }
            }
        }
        for (int i = 0; i < TotalScheduledTrains; i++) {
            // Only draws Active trains (State == 1)
            if (TrainState[i] == 1) {
                int r = TrainCurrentRow[i];
                int c = TrainCurrentCol[i];
                
                float x = c * g_cellSize;
                float y = r * g_cellSize;

                sf::CircleShape trainShape(g_cellSize * 0.4f);
                trainShape.setPosition(x + g_cellSize * 0.1f, y + g_cellSize * 0.1f);
                
                // Color based on config
                int colorCode = TrainColorCode[i];
                if (colorCode == 0) trainShape.setFillColor(sf::Color::Red);
                else if (colorCode == 1) trainShape.setFillColor(sf::Color::Green);
                else if (colorCode == 2) trainShape.setFillColor(sf::Color::Blue);
                else trainShape.setFillColor(sf::Color::White);

                (*g_window).draw(trainShape);
            }
        }

        (*g_window).display();
    }
}

// ----------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
// This function will clean up all resources and close the application window.
// It deletes the render window object and sets the pointer to nullptr. This
// should be called once at the end of the application before exiting to ensure
// proper resource cleanup.
// ----------------------------------------------------------------------------
void cleanupApp() {
    if (g_window) {
        delete g_window; // Cleans the previous simulation 
        g_window = nullptr;
    }
}
