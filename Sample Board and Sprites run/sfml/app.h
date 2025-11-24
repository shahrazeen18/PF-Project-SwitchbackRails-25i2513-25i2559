#ifndef APP_H
#define APP_H

// ============================================================================
// APP.H - SFML application for visualization (NO CLASSES)
// ============================================================================
// This module uses only functions and global variables for the SFML frontend.
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// Initialize the SFML window and resources
// Returns true on success, false on failure
bool initializeApp();

// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
// Run the main application loop
void runApp();

// ----------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
// Clean up resources and close window
void cleanupApp();

#endif
