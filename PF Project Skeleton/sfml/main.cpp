#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>
using namespace std;
// ============================================================================
// MAIN.CPP - Entry point of the application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// MAIN ENTRY POINT
// ----------------------------------------------------------------------------
// This function is the main entry point of the application. It handles command
// line arguments to specify the level file to load, loads the level file using
// loadLevelFile, initializes the simulation system, initializes the SFML
// application window, prints control instructions to the console, runs the
// main application loop, cleans up resources, and prints final simulation
// statistics. Returns 0 on success, 1 on error (e.g., failed to load level
// file or initialize application).
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    // Handles Command Line Arguments
    if (argc < 2) {
        cout << "Usage: ./switchback_rails <level_file_path>" << endl;
        cout << "Example: ./switchback_rails data/levels/easy_level.lvl" << endl;
        return 1;
    }

    // Loads the Level File
    // argv[1] contains the path string (e.g. easy level)
    cout << "Loading level: " << argv[1] << "..." << endl;
    if (!loadLevelFile(argv[1])) {
        cout << "Error: Failed to load level file." << endl;
        return 1;
    }
    
    // Initializes Simulation Logic
    initializeSimulation(); // Sets up any runtime counters (in simulation.cpp)

    // Initializes SFML Application
    if (!initializeApp()) {
        cout << "Error: Failed to initialize application window." << endl;
        return 1;
    }

    // Prints Controls
    cout << "========================================" << endl;
    cout << "       SWITCHBACK RAILS CONTROL         " << endl;
    cout << "========================================" << endl;
    cout << " SPACE        : Pause/Resume" << endl;
    cout << " . (Period)   : Step forward one tick" << endl;
    cout << " Left-Click   : Toggle Safety Tile (=)" << endl;
    cout << " Right-Click  : Toggle Switch State" << endl;
    cout << " Middle-Drag  : Pan Camera" << endl;
    cout << " Mouse Wheel  : Zoom In/Out" << endl;
    cout << " ESC          : Exit and Save" << endl;
    cout << "========================================" << endl;

    // Runs the Application Loop
    runApp();

    // Cleans up Resources
    cleanupApp();
    
    // Prints Final Statistics 
    cout << "Simulation ended." << endl;
    writeMetrics();

    return 0;
}
