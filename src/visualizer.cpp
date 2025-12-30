// visualizer.cpp - SDL2 Version
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <windows.h>
#include <process.h>
#include <iostream>
#include <cstring>
#include <cstdio>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ROAD_WIDTH 200
#define LANE_WIDTH 50
#define PIPE_NAME "\\\\.\\pipe\\VehicleQueuePipe"
#define MAX_TEXT 100
#define MAX_SIZE 100
#define STR_LEN 15

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
    }
}

void dequeue(Queue* q) {
    if (!isEmpty(q)) {
        q->front++;
        if (q->front > q->rear) {
            q->front = -1;
            q->rear = -1;
        }
    }
}

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
        }

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

    return 0;
}