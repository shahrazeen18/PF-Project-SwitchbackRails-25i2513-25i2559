#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
    initializeLogFiles();
    CurrentTick = 0;
    updateSignalLights();
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {
   spawnTrainsForTick();
   
   determineAllRoutes();
   
   moveAllTrains();
   
   updateSwitchCounters();
   
   queueSwitchFlips();
   
   applyDeferredFlips();

   checkArrivals();
   
   updateSignalLights();
   for (int i = 0; i < TotalScheduledTrains; i++) {
        if (TrainState[i] == 1) { // 1 = Active
            const char* stateStr = "RUNNING";
            logTrainTrace(CurrentTick, i, 
                          TrainCurrentCol[i], 
                          TrainCurrentRow[i], 
                          TrainCurrentDir[i], 
                          stateStr);
        }
        else if (TrainState[i] == 2) {
        }
    }
    CurrentTick++;
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {
    for(int i=0; i< TotalScheduledTrains; i++) {
        if(TrainState[i] == 0 || TrainState[i] == 1) {
         return false;
        }
    }
 return true;
}