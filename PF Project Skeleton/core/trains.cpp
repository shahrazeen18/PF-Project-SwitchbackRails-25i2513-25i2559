#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

void getDelta(int dir, int &dr, int &dc) {
    dr = 0; dc = 0;
    if (dir == DIR_UP) dr = -1;
    else if (dir == DIR_RIGHT) dc = 1;
    else if (dir == DIR_DOWN) dr = 1;
    else if (dir == DIR_LEFT) dc = -1;
}

int calculateDistance(int trainIdx) {
    int r = TrainCurrentRow[trainIdx];
    int c = TrainCurrentCol[trainIdx];
    int minDist = 99999;
    bool found = false;
    for (int i = 0; i < LevelNumRows; i++) {
        for (int j = 0; j < LevelNumCols; j++) {
            if (TheGrid[i][j] == 'D') {
                int dist = abs(i - r) + abs(j - c);
                if (dist < minDist) minDist = dist;
                found = true;
            }
        }
    }
    return found ? minDist : 0;
}

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainSpawnTicks[i] <= CurrentTick && TrainState[i] == 0) {
            
            int r = TrainStartRow[i];
            int c = TrainStartCol[i];
           
            bool blocked = false;
            for (int j = 0; j < TotalScheduledTrains; j++) {
                if (TrainState[j] == 1 && TrainCurrentRow[j] == r && TrainCurrentCol[j] == c) {
                    blocked = true;
                    break;
                }
            }
            
            if (blocked) {
                TrainSpawnTicks[i]++; 
            } else {
                TrainIsActive[i] = true;
                TrainState[i] = 1;
                TrainCurrentRow[i] = r;
                TrainCurrentCol[i] = c;
                TrainCurrentDir[i] = TrainStartDir[i];
                
                TrainNextRow[i] = r;
                TrainNextCol[i] = c;
                TrainNextDir[i] = TrainStartDir[i];
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
    
    int dr, dc;
    getDelta(dir, dr, dc);

    int nextR = r + dr;
    int nextC = c + dc;
    int nextDir = dir;

    if (isInBounds(nextR, nextC)) {
        char tile = TheGrid[nextR][nextC];
        if (tile == '+') {
            nextDir = getSmartDirectionAtCrossing(nextR, nextC, dir);
        } else {
            nextDir = getNextDirection(nextR, nextC, dir, tile);
        }
    }
    
    TrainNextRow[i] = nextR;
    TrainNextCol[i] = nextC;
    TrainNextDir[i] = nextDir;
    return true;
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int r, int c, int dir, char tile) { 
    if (tile == '/') {
        if (dir == DIR_RIGHT) return DIR_UP;
        if (dir == DIR_DOWN) return DIR_LEFT;
        if (dir == DIR_LEFT) return DIR_DOWN;
        if (dir == DIR_UP) return DIR_RIGHT;
    }
    else if (tile == '\\') {
        if (dir == DIR_RIGHT) return DIR_DOWN;
        if (dir == DIR_UP) return DIR_LEFT;
        if (dir == DIR_LEFT) return DIR_UP;
        if (dir == DIR_DOWN) return DIR_RIGHT;
    }
    else if (tile >= 'A' && tile <= 'Z') {
        int idx = tile - 'A';
        int state = SwitchCurrentState[idx];
        
        if (state == 0) {
            return dir;
        }
        else {
            int rightDir = (dir + 1) % 4;
            int dr, dc;
            getDelta(rightDir, dr, dc);
            
            if (isTrackTile(r + dr, c + dc)) {
                return rightDir;
            }
            
            int leftDir = (dir + 3) % 4;
            getDelta(leftDir, dr, dc);
            if (isTrackTile(r + dr, c + dc)) {
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
    int minDistFound = 99999;
    
    for(int i = 0; i < LevelNumRows; i++) {
        for(int j = 0; j < LevelNumCols; j++) {
            if(TheGrid[i][j] == 'D') {
                int d = abs(i - r) + abs(j - c);
                if (d < minDistFound) {
                    minDistFound = d;
                    destR = i; 
                    destC = j; 
                }
            }
        }
    }
    if (destR == -1) return currentDir;

    int bestDir = currentDir;
    int minDist = 99999;

    int candidates[3];
    candidates[0] = currentDir;
    candidates[1] = (currentDir + 3) % 4; 
    candidates[2] = (currentDir + 1) % 4; 

    for(int k=0; k<3; k++) {
        int d = candidates[k];
        int dr, dc;
        getDelta(d, dr, dc);
        
        if(!isTrackTile(r + dr, c + dc)) continue;

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
        if (TrainState[i] == 1) { 
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
            
            bool collision = false;

            if (TrainNextRow[i] == TrainNextRow[j] && TrainNextCol[i] == TrainNextCol[j]) {
                collision = true;
            }
            else if (TrainNextRow[i] == TrainCurrentRow[j] && TrainNextCol[i] == TrainCurrentCol[j] &&
                     TrainNextRow[j] == TrainCurrentRow[i] && TrainNextCol[j] == TrainCurrentCol[i]) {
                collision = true;
            }

            if (collision) {
                int distI = calculateDistance(i);
                int distJ = calculateDistance(j);

                if (distI > distJ) {
                    TrainNextRow[j] = TrainCurrentRow[j];
                    TrainNextCol[j] = TrainCurrentCol[j];
                } 
                else if (distJ > distI) {
                    TrainNextRow[i] = TrainCurrentRow[i];
                    TrainNextCol[i] = TrainCurrentCol[i];
                } 
                else {
                    TrainNextRow[j] = TrainCurrentRow[j];
                    TrainNextCol[j] = TrainCurrentCol[j];
                }
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

            if (TheGrid[r][c] == 'D') {
                TrainState[i] = 2; // Arrived
                TrainIsActive[i] = false;
            }
            else if (!isInBounds(r, c) || !isTrackTile(r, c)) {
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