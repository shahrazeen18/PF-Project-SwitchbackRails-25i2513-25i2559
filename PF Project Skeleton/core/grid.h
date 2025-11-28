#ifndef GRID_H
#define GRID_H

// ============================================================================
// GRID.H - Grid manipulation functions
// ============================================================================
// Functions for working with the 2D grid map.
// ============================================================================

// Check if a position is within grid bounds
bool isInBounds( int r , int c);//gives if the row and column are inside map

// Check if a tile is a track (can trains move on it?)
bool isTrackTile( char tile );//tells us if the tile allows movement

// Check if a tile is a switch (A-Z)
bool isSwitchTile( int r , int c);//tells if the current tile is a switch

// Get the switch index (0-25) from a switch character (A-Z)
int getSwitchIndex( int r , int c);// gives 0 for A and 25 for Z

// Check if a position is a spawn point
bool isSpawnPoint( int r , int c); //checks for S tile

// Check if a position is a destination point
bool isDestinationPoint( int r , int c); // check for D tile

// Place or remove a safety tile at a position (for mouse editing)
// Returns true if successful
// safety tile ko lagata ya remove karta hai
bool toggleSafetyTile( int r , int c);

#endif
