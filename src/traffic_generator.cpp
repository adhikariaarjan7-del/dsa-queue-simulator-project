#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <unistd.h>

using namespace std;

/*
 This function creates a random vehicle ID
 Example: AB3
*/
string generateVehicleID() {
    string id = "";
    id += char('A' + rand() % 26);
    id += char('A' + rand() % 26);
    id += char('0' + rand() % 10);
    return id;
}

/*
 This function selects random road and lane
 Only A, B, C, D roads
 Example: AL1, BL2
*/
string generateLane() {
    char roads[] = {'A', 'B', 'C', 'D'};
    int lane = rand() % 3 + 1; // Lane 1 to 3

    string laneID = "";
    laneID += roads[rand() % 4];
    laneID += "L";
    laneID += char('0' + lane);

    return laneID;
}

int main() {
    srand((unsigned int)time(NULL));

    ofstream file("data/vehicles.data", ios::app);

    if (!file) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    cout << "Traffic generator started..." << endl;

    while (true) {
        string vehicle = generateVehicleID();
        string lane = generateLane();

        file << vehicle << ":" << lane << endl;
        cout << "Generated: " << vehicle << " in " << lane << endl;

        sleep(1); // wait 1 second
    }

    file.close();
    return 0;
}
