#include "core/io.h"
#include "core/simulation.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./console_game <level_file_path>" << endl;
        return 1;
    }

    cout << "Loading level: " << argv[1] << "..." << endl;
    if (!loadLevelFile(argv[1])) {
        cout << "Error: Failed to load level file." << endl;
        return 1;
    }

    initializeSimulation();

    cout << "Simulation Loaded." << endl;
    cout << "Press [ENTER] to step forward one tick." << endl;
    cout << "Type 'q' and [ENTER] to quit." << endl;
    cout << "----------------------------------------" << endl;

    printGrid();

    string input;
    while (true) {
        getline(cin, input);

        if (input == "q" || input == "Q") {
            break;
        }

        simulateOneTick();
        printGrid();
    }

    cout << "Exiting simulation." << endl;
    return 0;
}
