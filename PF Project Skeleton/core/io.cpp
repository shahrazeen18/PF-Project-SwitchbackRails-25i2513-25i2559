#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ---------------------------filename---------------------------------------
bool loadLevelFile(const char* filename) {
    // 1. Reset Global State
    initializeSimulationState();

    // 2. Open File
    FILE* file = fopen(filename, "r");
    if (!file) {   //If Error
        printf("Error: Not opening");
        return false;
    }

    char key[64];       // Buffer for single words
    int mode = 0;       // 0=Header, 1=Switches, 2=Trains

    // 3. Read Word by Word
    while (fscanf(file, "%s", key) == 1) {

        // KEYWORD DETECTION 
        
        if (strcmp(key, "NAME:") == 0) {
            char c;
            while ((c = fgetc(file)) != '\n' && c != EOF);            
            continue;
        }
        if (strcmp(key, "ROWS:") == 0) { 
            fscanf(file, "%d", &LevelNumRows); 
            continue; 
        }
        if (strcmp(key, "COLS:") == 0) { 
            fscanf(file, "%d", &LevelNumCols); 
            continue; 
        }
        if (strcmp(key, "SEED:") == 0) {
            fscanf(file, "%d", &GameSeed);
            srand(GameSeed); continue; 
        }
        if (strcmp(key, "WEATHER:") == 0) {
            char w[32]; 
            fscanf(file, "%s", w);
            if (strcmp(w, "RAIN") == 0) 
                GameWeather = WEATHER_RAIN;
            else if(strcmp(w, "FOG") == 0)
                GameWeather = WEATHER_FOG;
            else 
                GameWeather = WEATHER_NORMAL;
            continue;
        }
        if (strcmp(key, "MAP:") == 0) {
            // Consume the newline after "MAP:"
            fgetc(file); 

            for (int r = 0; r < LevelNumRows; r++) {
                char buffer[256];
                int idx = 0;
                char c;
                
                // Read line manually
                while ((c = fgetc(file)) != '\n' && c != EOF) {
                    if (idx < 255) buffer[idx++] = c;
                }
                buffer[idx] = '\0';

                // Check if this line is actually "SWITCHES:" or "TRAINS:"
                // We check the first few letters manually.
                bool isKeyword = false;
                if (buffer[0] == 'S' && buffer[1] == 'W' && buffer[2] == 'I' && buffer[3] == 'T') isKeyword = true;
                if (buffer[0] == 'T' && buffer[1] == 'R' && buffer[2] == 'A' && buffer[3] == 'I') isKeyword = true;
            
                if (isKeyword) {
                    if (buffer[0] == 'S') mode = 1; // Switches
                    else mode = 2; // Trains
                    break; 
                }

                // If it's a valid map line, copy it to the grid
                for (int col = 0; col < LevelNumCols; col++) {
                    if (col < idx) {
                        char ch = buffer[col];
                        // Filter visible characters only
                        if (ch >= 32 && ch <= 126) {
                            TheGrid[r][col] = ch;
                        } else {
                            TheGrid[r][col] = ' ';
                        }
                    } else {
                        TheGrid[r][col] = ' ';
                    }
                }
            }
            continue;
        }

        if (strcmp(key, "SWITCHES:") == 0) {
            mode = 1;
            continue;
        }
        if (strcmp(key, "TRAINS:") == 0) {
            mode = 2;
            continue;
        }

        //DATA PARSING

        // Parse Switch
        if (mode == 1) {
            // key is the switch letter
            if( key[0] >= 'A' && key[0] <= 'Z' && key[1] == '\0') {
                char letter = key[0];
                int idx = letter - 'A';

                if (idx >= 0 && idx < MAX_SWITCHES) {
                    SwitchExists[idx] = true;

                    char modeStr[32];
                    fscanf(file, "%s", modeStr);
                    
                    SwitchLogicMode[idx] = (strcmp(modeStr, "GLOBAL") == 0) ? MODE_GLOBAL : MODE_PER_DIR;
                    fscanf(file, "%d", &SwitchCurrentState[idx]);
                    
                    for(int k=0; k<4; k++) {
                        fscanf(file, "%d", &SwitchFlipThresholds[idx][k]);
                    }
                    
                    char c;
                    while ((c = fgetc(file)) != '\n' && c != EOF); 
                }
            }
        }
        else if (mode == 2) {
            // 'key' is the Spawn Tick
            if (TotalScheduledTrains < MAX_TRAINS) {
                // Ensure key is a digit before converting
                if (key[0] >= '0' && key[0] <= '9') {
                    TrainSpawnTicks[TotalScheduledTrains] = atoi(key);
                    
                    fscanf(file, "%d %d %d %d", 
                        &TrainStartCol[TotalScheduledTrains], 
                        &TrainStartRow[TotalScheduledTrains],
                        &TrainStartDir[TotalScheduledTrains], 
                        &TrainColorCode[TotalScheduledTrains]);

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
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {
       // Open files in "w" (write) mode to clear them
    FILE* fTrace = fopen("out/trace.csv", "w");
    if (fTrace) {
        fprintf(fTrace, "Tick,TrainID,X,Y,Direction,State\n");
        fclose(fTrace);
    }

    FILE* fSwitches = fopen("out/switches.csv", "w");
    if (fSwitches) {
        fprintf(fSwitches, "Tick,Switch,Mode,State\n");
        fclose(fSwitches);
    }

    FILE* fSignals = fopen("out/signals.csv", "w");
    if (fSignals) {
        fprintf(fSignals, "Tick,Switch,Signal\n");
        fclose(fSignals);
    }
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace(int tick, int trainId, int x, int y, int dir, const char *state) {
    FILE* f = fopen("out/trace.csv", "a"); // "a" = append
    if (f) {
        // Direction names for readability (Optional)
        const char* dirName = "UP";
        if (dir == 1) dirName = "RIGHT";
        else if (dir == 2) dirName = "DOWN";
        else if (dir == 3) dirName = "LEFT";

        fprintf(f, "%d,%d,%d,%d,%s,%s\n", tick, trainId, x, y, dirName, state);
        fclose(f);
    }
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState(int tick, char switchId, const char *mode, int state) {
    FILE* f = fopen("out/switches.csv", "a");
    if (f) {
        // State: 0 = Straight (usually), 1 = Turn
        const char* stateStr = (state == 0) ? "Straight" : "Turn";
        fprintf(f, "%d,%c,%s,%s\n", tick, switchId, mode, stateStr);
        fclose(f);
    }
}
// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState(int tick, char switchId, const char *color) {
    FILE* f = fopen("out/signals.csv", "a");
    if (f) {
        fprintf(f, "%d,%c,%s\n", tick, switchId, color);
        fclose(f);
    }
}
// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
    FILE* f = fopen("out/metrics.txt", "w");
    if (f) {
        fprintf(f, "SIMULATION METRICS\n");
        fprintf(f, "==================\n");
        fprintf(f, "Total Trains Scheduled: %d\n", TotalScheduledTrains);
        // You can add more global counters here later (like TotalCrashes)
        fprintf(f, "Simulation Ended.\n");
        fclose(f);
    }
}

void printGrid() {
   
   for(int r = 0; r < LevelNumRows; r++) {
     for(int c = 0; c < LevelNumCols; c++) {

        bool trainHere = false;
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