// visualizer.cpp - SDL2 Version
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
=======
#include <windows.h>
#include <process.h>
#include <iostream>
#include <cstring>
#include <cstdio>
>>>>>>> fc1faef ( updated traffic generator program)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ROAD_WIDTH 200
#define LANE_WIDTH 50
#define PIPE_NAME "\\\\.\\pipe\\VehicleQueuePipe"
#define MAX_TEXT 100
#define MAX_SIZE 100
#define STR_LEN 15

<<<<<<< HEAD
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
=======
struct Vehicle {
    char id[STR_LEN];
    float progress;
};

struct Queue {
    Vehicle data[MAX_SIZE];
    int front;
    int rear;
};

struct SharedData {
    int nextLight;
    int priority;
    int lightPhase;
    Queue traffic[4][3];
};

int queueSize(Queue* q) {
    if (q->front == -1 || q->front > q->rear) return 0;
    return q->rear - q->front + 1;
}

void initQueue(Queue* q) { 
    q->front = -1; 
    q->rear = -1; 
}

bool isFull(Queue* q) { 
    return (q->rear == MAX_SIZE - 1); 
}

bool isEmpty(Queue* q) { 
    return (q->front == -1 || q->front > q->rear); 
}

void enqueue(Queue* q, const char* value) {
    if (!isFull(q)) {
        if (isEmpty(q)) q->front = 0;
        q->rear++;
        strcpy(q->data[q->rear].id, value);
        q->data[q->rear].progress = 0.0f;
>>>>>>> fc1faef ( updated traffic generator program)
    }
    
    file.close();
    
    // Clear file
    ofstream clear("data/vehicles.data", ios::trunc);
    clear.close();
}

<<<<<<< HEAD
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
=======
void dequeue(Queue* q) {
    if (!isEmpty(q)) {
        q->front++;
        if (q->front > q->rear) {
            q->front = -1;
            q->rear = -1;
>>>>>>> fc1faef ( updated traffic generator program)
        }
    }
}

<<<<<<< HEAD
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
=======
unsigned __stdcall pipeListenerThread(void* arg) {
    SharedData* sharedData = (SharedData*)arg;
    HANDLE pipe;
    char buffer[MAX_TEXT];
    DWORD bytesRead;

    pipe = CreateNamedPipeA(
        PIPE_NAME, PIPE_ACCESS_INBOUND,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, MAX_TEXT, MAX_TEXT, 0, NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create pipe" << std::endl;
        return 1;
    }

    std::cout << "Traffic Simulator: Waiting for generator..." << std::endl;
    
    if (ConnectNamedPipe(pipe, NULL)) {
        std::cout << "Generator connected!" << std::endl;
        
        while (true) {
            BOOL success = ReadFile(pipe, buffer, sizeof(buffer)-1, &bytesRead, NULL);
            if (!success || bytesRead == 0) break;

            buffer[bytesRead] = '\0';
            
            int out = (buffer[9] - '0');
            int in = (buffer[10] - '0');

            if (out >= 0 && out < 4) {
                int lane;
                if (out + 1 == in || (out == 3 && in == 0)) {
                    lane = 0; // Left turn
                } else if (out == in - 1 || (out == 0 && in == 3)) {
                    lane = 2; // Right turn
                } else {
                    lane = 1; // Straight
                }
                
                enqueue(&sharedData->traffic[out][lane], buffer);
                std::cout << "✓ " << buffer << " → Road " << out << " Lane " << lane << std::endl;
            }
        }
    }
    
    CloseHandle(pipe);
    return 0;
}

unsigned __stdcall checkQueueThread(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    while (true) {
        int total_cars = 0;
        
        for (int r = 0; r < 4; r++) {
            for (int l = 1; l < 3; l++) {
                Queue* q = &data->traffic[r][l];
                if (!isEmpty(q)) {
                    total_cars += queueSize(q);
                }
            }
        }
        
        int avg_cars = (total_cars + 3) / 4;
        int greenlight_time = avg_cars * 1000;
        if (greenlight_time < 3000) greenlight_time = 3000;
        if (greenlight_time > 8000) greenlight_time = 8000;
        
        bool foundCongestion = false;
        
        if (data->priority == 4) {
            data->lightPhase = 0; // Green
            Sleep(greenlight_time);
            data->lightPhase = 1; // Yellow
            Sleep(2000);
            data->nextLight = (data->nextLight + 1) % 4;
        } else {
            data->nextLight = data->priority;
            data->priority = 4;
            data->lightPhase = 0;
            Sleep(greenlight_time);
        }
        
        for (int r = 0; r < 4; r++) {
            int count = 0;
            for (int l = 1; l < 3; l++) {
                Queue* q = &data->traffic[r][l];
                if (!isEmpty(q)) {
                    count += queueSize(q);
                }
            }
            
            if (count > 10) {
                foundCongestion = true;
                data->priority = r;
                std::cout << "[PRIORITY] Road " << r << " has " << count << " vehicles!" << std::endl;
                break;
            }
        }
        
        if (!foundCongestion) {
            data->priority = 4;
        }
    }
    return 0;
}

void drawRoads(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    
    SDL_Rect vRoad = {WINDOW_WIDTH/2 - ROAD_WIDTH/2, 0, ROAD_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &vRoad);
    
    SDL_Rect hRoad = {0, WINDOW_HEIGHT/2 - ROAD_WIDTH/2, WINDOW_WIDTH, ROAD_WIDTH};
    SDL_RenderFillRect(renderer, &hRoad);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (int i = 0; i <= 3; i++) {
        int offset = WINDOW_HEIGHT/2 - ROAD_WIDTH/2 + LANE_WIDTH * i;
        SDL_RenderDrawLine(renderer, 0, offset, WINDOW_WIDTH/2 - ROAD_WIDTH/2, offset);
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH/2 + ROAD_WIDTH/2, offset, WINDOW_WIDTH, offset);
        
        int vOffset = WINDOW_WIDTH/2 - ROAD_WIDTH/2 + LANE_WIDTH * i;
        SDL_RenderDrawLine(renderer, vOffset, 0, vOffset, WINDOW_HEIGHT/2 - ROAD_WIDTH/2);
        SDL_RenderDrawLine(renderer, vOffset, WINDOW_HEIGHT/2 + ROAD_WIDTH/2, vOffset, WINDOW_HEIGHT);
    }
}

void drawLight(SDL_Renderer* renderer, int x, int y, int w, int h, bool isGreen, bool isYellow) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_Rect box = {x - 5, y - 5, w + 10, h + 10};
    SDL_RenderFillRect(renderer, &box);

    if (isYellow) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else if (isGreen) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    
    SDL_Rect bulb = {x, y, w, h};
    SDL_RenderFillRect(renderer, &bulb);
}

void drawLights(SDL_Renderer* renderer, SharedData* data) {
    bool yellow = (data->lightPhase == 1);
    
    drawLight(renderer, 265, 140, 30, 60, data->nextLight == 0, yellow);
    drawLight(renderer, 140, 505, 60, 30, data->nextLight == 1, yellow);
    drawLight(renderer, 505, 600, 30, 60, data->nextLight == 2, yellow);
    drawLight(renderer, 600, 265, 60, 30, data->nextLight == 3, yellow);
}

void drawVehicles(SDL_Renderer* renderer, SharedData* sharedData) {
    float speed = 2.0f;
    float stopLineDistance = 260.0f;
    float turnPoint0 = 310.0f;
    float turnPoint2 = 410.0f;

    for (int road = 0; road < 4; road++) {
        for (int lane = 0; lane < 3; lane++) {
            Queue* q = &sharedData->traffic[road][lane];
            if (isEmpty(q)) continue;

            for (int i = q->front; i <= q->rear; i++) {
                Vehicle* v = &q->data[i];
                int actualPosInLine = 0;
                float targetProgress = 1500.0f;

                if (lane == 0) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                else if (lane == 1) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                else SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

                if (lane != 0) {
                    for (int j = sharedData->traffic[road][1].front; 
                         j <= sharedData->traffic[road][1].rear; j++) {
                        if (j >= 0 && sharedData->traffic[road][1].data[j].progress > v->progress) 
                            actualPosInLine++;
                    }
                    for (int j = sharedData->traffic[road][2].front; 
                         j <= sharedData->traffic[road][2].rear; j++) {
                        if (j >= 0 && sharedData->traffic[road][2].data[j].progress > v->progress) 
                            actualPosInLine++;
                    }
                    targetProgress = stopLineDistance - (actualPosInLine * 40.0f);
                }

                if (lane == 0 || (sharedData->nextLight == road || v->progress > stopLineDistance)) {
                    targetProgress = 1500.0f;
                }

                if (v->progress < targetProgress) {
                    v->progress += speed;
                }

                SDL_Rect carRect;
                int displayLane = (lane != 0) ? 1 : 0;

                if (lane == 0 && v->progress >= turnPoint0) {
                    float turnOffset = v->progress - turnPoint0;
                    if (road == 0) carRect = {309 - (int)turnOffset, (int)turnPoint0, 30, 30};
                    else if (road == 1) carRect = {(int)turnPoint0, 461 + (int)turnOffset, 30, 30};
                    else if (road == 2) carRect = {461 + (int)turnOffset, 800 - (int)turnPoint0 - 30, 30, 30};
                    else carRect = {800 - (int)turnPoint0 - 30, 309 - (int)turnOffset, 30, 30};
                } else if (lane == 2 && v->progress >= turnPoint2) {
                    float turnOffset = v->progress - turnPoint2;
                    if (road == 0) carRect = {309 + 50 + (int)turnOffset, (int)turnPoint2, 30, 30};
                    else if (road == 1) carRect = {(int)turnPoint2, 461 - 50 - (int)turnOffset, 30, 30};
                    else if (road == 2) carRect = {461 - 50 - (int)turnOffset, 800 - (int)turnPoint2 - 30, 30, 30};
                    else carRect = {800 - (int)turnPoint2 - 30, 309 + 50 + (int)turnOffset, 30, 30};
                } else {
                    if (road == 0) carRect = {309 + (displayLane * 50), (int)v->progress, 30, 30};
                    else if (road == 1) carRect = {(int)v->progress, 461 - (displayLane * 50), 30, 30};
                    else if (road == 2) carRect = {461 - (displayLane * 50), 800 - (int)v->progress - 30, 30, 30};
                    else carRect = {800 - (int)v->progress - 30, 309 + (displayLane * 50), 30, 30};
                }

                SDL_RenderFillRect(renderer, &carRect);
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &carRect);
            }

            if (!isEmpty(q) && q->data[q->front].progress >= 1000.0f) {
                dequeue(q);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Traffic Simulator - Named Pipe IPC   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Traffic Simulator - Queue System",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SharedData sharedData;
    sharedData.nextLight = 0;
    sharedData.priority = 4;
    sharedData.lightPhase = 0;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            initQueue(&sharedData.traffic[i][j]);
        }
    }

    _beginthreadex(NULL, 0, checkQueueThread, &sharedData, 0, NULL);
    _beginthreadex(NULL, 0, pipeListenerThread, &sharedData, 0, NULL);

    std::cout << "\nSimulator ready!" << std::endl;
    std::cout << "Start the generator program now..." << std::endl;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
>>>>>>> fc1faef ( updated traffic generator program)
        }
        
        SDL_RenderFillRect(r, &carRect);
    }
}

<<<<<<< HEAD
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
    
=======
        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
        SDL_RenderClear(renderer);

        drawRoads(renderer);
        drawVehicles(renderer, &sharedData);
        drawLights(renderer, &sharedData);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

>>>>>>> fc1faef ( updated traffic generator program)
    return 0;
}