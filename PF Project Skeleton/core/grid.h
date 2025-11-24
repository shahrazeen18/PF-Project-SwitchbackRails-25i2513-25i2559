#ifndef GRID_H
#define GRID_H

// ============================================================================
// GRID.H - Grid manipulation functions
// ============================================================================
// Functions for working with the 2D grid map.
// ============================================================================

// Check if a position is within grid bounds
bool isInBounds();

// Check if a tile is a track (can trains move on it?)
bool isTrackTile();

// Check if a tile is a switch (A-Z)
bool isSwitchTile();

// Get the switch index (0-25) from a switch character (A-Z)
int getSwitchIndex();

// Check if a position is a spawn point
bool isSpawnPoint();

// Check if a position is a destination point
bool isDestinationPoint();

// Place or remove a safety tile at a position (for mouse editing)
// Returns true if successful
bool toggleSafetyTile();

#endif
