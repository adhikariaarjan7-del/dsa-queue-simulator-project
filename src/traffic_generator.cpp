#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <unistd.h>

using namespace std;

string generateVehicleID() {
    string id = "";
    id += char('A' + rand() % 26);
    id += char('A' + rand() % 26);
    id += char('0' + rand() % 10);
    return id;
}
string generateLane() {
    char roads[] = {'A','B','C','D'};
    int lane = rand() % 3 + 1;

    string laneID = "";
    laneID += roads[rand() % 4];
    laneID += "L";
    laneID += char('0' + lane);
    return laneID;
}


int main() {
   ofstream file("data/vehicles.data", ios::app);
if(!file){
    cout << "Error opening file!" << endl;
    return 1;
}

