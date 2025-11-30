#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds( int x , int y ) {
    return x >= 0 && x< LevelNumRows && y>=0 && y< LevelNumCols ;
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile( int r, int c ) {
    // 1. Safety Check
    if (!isInBounds(r, c)) return false;
    
    // 2. Get the character
    char tile = TheGrid[r][c];

    // 3. Logic Check
    // Standard tracks
    if (tile == '-' || tile == '|' || tile == '/' || tile == '\\' || tile == '+') return true;
    
    // Stations and Safety
    if (tile == 'S' || tile == 'D' || tile == '=') return true;
    
    // Switches (A-Z)
    if (tile >= 'A' && tile <= 'Z') return true;
    
    return false;
}

// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile( int r , int c) {
    if (!isInBounds(r, c)) return false;
    char tile = TheGrid[r][c];
    if (tile == 'S' || tile == 'D') {
    return false; 
}
    return (tile >= 'A' && tile <= 'Z');
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(int r , int c) {

    if (!isInBounds(r, c)) 
        return -1;
    char tile = TheGrid[r][c];
    if (tile >= 'A' && tile <= 'Z') {
        return tile - 'A';
    }
    return -1;

}

// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint( int r , int c) {
    if (!isInBounds(r, c)) return false;
    return TheGrid[r][c] == 'S';
}



// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint( int r , int c) {
    if (!isInBounds(r, c)) return false;
    return TheGrid[r][c] == 'D';
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile( int r , int c ) {
     if (!isInBounds(r, c)) return false;
    
    char current = TheGrid[r][c];

    // 1. If it is currently a Safety Tile, change it into a normal tile otherwise when you toggle normal tile turns to safety.
    if (current == '=') {
        
        TheGrid[r][c] = '-';
        return true;
    }
    
    if (current == '-' || current == '|') {
        TheGrid[r][c] = '=';
        return true;
    }
    // safety tile is placed on only normal track

    return false; // One cannot place safety tile on switches or curves or empty space or others
}

