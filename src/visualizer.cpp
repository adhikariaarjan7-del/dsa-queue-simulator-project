#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int SCREEN_W = 1000;
const int SCREEN_H = 700;

// Vehicle queues for each lane
queue<string> AL2, BL2, CL2, DL2;

// Moving vehicle structure
struct Car {
    int x, y;
    string lane;
};

vector<Car> cars;

// Traffic state
string greenLane = "AL2";
int laneIndex = 0;
bool priority = false;

// Load vehicles from file
void loadVehicles() {
    ifstream file("data/vehicles.data");
    if (!file.is_open()) return;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos = line.find(":");
        if (pos == string::npos) continue;
        
        string lane = line.substr(pos + 1);
        
        // Remove spaces using std::remove
        lane.erase(std::remove(lane.begin(), lane.end(), ' '), lane.end());
        
        // Add to correct queue
        if (lane == "AL2") AL2.push(line);
        else if (lane == "BL2") BL2.push(line);
        else if (lane == "CL2") CL2.push(line);
        else if (lane == "DL2") DL2.push(line);
    }
    
    file.close();
    
    // Clear file
    ofstream clear("data/vehicles.data", ios::trunc);
    clear.close();
}

// Add car to road
void addCar(string lane) {
    Car c;
    c.lane = lane;
    
    if (lane == "AL2") {
        if (AL2.empty()) return;
        c.x = 490; c.y = 700;
        AL2.pop();
    }
    else if (lane == "BL2") {
        if (BL2.empty()) return;
        c.x = 490; c.y = 0;
        BL2.pop();
    }
    else if (lane == "CL2") {
        if (CL2.empty()) return;
        c.x = 0; c.y = 340;
        CL2.pop();
    }
    else if (lane == "DL2") {
        if (DL2.empty()) return;
        c.x = SCREEN_W; c.y = 340;
        DL2.pop();
    }
    
    cars.push_back(c);
}

// Move all cars
void moveCars() {
    for (int i = cars.size() - 1; i >= 0; i--) {
        Car& c = cars[i];
        
        // Move based on lane
        if (c.lane == "AL2") c.y -= 4;
        else if (c.lane == "BL2") c.y += 4;
        else if (c.lane == "CL2") c.x += 4;
        else if (c.lane == "DL2") c.x -= 4;
        
        // Remove if off screen
        if (c.y < -50 || c.y > SCREEN_H + 50 || c.x < -50 || c.x > SCREEN_W + 50) {
            cars.erase(cars.begin() + i);
        }
    }
}

// Change traffic light
void changeLight() {
    // Check priority condition
    if (AL2.size() > 10) {
        priority = true;
        greenLane = "AL2";
        cout << "PRIORITY: AL2 (" << AL2.size() << " vehicles)" << endl;
        
        // Release multiple vehicles
        for (int i = 0; i < 3; i++) addCar("AL2");
    }
    else {
        priority = false;
        
        // Normal rotation
        string lanes[] = {"AL2", "BL2", "CL2", "DL2"};
        greenLane = lanes[laneIndex];
        laneIndex = (laneIndex + 1) % 4;
        
        cout << "GREEN: " << greenLane << endl;
        
        // Release 2 vehicles
        addCar(greenLane);
        addCar(greenLane);
    }
    
    cout << "Queues - AL2:" << AL2.size() << " BL2:" << BL2.size() 
         << " CL2:" << CL2.size() << " DL2:" << DL2.size() << endl;
}

// Draw road
void drawRoad(SDL_Renderer* r) {
    // Road background
    SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
    SDL_Rect hRoad = {0, 300, SCREEN_W, 100};
    SDL_Rect vRoad = {450, 0, 100, SCREEN_H};
    SDL_RenderFillRect(r, &hRoad);
    SDL_RenderFillRect(r, &vRoad);
    
    // Lane lines
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    for (int i = 0; i < SCREEN_W; i += 30) {
        SDL_RenderDrawLine(r, i, 350, i + 15, 350);
    }
    for (int i = 0; i < SCREEN_H; i += 30) {
        SDL_RenderDrawLine(r, 500, i, 500, i + 15);
    }
}

// Draw traffic lights
void drawLights(SDL_Renderer* r) {
    // Light positions
    SDL_Rect lightN = {490, 270, 20, 20};
    SDL_Rect lightS = {490, 420, 20, 20};
    SDL_Rect lightE = {570, 340, 20, 20};
    SDL_Rect lightW = {410, 340, 20, 20};
    
    // AL2 (North)
    if (greenLane == "AL2")
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &lightN);
    
    // BL2 (South)
    if (greenLane == "BL2")
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &lightS);
    
    // CL2 (East)
    if (greenLane == "CL2")
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &lightE);
    
    // DL2 (West)
    if (greenLane == "DL2")
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &lightW);
}

// Draw cars
void drawCars(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 0, 120, 255, 255);
    
    for (const Car& c : cars) {
        SDL_Rect carRect;
        
        if (c.lane == "AL2" || c.lane == "BL2") {
            carRect = {c.x, c.y, 20, 30};
        } else {
            carRect = {c.x, c.y, 30, 20};
        }
        
        SDL_RenderFillRect(r, &carRect);
    }
}

// Draw queue info
void drawInfo(SDL_Renderer* r) {
    // Info box
    SDL_SetRenderDrawColor(r, 40, 40, 40, 200);
    SDL_Rect box = {10, 10, 250, 130};
    SDL_RenderFillRect(r, &box);
    
    // Priority indicator
    if (priority) {
        SDL_SetRenderDrawColor(r, 255, 50, 50, 255);
        SDL_Rect pBox = {10, 150, 250, 30};
        SDL_RenderFillRect(r, &pBox);
    }
    
    // Queue visualization
    SDL_SetRenderDrawColor(r, 255, 200, 0, 255);
    int y = 25;
    
    // AL2
    for (int i = 0; i < min((int)AL2.size(), 25); i++) {
        SDL_Rect dot = {20 + i*9, y, 6, 6};
        SDL_RenderFillRect(r, &dot);
    }
    
    y += 30;
    // BL2
    for (int i = 0; i < min((int)BL2.size(), 25); i++) {
        SDL_Rect dot = {20 + i*9, y, 6, 6};
        SDL_RenderFillRect(r, &dot);
    }
    
    y += 30;
    // CL2
    for (int i = 0; i < min((int)CL2.size(), 25); i++) {
        SDL_Rect dot = {20 + i*9, y, 6, 6};
        SDL_RenderFillRect(r, &dot);
    }
    
    y += 30;
    // DL2
    for (int i = 0; i < min((int)DL2.size(), 25); i++) {
        SDL_Rect dot = {20 + i*9, y, 6, 6};
        SDL_RenderFillRect(r, &dot);
    }
}

int main() {
    cout << "Traffic Simulator Started" << endl;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "Traffic Queue System",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    bool running = true;
    SDL_Event event;
    
    Uint32 lastLoad = SDL_GetTicks();
    Uint32 lastLight = SDL_GetTicks();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        
        Uint32 now = SDL_GetTicks();
        
        // Load vehicles every 2 seconds
        if (now - lastLoad > 2000) {
            loadVehicles();
            lastLoad = now;
        }
        
        // Change light every 4 seconds
        if (now - lastLight > 4000) {
            changeLight();
            lastLight = now;
        }
        
        // Update positions
        moveCars();
        
        // Draw everything
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderClear(renderer);
        
        drawRoad(renderer);
        drawLights(renderer);
        drawCars(renderer);
        drawInfo(renderer);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}