#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

string generateVehicleID() {
    string id = "";
    id += char('A' + rand() % 26);
    id += char('A' + rand() % 26);
    id += char('0' + rand() % 10);
    return id;
}

int main() {
    srand(time(0));
    cout << generateVehicleID() << endl;
    return 0;
}


