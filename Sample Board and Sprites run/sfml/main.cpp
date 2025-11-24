#include "app.h"
#include <iostream>

// ============================================================================
// MAIN.CPP - Entry point of the application
// ============================================================================

int main() {
    std::cout << "Initializing SFML Sprite Transformations Demo..." << std::endl;
    
    if (!initializeApp()) {
        std::cerr << "Failed to initialize application!" << std::endl;
        return 1;
    }
    
    std::cout << "Application initialized successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Displaying 3 rows with different transformations:" << std::endl;
    std::cout << "  Row 0: Original images (40x40px)" << std::endl;
    std::cout << "  Row 1: Zoomed images (2x)" << std::endl;
    std::cout << "  Row 2: Rotated images (45 degrees)" << std::endl;
    
    runApp();
    
    cleanupApp();
    
    std::cout << "Application closed." << std::endl;
    return 0;
}
