#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <algorithm> // for remove
using namespace std;

/*
 Each lane is represented using a queue
*/

// Road A
queue<string> AL1, AL2, AL3;
// Road B
queue<string> BL1, BL2, BL3;
// Road C
queue<string> CL1, CL2, CL3;
// Road D
queue<string> DL1, DL2, DL3;

/*
 Read vehicles from file and insert into correct lane
*/
void loadVehicles() {
    ifstream file("data/vehicles.data");
    if (!file.is_open()) {
        cout << "Error: vehicles.data not found!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Extract lane name after ":"
        string lane = line.substr(line.find(":") + 1);
        lane.erase(remove(lane.begin(), lane.end(), ' '), lane.end()); // remove spaces

        if (lane == "AL1") AL1.push(line);
        else if (lane == "AL2") AL2.push(line);
        else if (lane == "AL3") AL3.push(line);
        else if (lane == "BL1") BL1.push(line);
        else if (lane == "BL2") BL2.push(line);
        else if (lane == "BL3") BL3.push(line);
        else if (lane == "CL1") CL1.push(line);
        else if (lane == "CL2") CL2.push(line);
        else if (lane == "CL3") CL3.push(line);
        else if (lane == "DL1") DL1.push(line);
        else if (lane == "DL2") DL2.push(line);
        else if (lane == "DL3") DL3.push(line);
    }
}

/*
 Calculate number of vehicles to serve in normal condition
 Formula: average of vehicles in normal lanes
*/
size_t calculateNormalServe() {
    size_t total = BL2.size() + CL2.size() + DL2.size();
    return total / 3;
}

int main() {
    loadVehicles();

    // DEBUG: Show initial lane sizes
    cout << "Initial lane sizes:" << endl;
    cout << "AL2: " << AL2.size() << " | BL2: " << BL2.size()
         << " | CL2: " << CL2.size() << " | DL2: " << DL2.size() << endl;

    /*
     Priority lane logic:
     AL2 becomes priority when more than 10 vehicles
     It is served until vehicles become less than 5
    */
    if (AL2.size() > 10) {
        cout << "AL2 is priority lane. Serving vehicles..." << endl;
        while (AL2.size() >= 5 && !AL2.empty()) {
            AL2.pop();
        }
    } else {
        size_t serve = calculateNormalServe();
        cout << "Normal condition. Serving " << serve << " vehicles per lane..." << endl;
        for (size_t i = 0; i < serve; i++) {
            if (!BL2.empty()) BL2.pop();
            if (!CL2.empty()) CL2.pop();
            if (!DL2.empty()) DL2.pop();
        }
    }

    // DEBUG: Show lane sizes after serving
    cout << "Lane sizes after serving:" << endl;
    cout << "AL2: " << AL2.size() << " | BL2: " << BL2.size()
         << " | CL2: " << CL2.size() << " | DL2: " << DL2.size() << endl;

    return 0;
}
