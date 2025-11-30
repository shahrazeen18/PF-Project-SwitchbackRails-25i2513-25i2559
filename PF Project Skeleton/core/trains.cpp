#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>


void getDelta(int dir, int &dr, int &dc) {
    dr = 0; dc = 0;
    if (dir == DIR_UP) dr = -1;
    else if (dir == DIR_RIGHT) dc = 1;
    else if (dir == DIR_DOWN) dr = 1;
    else if (dir == DIR_LEFT) dc = -1;
}


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
        if (TrainSpawnTicks[i] == CurrentTick && TrainState[i] == 0) { //if the current tick is spawn tick then spawn it
            
            int r = TrainStartRow[i];
            int c = TrainStartCol[i];
           
        bool occupied = false;
           
                for (int j = 0; j < TotalScheduledTrains; j++) {
                if (TrainState[j] == 1 && TrainCurrentRow[j] == r && TrainCurrentCol[j] == c) // Don't check if self                    
                occupied = true;
                break;
                }
            
            if (occupied) {
                // If it is blocked, wait until next tick
                TrainSpawnTicks[i]++; 
            } else {
                // spawn the train
                TrainIsActive[i] = true;
                TrainState[i] = 1;
                TrainCurrentRow[i] = r;
                TrainCurrentCol[i] = c;
                TrainCurrentDir[i] = TrainStartDir[i];
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
bool determineNextPosition(int i) { 
    int r = TrainCurrentRow[i];
    int c = TrainCurrentCol[i];
    int dir = TrainCurrentDir[i];
    int dr = 0, dc = 0;
    if (dir == DIR_UP) dr = -1;
    else if (dir == DIR_RIGHT) dc = 1;
    else if (dir == DIR_DOWN) dr = 1;
    else if (dir == DIR_LEFT) dc = -1;

    int nextR = r + dr;
    int nextC = c + dc;
    int nextDir = dir;
    // Handles Crossing
    if (isInBounds(nextR, nextC)) {
        char tile = TheGrid[nextR][nextC];
    if (tile == '+') {
            nextDir = getSmartDirectionAtCrossing(nextR, nextC, dir);
        } else {
            // Handles Curves and Switches
            nextDir = getNextDirection(nextR,nextC, dir, tile);
        }
    }
      // Stores planned move
    TrainNextRow[i] = nextR;
    TrainNextCol[i] = nextC;
    TrainNextDir[i] = nextDir;

    return true;
}

// ----------------------------------------------------------------------------
// GET N+EXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int r, int c, int dir, char tile) { 
     // Curve 
    if (tile == '/') {
        if (dir == DIR_RIGHT) return DIR_UP; // Right -> Up
        if (dir == DIR_DOWN) return DIR_LEFT; // Down -> Left
        if (dir == DIR_LEFT) return DIR_DOWN; // Left -> Down
        if (dir == DIR_UP) return DIR_RIGHT;// Up -> Right
    }
    // Curve \ (backslash)
    else if (tile == '\\') {
        if (dir == DIR_RIGHT) return DIR_DOWN; // Right -> Down
        if (dir == DIR_UP) return DIR_LEFT; // Up -> Left
        if (dir == DIR_LEFT) return DIR_UP; // Left -> Up
        if (dir == DIR_DOWN) return DIR_RIGHT; // Down -> Right
    }
    // Switches (A-Z)
    else if (tile >= 'A' && tile <= 'Z') {
        int idx = tile - 'A';
        int state = SwitchCurrentState[idx];
        
        // If State is 1 (Turn), we mimic a right-hand turn relative to entry
        // This is a simplification. Real logic depends on track layout.
        if (state == 0) {
            // Simple logic: Turn Right relative to movement
            // Up(0)->Right(1), Right(1)->Down(2), Down(2)->Left(3), Left(3)->Up(0)
            return dir ;
        }
        else{
            
            int rightDir = (dir + 1) % 4;
            int dr, dc;
            getDelta(rightDir, dr, dc);
            
            if (isTrackTile(r + dr, c + dc)) {
                return rightDir;
            }
            int leftDir = (dir + 3) % 4;
            getDelta(leftDir, dr, dc);
            if(isTrackTile(r + dr, c + dc)) {
            return leftDir;
        }
            return dir;
        }

    }
    return dir;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int r, int c, int currentDir) { 
     int destR = -1, destC = -1;
    bool found = false; // Flag to break nested loops
    // Simple search for any 'D'
    for(int i = 0; i < LevelNumRows; i++) {
        for(int j = 0; j < LevelNumCols; j++) {
            if(TheGrid[i][j] == 'D') {
                destR = i; 
                destC = j;
                found = true;
                break; // Breaks Inner Loop
            }
        }
        if (found) 
        break;
    }
    if (destR == -1)
     return currentDir;

    // Checks 3 directions: Straight, Left, Right
    // 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT
    
    int bestDir = currentDir;
    int minDist = 10000;

    // Candidates: Straight, Left Turn, Right Turn
    int candidates[3];
    candidates[0] = currentDir;
    candidates[1] = (currentDir + 3) % 4; // Left
    candidates[2] = (currentDir + 1) % 4; // Right

    for(int k=0; k<3; k++) {
        int d = candidates[k];
        int dr, dc;
        getDelta(d, dr, dc);

    if(!isTrackTile(r +dr, c + dc))
        continue;
    


        // Manhattan Distance calculation
        int dist = abs(destR - (r + dr)) + abs(destC - (c + dc));
        
        if (dist < minDist) {
            minDist = dist;
            bestDir = d;
        }
    }

    return bestDir;
}


// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainState[i] == 1) { // Active
            determineNextPosition(i);
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    detectCollisions();
for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainState[i] == 1) {
            TrainCurrentRow[i] = TrainNextRow[i];
            TrainCurrentCol[i] = TrainNextCol[i];
            TrainCurrentDir[i] = TrainNextDir[i];
         if (isSwitchTile(TrainCurrentRow[i], TrainCurrentCol[i])) {
            }
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainState[i] != 1) continue;

     for (int j = i + 1; j < TotalScheduledTrains; j++) {
       if (TrainState[j] != 1) continue;
        if (TrainNextRow[i] == TrainNextRow[j] && TrainNextCol[i] == TrainNextCol[j]) {
            TrainNextRow[j] = TrainCurrentRow[j];
            TrainNextCol[j] = TrainCurrentCol[j];
            }
    else if (TrainNextRow[i] == TrainCurrentRow[j] && TrainNextCol[i] == TrainCurrentCol[j] &&
      TrainNextRow[j] == TrainCurrentRow[i] && TrainNextCol[j] == TrainCurrentCol[i]) {
         TrainNextRow[j] = TrainCurrentRow[j];
                TrainNextCol[j] = TrainCurrentCol[j];
            }
        }
    }
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
        for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainState[i] == 1) {
            int r = TrainCurrentRow[i];
            int c = TrainCurrentCol[i];

            // Checks Arrival
            if (TheGrid[r][c] == 'D') {
                TrainState[i] = 2; // Arrived
                TrainIsActive[i] = false;
            }
            // Checks Crash 
            else if (!isInBounds(r, c) || !isTrackTile(r , c) ) 
            {
                TrainState[i] = 3; // Crashed
                TrainIsActive[i] = false;
            }
        }
    }
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
