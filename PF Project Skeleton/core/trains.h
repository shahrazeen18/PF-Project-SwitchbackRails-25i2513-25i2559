#ifndef TRAINS_H
#define TRAINS_H

// ============================================================================
// TRAINS.H - Train logic
// ============================================================================

// ----------------------------------------------------------------------------
// TRAIN SPAWNING
// ----------------------------------------------------------------------------
// Spawn trains scheduled for the current tick.
void spawnTrainsForTick();

// ----------------------------------------------------------------------------
// TRAIN ROUTING
// ----------------------------------------------------------------------------
// Compute routes for all trains (Phase 2).
void determineAllRoutes();

// Compute next position/direction for a train.
bool determineNextPosition(int trainId);

// Get next direction on entering a tile.
int getNextDirection(int r, int c, int currentDir, char tile);

// Choose best direction at a crossing.
int getSmartDirectionAtCrossing(int r, int c, int currentDir);

// ----------------------------------------------------------------------------
// TRAIN MOVEMENT
// ----------------------------------------------------------------------------
// Move trains and handle collisions (Phase 5).
void moveAllTrains();

// ----------------------------------------------------------------------------
// COLLISION DETECTION
// ----------------------------------------------------------------------------
// Detect trains targeting the same tile/swap/crossing.
void detectCollisions();

// ----------------------------------------------------------------------------
// ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
void checkArrivals();

// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply emergency halt in active zone.
void applyEmergencyHalt();

// Update emergency halt timer.
void updateEmergencyHalt();

#endif
