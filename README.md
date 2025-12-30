# Traffic Queue Simulator 🚦

**Assignment #1 - Data Structures and Algorithms (COMP202)**  
**Name:** Aarjan Adhikari  
**Roll Number:** 03  
**Section:** CS I  
**Date:** November 2025


---

##  Demo Video



https://github.com/user-attachments/assets/c3d81d1f-5e82-4c18-8397-8b867c44be12



##  Summary of Work

This project implements a real-time traffic management system for a 4-way junction using queue data structures. The system simulates vehicle movement through an intersection with intelligent traffic light control that adapts based on congestion levels.

**What I implemented:**

1. **Queue-based vehicle management** - Created 12 separate queues (4 roads × 3 lanes) to manage waiting vehicles
2. **Dynamic traffic light system** - Implemented adaptive timing that adjusts green light duration based on average queue length
3. **Priority mechanism** - Built automatic congestion detection that prioritizes roads with 10+ waiting vehicles
4. **Visual simulation** - Used SDL2 to create real-time graphics showing vehicle movement and traffic lights
5. **Inter-process communication** - Set up Windows Named Pipes for communication between generator and visualizer programs
6. **Vehicle routing** - Implemented logic for three types of movements: left turn, straight, and right turn

The system successfully demonstrates how queue data structures can solve real-world traffic flow problems while maintaining fairness and efficiency.

---

## Data Structures Used

| Data Structure | Implementation Details | Purpose |
|---------------|----------------------|---------|
| **Queue (Array-based)** | Array of 100 elements with front and rear pointers | Stores vehicles waiting in each lane; FIFO ensures fairness |
| **Vehicle Struct** | Contains: `char id[15]` and `float progress` | Tracks individual vehicle identification and current position on road |
| **SharedData Struct** | Contains: traffic arrays, nextLight, priority, lightPhase | Maintains global state shared between threads for synchronization |
| **2D Array of Queues** | `Queue traffic[4][3]` - 4 roads, 3 lanes each | Organizes all 12 lane queues in logical structure matching road layout |

### Implementation Details

**Queue Structure:**
```cpp
struct Queue {
    Vehicle data[MAX_SIZE];  // Array of 100 vehicles
    int front;                // Index of first element (-1 if empty)
    int rear;                 // Index of last element (-1 if empty)
};
```

**Vehicle Structure:**
```cpp
struct Vehicle {
    char id[STR_LEN];  // License plate format: "AB3CD5:12"
    float progress;     // Current position (0 to 1500 pixels)
};
```

---

## 🔧 Functions Implemented

### Queue Operations

| Function | Purpose | Time Complexity |
|----------|---------|----------------|
| `initQueue(Queue* q)` | Initialize empty queue by setting front and rear to -1 | O(1) |
| `isEmpty(Queue* q)` | Check if queue has no elements | O(1) |
| `isFull(Queue* q)` | Check if queue reached maximum capacity (100) | O(1) |
| `enqueue(Queue* q, char* value)` | Add new vehicle to rear of queue | O(1) |
| `dequeue(Queue* q)` | Remove vehicle from front of queue | O(1) |
| `queueSize(Queue* q)` | Calculate number of vehicles waiting | O(1) |

### Threading Functions

| Function | Purpose |
|----------|---------|
| `pipeListenerThread()` | Continuously reads vehicle data from Named Pipe and enqueues to appropriate lane |
| `checkQueueThread()` | Monitors all queues, calculates light timing, detects congestion, manages priority system |

### Rendering Functions

| Function | Purpose |
|----------|---------|
| `drawRoads()` | Renders gray road surfaces with white lane markings |
| `drawLights()` | Displays traffic lights with correct colors (red/yellow/green) |
| `drawVehicles()` | Updates vehicle positions and renders them with collision detection |

---

##  Algorithms

### 1. Traffic Light Control Algorithm

```
ALGORITHM: ManageTrafficLights

INPUT: SharedData containing all queue states
OUTPUT: Updated light states and timing

BEGIN
    WHILE simulation running DO
        // Calculate average vehicles across all lanes
        total_cars ← 0
        FOR each road r in [0, 3] DO
            FOR each lane l in [1, 2] DO  // Skip left turn lane
                total_cars ← total_cars + queueSize(traffic[r][l])
            END FOR
        END FOR
        
        avg_cars ← (total_cars + 3) / 4  // Round up division
        
        // Dynamic green light timing
        greenlight_time ← avg_cars × 1000 milliseconds
        IF greenlight_time < 3000 THEN
            greenlight_time ← 3000  // Minimum 3 seconds
        END IF
        IF greenlight_time > 8000 THEN
            greenlight_time ← 8000  // Maximum 8 seconds
        END IF
        
        // Check for congestion and set priority
        foundCongestion ← FALSE
        FOR each road r in [0, 3] DO
            count ← 0
            FOR each lane l in [1, 2] DO
                count ← count + queueSize(traffic[r][l])
            END FOR
            
            IF count > 10 THEN
                priority ← r
                foundCongestion ← TRUE
                BREAK
            END IF
        END FOR
        
        // Determine next light
        IF priority is set THEN
            nextLight ← priority
            priority ← 4  // Reset priority after serving
        ELSE
            nextLight ← (nextLight + 1) mod 4  // Round-robin
        END IF
        
        // Execute light cycle
        lightPhase ← 0  // Green
        SLEEP(greenlight_time)
        lightPhase ← 1  // Yellow
        SLEEP(2000)     // Yellow for 2 seconds
    END WHILE
END
```

**Time Complexity:** O(L) where L = total number of lanes (12)
- Checking all queues: O(12) = O(1) constant
- Priority detection: O(4 roads × 2 lanes) = O(8) = O(1) constant
- Overall per cycle: **O(1)** constant time

### 2. Vehicle Movement Algorithm

```
ALGORITHM: MoveVehicles

INPUT: SharedData with all queues, current light state
OUTPUT: Updated vehicle positions

BEGIN
    FOR each road r in [0, 3] DO
        FOR each lane l in [0, 2] DO
            IF queue is empty THEN CONTINUE
            
            FOR each vehicle v in queue DO
                // Calculate stopping position
                IF lane is straight or right THEN
                    actualPosInLine ← count vehicles ahead in both lanes
                    targetProgress ← stopLineDistance - (actualPosInLine × 40)
                ELSE
                    targetProgress ← 1500  // No stopping for left turns
                END IF
                
                // Check if can proceed through light
                IF light is GREEN for this road OR vehicle already passed stop line THEN
                    targetProgress ← 1500  // Allow to exit
                END IF
                
                // Move vehicle forward
                IF v.progress < targetProgress THEN
                    v.progress ← v.progress + speed
                END IF
                
                // Handle turns at intersection
                IF lane is left AND v.progress >= turnPoint THEN
                    Calculate left turn trajectory
                ELSE IF lane is right AND v.progress >= turnPoint THEN
                    Calculate right turn trajectory
                ELSE
                    Move straight along lane
                END IF
                
                Render vehicle at calculated position
            END FOR
            
            // Remove vehicles that exited
            IF front vehicle progress >= 1000 THEN
                dequeue(queue)
            END IF
        END FOR
    END FOR
END
```

**Time Complexity:** O(V) where V = total vehicles currently in system
- Iterating through all queues: O(12 lanes)
- Processing each vehicle: O(V / 12) average per lane
- Overall: **O(V)** linear in number of vehicles
- Typical case: V = 10-40 vehicles on screen

### 3. Vehicle Generation Algorithm

```
ALGORITHM: GenerateVehicles

OUTPUT: Vehicle data sent via Named Pipe

BEGIN
    Connect to Named Pipe
    
    WHILE TRUE DO
        // Generate random license plate
        vehicle_id ← Generate 8 random characters (AA0AA0)
        
        // Generate random route
        incoming_road ← Random(0, 3)
        outgoing_road ← Random(0, 3) WHERE outgoing ≠ incoming
        
        // Create message
        message ← vehicle_id + ":" + incoming_road + outgoing_road
        
        // Send via pipe
        WriteFile(pipe, message)
        
        SLEEP(1000)  // Generate every 1 second
    END WHILE
END
```

**Time Complexity:** O(1) constant time per vehicle generation

---

## ⏱️ Overall Time Complexity Analysis

### Per Frame (60 FPS):
- Traffic light check: O(1)
- Vehicle movement: O(V) where V is current vehicles
- Rendering: O(V)
- **Total per frame: O(V)**

### Worst Case Scenario:
- Maximum vehicles: 12 lanes × 100 = 1200 vehicles
- But rendering only shows ~40 vehicles at once
- Practical complexity: **O(40) = O(1)** constant

### Space Complexity:
- 12 queues × 100 vehicles = 1200 vehicle slots
- Each vehicle: 15 bytes + 4 bytes = 19 bytes
- **Total space: O(1)** = 22.8 KB constant memory

The algorithm is highly efficient because:
1. Queue operations are all O(1)
2. Only visible vehicles are processed for movement
3. No sorting or searching required
4. Fixed memory allocation (no dynamic growth)

---

## 🛠️ Build and Run Instructions

### Prerequisites

1. **Install MinGW-w64:**
   - Download from [mingw-w64.org](https://www.mingw-w64.org/)
   - Add to PATH: `C:\mingw64\bin`

2. **Install SDL2:**
   - Download SDL2-devel-mingw from [libsdl.org](https://www.libsdl.org/download-2.0.php)
   - Extract to `C:\SDL2`

### Compilation

**Option 1: Using Makefile**
```bash
make
```

**Option 2: Manual Compilation**

Compile visualizer:
```bash
g++ -I"C:\SDL2\include" -L"C:\SDL2\lib" -o visualizer.exe visualizer.cpp -lmingw32 -lSDL2main -lSDL2 -mwindows
```

Compile generator:
```bash
g++ -o generator.exe traffic_generator.cpp
```

### Running the Simulation

**IMPORTANT:** Start programs in this exact order!

**Step 1:** Open first terminal and start visualizer:
```bash
./visualizer.exe
```
Wait for message: `"Traffic Simulator: Waiting for generator..."`

**Step 2:** Open second terminal and start generator:
```bash
./generator.exe
```
You should see: `"Connected to visualizer!"`

Now vehicles will start appearing and moving through the intersection!

### Controls
- **ESC** key or close window to exit

---

## 📁 Project Structure

```
dsa-queue-simulator/
│
├── visualizer.cpp              # Main simulation with SDL2 graphics
├── traffic_generator.cpp       # Vehicle generation program
├── Makefile                    # Build automation
└── README.md                   # This documentation
```

---

## 🎨 Visual Guide

### Color Coding
- **Background:** Green (grass/surroundings)
- **Roads:** Gray with white lane markings
- **Traffic Lights:**
  - Red = Stop (vehicles must wait)
  - Yellow = Caution (2 second transition)
  - Green = Go (vehicles can proceed)
- **Vehicles:**
  - Yellow = Left turn lane
  - Blue = Straight lane
  - Cyan = Right turn lane

### Vehicle ID Format
Example: `AB3CD512:23`
- `AB3CD512` = Random license plate
- `2` = Source road (0-3)
- `3` = Destination road (0-3)

---

## 🔗 Source Code

GitHub Repository: [Add your repository link here]

```
https://github.com/yourusername/dsa-queue-simulator
```

---

##  References

### Documentation & Tutorials
1. **SDL2 Documentation** - Graphics rendering and window management  
   https://wiki.libsdl.org/SDL2/FrontPage

2. **Windows Named Pipes API** - Inter-process communication  
   https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipes

3. **Queue Data Structure** - COMP202 Course Textbook, Chapter 4

### Code References
1. **SDL2 Setup Tutorial** - Used for initial window and renderer setup  
   https://lazyfoo.net/tutorials/SDL/

2. **Named Pipes Example** - Adapted for generator-visualizer communication  
   https://docs.microsoft.com/en-us/windows/win32/ipc/named-pipe-client

3. **Traffic Simulation Concepts** - General algorithm design inspiration  
   Course lectures and lab materials

### External Libraries
- **SDL2 (v2.0+)** - Simple DirectMedia Layer for graphics
- **MinGW-w64** - GNU Compiler Collection for Windows

---

## ⚙️ Configuration Options

You can customize behavior by modifying these constants:

**visualizer.cpp:**
```cpp
#define WINDOW_WIDTH 800        // Window width in pixels
#define WINDOW_HEIGHT 800       // Window height in pixels
#define MAX_SIZE 100           // Max vehicles per queue
float speed = 2.0f;            // Vehicle speed (pixels/frame)
```

**Timing in `checkQueueThread()`:**
```cpp
greenlight_time (min: 3000ms, max: 8000ms)  // Green light duration
Sleep(2000)                                  // Yellow light duration
```

**traffic_generator.cpp:**
```cpp
Sleep(1000)  // Change to adjust vehicle generation rate
```

---

## 🐛 Known Limitations

1. **Platform:** Windows-only (uses Windows Named Pipes API)
2. **Single Generator:** Only one generator can connect at a time
3. **Left Turn Lane:** No traffic light control (always free to turn)
4. **Collision:** No vehicle-to-vehicle collision detection
5. **Queue Overflow:** If queue fills (100 vehicles), new vehicles are dropped

---

## 🎯 Learning Outcomes

Through this project, I learned:

- How to implement and use queue data structures in real applications
- Thread synchronization and concurrent programming concepts
- Inter-process communication using Named Pipes
- Graphics programming with SDL2
- Algorithm optimization for real-time systems
- Importance of data structure choice for efficiency


---

*This project demonstrates practical application of queue data structures in solving real-world traffic management problems.*
