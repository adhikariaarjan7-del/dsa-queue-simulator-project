#include <iostream>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

queue<string> AL1, AL2, AL3;
queue<string> BL1, BL2, BL3;
queue<string> CL1, CL2, CL3;
queue<string> DL1, DL2, DL3;



void loadVehicles()
 {
    ifstream file("data/vehicles.data");
    string line;
    while(getline(file, line)) {
        string lane = line.substr(line.find(":") + 1);
        if(lane=="AL2") AL2.push(line);
        else if(lane=="BL2") BL2.push(line);
        else if(lane=="CL2") CL2.push(line);
        else if(lane=="DL2") DL2.push(line);
    }
}



int main() {
    cout << "Simulator started" << endl;
    return 0;
}
