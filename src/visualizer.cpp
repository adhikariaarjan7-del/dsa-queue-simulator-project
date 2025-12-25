#include <iostream>
using namespace std;

int main() {
     if (SDL_Init(SDL_INIT_VIDEO) != 0) 
     {
        cout << "SDL Init Failed" << endl;
        return 1;
    }
    return 0;
}