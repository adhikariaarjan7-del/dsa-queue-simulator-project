#include <iostream>
using namespace std;

int main() {
    
    SDL_Window* window = SDL_CreateWindow(
    "Traffic Junction",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800, 600,
    SDL_WINDOW_SHOWN
);


     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL Init Failed" << endl;
        return 1;
    }
    return 0;
}