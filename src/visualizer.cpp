#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

/* ---------------- WINDOW ---------------- */
const int WIDTH = 1000;
const int HEIGHT = 600;

/* ---------------- VEHICLE ---------------- */
struct Vehicle {
    string id;
    int x, y;
    int speed;
};

/* ---------------- TRAFFIC LIGHT ---------------- */
struct TrafficLight {
    bool green;
    Uint32 lastSwitch;
};

/* ---------------- QUEUES (DSA) ---------------- */
queue<Vehicle> AL2, BL2, CL2, DL2;

/* ---------------- LIGHTS ---------------- */
TrafficLight north{false, 0};
TrafficLight south{false, 0};
TrafficLight east{true, 0};
TrafficLight west{true, 0};

/* ---------------- LOAD VEHICLES (FROM YOUR GENERATOR) ---------------- */
void loadVehicles() {
    ifstream file("data/vehicles.data");
    if (!file.is_open()) {
        cout << "vehicles.data not found!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {

        // FORMAT: VEHICLE:LANE  →  AB3:AL2
        size_t pos = line.find(":");
        if (pos == string::npos) continue;

        string id = line.substr(0, pos);
        string lane = line.substr(pos + 1);

        // We visualize ONLY lane 2 (DSA logic)
        if (lane == "AL2")
            AL2.push({id, 485, HEIGHT, 2});
        else if (lane == "BL2")
            BL2.push({id, 515, -40, 2});
        else if (lane == "CL2")
            CL2.push({id, -40, 295, 2});
        else if (lane == "DL2")
            DL2.push({id, WIDTH, 325, 2});
    }
}

/* ---------------- UPDATE LIGHTS ---------------- */
void updateLights() {
    Uint32 now = SDL_GetTicks();

    if (now - north.lastSwitch > 4000) {
        north.lastSwitch = now;

        bool priority = AL2.size() > 10;

        if (priority) {
            north.green = south.green = true;
            east.green = west.green = false;
        } else {
            east.green = west.green = true;
            north.green = south.green = false;
        }
    }
}

/* ---------------- MOVE VEHICLES ---------------- */
void moveVehicles() {
    if (north.green && !AL2.empty()) {
        AL2.front().y -= AL2.front().speed;
        if (AL2.front().y < 260)
            AL2.pop();
    }

    if (south.green && !BL2.empty()) {
        BL2.front().y += BL2.front().speed;
        if (BL2.front().y > 340)
            BL2.pop();
    }

    if (east.green && !CL2.empty()) {
        CL2.front().x += CL2.front().speed;
        if (CL2.front().x > 500)
            CL2.pop();
    }

    if (west.green && !DL2.empty()) {
        DL2.front().x -= DL2.front().speed;
        if (DL2.front().x < 420)
            DL2.pop();
    }
}

/* ---------------- DRAW ROADS ---------------- */
void drawRoads(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 50, 50, 50, 255);

    SDL_Rect h = {0, 260, WIDTH, 100};
    SDL_Rect v = {450, 0, 100, HEIGHT};

    SDL_RenderFillRect(r, &h);
    SDL_RenderFillRect(r, &v);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    for (int i = 0; i < WIDTH; i += 40)
        SDL_RenderDrawLine(r, i, 310, i + 20, 310);

    for (int i = 0; i < HEIGHT; i += 40)
        SDL_RenderDrawLine(r, 500, i, 500, i + 20);
}

/* ---------------- DRAW LIGHTS ---------------- */
void drawLights(SDL_Renderer* r) {
    SDL_Rect n = {485, 230, 20, 20};
    SDL_Rect s = {515, 380, 20, 20};
    SDL_Rect e = {560, 295, 20, 20};
    SDL_Rect w = {420, 325, 20, 20};

    SDL_SetRenderDrawColor(r, north.green ? 0 : 255, north.green ? 255 : 0, 0, 255);
    SDL_RenderFillRect(r, &n);
    SDL_RenderFillRect(r, &s);

    SDL_SetRenderDrawColor(r, east.green ? 0 : 255, east.green ? 255 : 0, 0, 255);
    SDL_RenderFillRect(r, &e);
    SDL_RenderFillRect(r, &w);
}

/* ---------------- DRAW VEHICLES ---------------- */
void drawVehicles(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 0, 0, 255, 255);

    if (!AL2.empty()) {
        SDL_Rect v;
        v.x = AL2.front().x;
        v.y = AL2.front().y;
        v.w = 20;
        v.h = 30;
        SDL_RenderFillRect(r, &v);
    }

    if (!BL2.empty()) {
        SDL_Rect v;
        v.x = BL2.front().x;
        v.y = BL2.front().y;
        v.w = 20;
        v.h = 30;
        SDL_RenderFillRect(r, &v);
    }

    if (!CL2.empty()) {
        SDL_Rect v;
        v.x = CL2.front().x;
        v.y = CL2.front().y;
        v.w = 30;
        v.h = 20;
        SDL_RenderFillRect(r, &v);
    }

    if (!DL2.empty()) {
        SDL_Rect v;
        v.x = DL2.front().x;
        v.y = DL2.front().y;
        v.w = 30;
        v.h = 20;
        SDL_RenderFillRect(r, &v);
    }
}


/* ---------------- MAIN ---------------- */
int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Traffic Queue Visualizer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );

    SDL_Renderer* r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    loadVehicles();

    bool run = true;
    SDL_Event e;

    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                run = false;
        }

        updateLights();
        moveVehicles();

        SDL_SetRenderDrawColor(r, 170, 170, 170, 255);
        SDL_RenderClear(r);

        drawRoads(r);
        drawLights(r);
        drawVehicles(r);

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
