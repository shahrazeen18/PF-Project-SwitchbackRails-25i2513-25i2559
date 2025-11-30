#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile(const char* filename) {
    // 1. Reset Global State
    initializeSimulationState();

    // 2. Open File
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return false;
    }

    char key[128];
    int mode = 0; // 0=Headers, 1=Switches, 2=Trains

    // 3. Read Word by Word
    while (fscanf(file, "%s", key) == 1) {
        
        // --- HEADERS ---
        if (strcmp(key, "NAME:") == 0) {
            char c; while ((c = fgetc(file)) != '\n' && c != EOF); 
            continue; 
        }
        if (strcmp(key, "ROWS:") == 0) { fscanf(file, "%d", &LevelNumRows); continue; }
        if (strcmp(key, "COLS:") == 0) { fscanf(file, "%d", &LevelNumCols); continue; }
        if (strcmp(key, "SEED:") == 0) { fscanf(file, "%d", &GameSeed); srand(GameSeed); continue; }
        if (strcmp(key, "WEATHER:") == 0) {
            char w[32]; fscanf(file, "%s", w);
            if (strcmp(w, "RAIN") == 0) GameWeather = WEATHER_RAIN;
            else if (strcmp(w, "FOG") == 0) GameWeather = WEATHER_FOG;
            else GameWeather = WEATHER_NORMAL;
            continue;
        }
        
        // --- MAP SECTION ---
        if (strcmp(key, "MAP:") == 0) {
            // Consume the newline immediately after "MAP:"
            char c; 
            while ((c = fgetc(file)) != '\n' && c != EOF);

            for (int r = 0; r < LevelNumRows; r++) {
                char buffer[512];
                int idx = 0;
                
                // Read the entire line char-by-char
                while ((c = fgetc(file)) != '\n' && c != EOF) {
                    if (idx < 511) buffer[idx++] = c;
                }
                buffer[idx] = '\0'; 

                // Check if we hit the next section (SWITCHES or TRAINS)
                // This handles cases where map lines are fewer than expected
                bool isSwitcheHeader = (idx >= 8 && buffer[0] == 'S' && buffer[1] == 'W' && buffer[2] == 'I');
                bool isTrainHeader   = (idx >= 6 && buffer[0] == 'T' && buffer[1] == 'R' && buffer[2] == 'A');

                if (isSwitcheHeader) {
                    mode = 1; 
                    break;    
                }
                if (isTrainHeader) {
                    mode = 2; 
                    break;    
                }

                // Fill the grid row
                for (int col = 0; col < LevelNumCols; col++) {
                    if (col < idx) {
                        char val = buffer[col];
                        // Filter out invisible chars 
                        if (val >= 32 && val <= 126) {
                            TheGrid[r][col] = val;
                        } else {
                            TheGrid[r][col] = ' ';
                        }
                    } else {
                        TheGrid[r][col] = ' '; // Fill short lines with space
                    }
                }
            }
            continue;
        }
        
        // --- MODE SWITCHING ---
        if (strcmp(key, "SWITCHES:") == 0) { mode = 1; continue; }
        if (strcmp(key, "TRAINS:") == 0) { mode = 2; continue; }

        // --- PARSING DATA ---
        if (mode == 1) {
            // Check if key is a valid switch letter (A-Z)
            if (key[0] >= 'A' && key[0] <= 'Z' && key[1] == '\0') {
                int idx = key[0] - 'A';
                if (idx >= 0 && idx < MAX_SWITCHES) {
                    SwitchExists[idx] = true;
                    
                    char modeStr[32]; 
                    fscanf(file, "%s", modeStr);
                    
                    if (strcmp(modeStr, "GLOBAL") == 0) SwitchLogicMode[idx] = MODE_GLOBAL;
                    else SwitchLogicMode[idx] = MODE_PER_DIR;

                    fscanf(file, "%d", &SwitchCurrentState[idx]);
                    for(int k=0; k<4; k++) fscanf(file, "%d", &SwitchFlipThresholds[idx][k]);
                    
                    // Consume rest of line (labels)
                    char c; while ((c = fgetc(file)) != '\n' && c != EOF);
                }
            }
        }
        else if (mode == 2) {
            // Check if key is a digit (Spawn Tick)
            if (key[0] >= '0' && key[0] <= '9') {
                if (TotalScheduledTrains < MAX_TRAINS) {
                    TrainSpawnTicks[TotalScheduledTrains] = atoi(key);
                    
                    int rawCol, rawRow;
                    fscanf(file, "%d %d %d %d", 
                     &rawCol, &rawRow,
                     &TrainStartDir[TotalScheduledTrains], &TrainColorCode[TotalScheduledTrains]);

                    TrainStartCol[TotalScheduledTrains] = rawCol - 1;
                    TrainStartRow[TotalScheduledTrains] = rawRow - 1;


                    TrainIsActive[TotalScheduledTrains] = false;
                    TotalScheduledTrains++;
                }
            }
        }
    }

    fclose(file);
    return true;
}

// ----------------------------------------------------------------------------
// LOGGING FUNCTIONS
// ----------------------------------------------------------------------------
void initializeLogFiles() {
    FILE* fTrace = fopen("out/trace.csv", "w"); if(fTrace){fprintf(fTrace,"Tick,TrainID,X,Y,Direction,State\n"); fclose(fTrace);}
    FILE* fSwitches = fopen("out/switches.csv", "w"); if(fSwitches){fprintf(fSwitches,"Tick,Switch,Mode,State\n"); fclose(fSwitches);}
    FILE* fSignals = fopen("out/signals.csv", "w"); if(fSignals){fprintf(fSignals,"Tick,Switch,Signal\n"); fclose(fSignals);}
}

void logTrainTrace(int tick, int trainId, int x, int y, int dir, const char *state) {
    FILE* f = fopen("out/trace.csv", "a");
    if(f){ fprintf(f,"%d,%d,%d,%d,%d,%s\n", tick, trainId, x, y, dir, state); fclose(f); }
}

void logSwitchState(int tick, char switchId, const char *mode, int state) {
    FILE* f = fopen("out/switches.csv", "a");
    if(f){ fprintf(f,"%d,%c,%s,%d\n", tick, switchId, mode, state); fclose(f); }
}

void logSignalState(int tick, char switchId, const char *color) {
    FILE* f = fopen("out/signals.csv", "a");
    if(f){ fprintf(f,"%d,%c,%s\n", tick, switchId, color); fclose(f); }
}

void writeMetrics() {
    FILE* f = fopen("out/metrics.txt", "w");
    if(f){ fprintf(f,"SIMULATION METRICS\nTotal Trains: %d\n", TotalScheduledTrains); fclose(f); }
}

// ----------------------------------------------------------------------------
// PRINT GRID (Terminal Output)
// ----------------------------------------------------------------------------
void printGrid() {
   printf("Tick: %d\n", CurrentTick);
   
   for(int r = 0; r < LevelNumRows; r++) {
     for(int c = 0; c < LevelNumCols; c++) {

        bool trainHere = false;
        // Check for Active Trains
        for(int i = 0; i < TotalScheduledTrains; i++) {
            if(TrainState[i] == 1 && TrainCurrentRow[i] == r && TrainCurrentCol[i] == c) {
                printf("%d", i % 10);
                trainHere = true;
                break;
            }
        }
        
        if(!trainHere) {
            printf("%c", TheGrid[r][c]);
        }
     }
      printf("\n");
    }
   printf("--------------------------\n");
}