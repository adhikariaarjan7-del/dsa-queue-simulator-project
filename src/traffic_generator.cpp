// generator.cpp - Named Pipe Version
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define PIPE_NAME "\\\\.\\pipe\\VehicleQueuePipe"
#define MAX_TEXT 100

void generateVehicleNumber(char* buffer) {
    buffer[0] = 'A' + rand() % 26;
    buffer[1] = 'A' + rand() % 26;
    buffer[2] = '0' + rand() % 10;
    buffer[3] = 'A' + rand() % 26;
    buffer[4] = 'A' + rand() % 26;
    buffer[5] = '0' + rand() % 10;
    buffer[6] = '0' + rand() % 10;
    buffer[7] = '0' + rand() % 10;
    buffer[8] = '\0';
}

char generateOutgoingDirection() {
    char lanes[] = {'0', '1', '2', '3'};
    return lanes[rand() % 4];
}

char generateIncomingDirection(char incoming) {
    char lanes[] = {'0', '1', '2', '3'};
    char outgoing;
    do {
        outgoing = lanes[rand() % 4];
    } while (outgoing == incoming);
    return outgoing;
}

int main() {
    HANDLE pipe;
    DWORD bytesWritten;
    char message[MAX_TEXT];
    
    srand((unsigned int)time(NULL));
    
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Traffic Generator - Named Pipe        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "\nConnecting to visualizer..." << std::endl;

    while (true) {
        pipe = CreateFileA(
            PIPE_NAME,
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        
        if (pipe != INVALID_HANDLE_VALUE) break;
        
        std::cout << "Waiting for visualizer... (retry in 1s)" << std::endl;
        Sleep(1000);
    }

    std::cout << "✓ Connected to visualizer!\n" << std::endl;
    std::cout << "Generating vehicles...\n" << std::endl;

    int count = 0;
    while (true) {
        char vehicle[9];
        generateVehicleNumber(vehicle);
        
        char inlane = generateOutgoingDirection();
        char outlane = generateIncomingDirection(inlane);
        
        snprintf(message, MAX_TEXT, "%s:%c%c", vehicle, inlane, outlane);

        BOOL success = WriteFile(pipe, message, (DWORD)strlen(message), &bytesWritten, NULL);
        
        if (success) {
            count++;
            std::cout << "[" << count << "] Sent: " << message 
                      << " (Road " << inlane << " → " << outlane << ")" << std::endl;
        } else {
            std::cerr << "Failed to send vehicle!" << std::endl;
            break;
        }

        Sleep(1000); // Generate vehicle every 1 second
    }

    CloseHandle(pipe);
    return 0;
}