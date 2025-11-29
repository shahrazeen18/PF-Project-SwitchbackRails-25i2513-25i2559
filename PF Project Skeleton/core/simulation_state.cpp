#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int LevelNumRows = 0;
int LevelNumCols = 0;
char TheGrid[MAX_ROWS][MAX_COLS];

// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
int TotalScheduledTrains = 0;
int TrainSpawnTicks[MAX_TRAINS];
int TrainStartCol[MAX_TRAINS];
int TrainStartRow[MAX_TRAINS];
int TrainStartDir[MAX_TRAINS];
int TrainColorCode[MAX_TRAINS];
bool TrainIsActive[MAX_TRAINS];
int TrainCurrentCol[MAX_TRAINS];
int TrainCurrentRow[MAX_TRAINS];
int TrainCurrentDir[MAX_TRAINS];
int TrainNextCol[MAX_TRAINS];
int TrainNextRow[MAX_TRAINS];
int TrainNextDir[MAX_TRAINS];
int TrainState[MAX_TRAINS];




// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
bool SwitchExists[MAX_SWITCHES];
int SwitchCurrentState[MAX_SWITCHES];
int SwitchLogicMode[MAX_SWITCHES];
int SwitchFlipThresholds[MAX_SWITCHES][4];
int SwitchCounters[MAX_SWITCHES][4];
bool SwitchFlipQueue[MAX_SWITCHES];
// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int GameSeed = 0;
int GameWeather = 0;
int CurrentTick = 0;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() {
    LevelNumRows = 0;
    LevelNumCols = 0;
    TotalScheduledTrains = 0;
    GameSeed = 0;
    GameWeather = WEATHER_NORMAL;
    CurrentTick = 0;
    
    // Clears the Map
  for (int i = 0; i < MAX_SWITCHES; i++) {
        SwitchExists[i] = false;
        SwitchCurrentState[i] = 0;
        SwitchLogicMode[i] = 0;
        for (int k = 0; k < 4; k++) {
            SwitchFlipThresholds[i][k] = 0;
        }
    }
}
