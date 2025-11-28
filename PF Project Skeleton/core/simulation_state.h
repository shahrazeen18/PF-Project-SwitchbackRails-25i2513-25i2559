#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================
#define MAX_ROWS 30
#define MAX_COLS 60
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

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------
extern bool SwitchExists[MAX_SWITCHES];       
extern int SwitchCurrentState[MAX_SWITCHES];  //(0 or 1)
extern int SwitchlogicMode[MAX_SWITCHES];    //(0=PerDir, 1=Global)
extern int SwitchFlipThresholds[MAX_SWITCHES][4]; 

// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
extern int GameSeed;      
extern int GameWeather;    

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
