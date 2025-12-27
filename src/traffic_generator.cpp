#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <windows.h>

using namespace std;

string generateVehicleID() {
    string id = "";
    id += char('A' + rand() % 26);
    id += char('A' + rand() % 26);
    id += char('0' + rand() % 10);
    return id;
}

string generateLane() {
    char roads[] = {'A', 'B', 'C', 'D'};
    int lane = rand() % 3 + 1;

    string laneID = "";
    laneID += roads[rand() % 4];
    laneID += "L";
    laneID += char('0' + lane);

    return laneID;
}

int main() {
    cout << "GENERATOR STARTED FROM HERE" << endl;

    srand((unsigned int)time(NULL));

   ofstream file("data/vehicles.data", ios::app);

    if (!file) {
        cout << "Error opening vehicles.data!" << endl;
        return 1;
    }

    cout << "Traffic generator started..." << endl;

    while (true) {
        string vehicle = generateVehicleID();
        string lane = generateLane();

        file << vehicle << ":" << lane << endl;
        file.flush();

        cout << "Generated: " << vehicle << " in " << lane << endl;
        Sleep(1000);  // 1 second delay
    }
}
