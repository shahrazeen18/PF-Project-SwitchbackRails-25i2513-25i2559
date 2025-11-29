#ifndef IO_H
#define IO_H

// ============================================================================
// IO.H - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LEVEL LOADING
// ----------------------------------------------------------------------------
// Load a .lvl file.
bool loadLevelFile(const char *filename);

// ----------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------
// Create/clear log files.
void initializeLogFiles();

// Append train movement to trace.csv.
void logTrainTrace(int tick, int trainId, int x, int y, int dir, const char *state);

// Append switch state to switches.csv.
void logSwitchState(int tick, char switchId, const char *mode, int state);

// Append signal state to signals.csv.
void logSignalState(int tick, char switchId, const char *color);

// Write final metrics to metrics.txt.
void writeMetrics();

// printGrid function to print character arrays 
void printGrid();



#endif
