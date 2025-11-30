#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
    
    for(int i = 0; i < TotalScheduledTrains; i++) {
    if (TrainState[i] == 1) { 
        int r = TrainCurrentRow[i];
        int c = TrainCurrentCol[i];

        if (isSwitchTile(r, c)) {
            int swIdx = getSwitchIndex(r, c);
            
            // Logic: Entry direction is opposite of current facing
            int currentDir = TrainCurrentDir[i];
            int entryDir = (currentDir + 2) % 4; 

            if (SwitchLogicMode[swIdx] == MODE_GLOBAL) {
                SwitchCounters[swIdx][0]++;
            } else {
                SwitchCounters[swIdx][entryDir]++;
              }
         
            }
    
        }

    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    for (int i = 0; i < MAX_SWITCHES; i++)
    {
        if (!SwitchExists[i])
        {
            continue;
        }
        bool shouldFlip = false;
            if (SwitchLogicMode[i] == MODE_GLOBAL)
            {
                int k = SwitchFlipThresholds[i][0];
                if (k> 0 && SwitchCounters [i][0] >=k)
                {
                    shouldFlip = true;
                }
                
            }
            else{
                // check four directions
                for (int dir = 0; dir < 4; dir++)
                {
                    int k = SwitchFlipThresholds[i][dir];
                    if (k > 0 && SwitchCounters [i][dir] >= k)
                    {
                        shouldFlip = true;
                    }
                    
                }
                
            }
            if (shouldFlip){
                SwitchFlipQueue [i] = true ;
            }
    }
    
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
    for (int i = 0; i < MAX_SWITCHES; i++) {
        if (SwitchFlipQueue[i]) {
         if (SwitchCurrentState[i] == 0) {
                SwitchCurrentState[i] = 1;
            } else {
                SwitchCurrentState[i] = 0;
            }
            char switchName = 'A' + i;
            const char* modeStr = (SwitchLogicMode[i] == MODE_GLOBAL) ? "GLOBAL" : "PER_DIR";

            logSwitchState(CurrentTick, switchName, modeStr, SwitchCurrentState[i]);

             for (int k = 0; k < 4; k++) {
                SwitchCounters[i][k] = 0;
            }
              SwitchFlipQueue[i] = false;
}
}
}
// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() { for (int i = 0; i < MAX_SWITCHES; i++) {
        if (!SwitchExists[i]) continue;
        
        // Default Signal is green
        const char* color = "GREEN";
        char switchName = 'A' + i;

        // If a train is on the switch, it's RED 
        for (int t = 0; t < TotalScheduledTrains; t++) {
            if (TrainState[t] == 1) { // Active
                int r = TrainCurrentRow[t];
                int c = TrainCurrentCol[t];
                if (getSwitchIndex(r, c) == i) {
                    color = "RED";
                    break;
                }
            }
        }
        
        // Log the signal state
        logSignalState(CurrentTick, switchName, color);
    }
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState() {

}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection() {
    return 0;
}
