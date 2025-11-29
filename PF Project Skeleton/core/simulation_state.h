#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================
#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_TRAINS 100
#define MAX_SWITCHES 26

// DIRECTIONS
#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3

// WEATHER TYPES
#define WEATHER_NORMAL 0
#define WEATHER_RAIN   1
#define WEATHER_FOG    2

// SWITCH MODES
#define MODE_PER_DIR 0
#define MODE_GLOBAL  1
// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------
extern int LevelNumRows;              
extern int LevelNumCols;              
extern char TheGrid[MAX_ROWS][MAX_COLS]; 

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
extern int TotalScheduledTrains;       
extern int TrainSpawnTicks[MAX_TRAINS];  
extern int TrainStartCol[MAX_TRAINS];    
extern int TrainStartRow[MAX_TRAINS];    
extern int TrainStartDir[MAX_TRAINS];    
extern int TrainColorCode[MAX_TRAINS];   
extern bool TrainIsActive[MAX_TRAINS];   
extern int TrainCurrentCol[MAX_TRAINS];
extern int TrainCurrentRow[MAX_TRAINS];
extern int TrainCurrentDir[MAX_TRAINS];
extern int TrainNextCol[MAX_TRAINS];
extern int TrainNextRow[MAX_TRAINS];
extern int TrainNextDir[MAX_TRAINS];
extern int TrainState[MAX_TRAINS]; // 0 for scheduled 1 for active 2 for arrived 3 for crashed

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------
extern bool SwitchExists[MAX_SWITCHES];       
extern int SwitchCurrentState[MAX_SWITCHES];  //(0 or 1)
extern int SwitchLogicMode[MAX_SWITCHES];    //(0=PerDir, 1=Global)
extern int SwitchFlipThresholds[MAX_SWITCHES][4]; 
extern int SwitchCounters[MAX_SWITCHES][4];
extern bool SwitchFlipQueue[MAX_SWITCHES];

// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
extern int GameSeed;      
extern int GameWeather;
extern int CurrentTick;    

// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
