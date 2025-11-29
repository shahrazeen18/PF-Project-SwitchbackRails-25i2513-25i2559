#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i = 0; i < TotalScheduledTrains ; i++) // check if train is scheduled for now.

    {
        if (TrainSpawnTicks[i] == CurrentTick) { //if the current tick is spawn tick then spawn it
            
            int r = TrainStartRow[i];
            int c = TrainStartCol[i];
            
        
        bool occupied = false;
           
                for (int j = 0; j < TotalScheduledTrains; j++) {
                if (i == j) continue; // Don't check if self
                if (TrainIsActive[j] && TrainStartRow[j] == r && TrainStartCol[j] == c) {
                    occupied = true;
                    break;
                }
            }
            
            if (occupied) {
                // If it is blocked, wait until next tick
                TrainSpawnTicks[i]++; 
            } else {
                // spawn the train
                TrainIsActive[i] = true;
                // We assume the train is now active at (r,c)
            }
        }
    }
}

        


// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition() { return false;
}

// ----------------------------------------------------------------------------
// GET N+EXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection() { return 0;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing() { return 0;
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
}
