#include <iostream>
#include <queue>
#include <string>
#include <fstream>
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
    string line;

    while(getline(file, line)) {
        string lane = line.substr(line.find(":") + 1);
        if(lane=="AL1") AL1.push(line);
        else if(lane=="AL2") AL2.push(line);
        else if(lane=="AL3") AL3.push(line);
        else if(lane=="BL1") BL1.push(line);
        else if(lane=="BL2") BL2.push(line);
        else if(lane=="BL3") BL3.push(line);
        else if(lane=="CL1") CL1.push(line);
        else if(lane=="CL2") CL2.push(line);
        else if(lane=="CL3") CL3.push(line);
        else if(lane=="DL1") DL1.push(line);
        else if(lane=="DL2") DL2.push(line);
        else if(lane=="DL3") DL3.push(line);
    }
}

        

/*
 Calculate number of vehicles to serve in normal condition
 Formula: average of vehicles in normal lanes
*/
int calculateNormalServe() {
    int total = BL2.size() + CL2.size() + DL2.size();
    return total / 3;
}

int main() {
    loadVehicles();

    /*
     Priority lane logic:
     AL2 becomes priority when more than 10 vehicles
     It is served until vehicles become less than 5
    */
    if(AL2.size() > 10) {
        while(AL2.size() >= 5) {
            AL2.pop();
        }
    }
    else {
        int serve = calculateNormalServe();
        for(int i = 0; i < serve; i++) {
            if(!BL2.empty()) BL2.pop();
            if(!CL2.empty()) CL2.pop();
            if(!DL2.empty()) DL2.pop();
        }
    }

    return 0;
}
