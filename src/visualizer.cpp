#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/*
 This program only VISUALIZES traffic.
 It does NOT decide priority or queue logic.
*/

// structure to store vehicle info
struct Vehicle {
    string id;
    string lane;
};

// read vehicles from file
vector<Vehicle> loadVehicles() {
    vector<Vehicle> vehicles;
    ifstream file("data/vehicles.data");

    string line;
    while (getline(file, line)) {
        int pos = line.find(":");
        if (pos != string::npos) {
            Vehicle v;
            v.id = line.substr(0, pos);
            v.lane = line.substr(pos + 1);
            vehicles.push_back(v);
        }
    }
    return vehicles;
}

int main(int argc, char* argv[]) {

    // start SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Traffic Junction Visualizer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    // traffic light
    bool green = true;
    Uint32 lastSwitch = SDL_GetTicks();

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // change light every 4 seconds
        if (SDL_GetTicks() - lastSwitch > 4000) {
            green = !green;
            lastSwitch = SDL_GetTicks();
        }

        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw roads
        SDL_Rect roadH = {100, 250, 600, 100};
        SDL_Rect roadV = {350, 50, 100, 500};
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderFillRect(renderer, &roadH);
        SDL_RenderFillRect(renderer, &roadV);

        // draw traffic light
        SDL_Rect light = {370, 200, 30, 30};
        if (green)
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &light);

        // load vehicles from file
        vector<Vehicle> vehicles = loadVehicles();

        // draw vehicles as rectangles
        int y = 260;
        for (auto &v : vehicles) {
            SDL_Rect car = {150, y, 30, 20};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &car);
            y += 25;
            if (y > 330) break;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
