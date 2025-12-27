Project Description
This project is a Traffic Junction Simulation System developed using C++ to demonstrate the practical application of Queue data structures in real‑world problems.
The system simulates vehicle movement at a four‑way traffic junction with proper traffic light control, priority lane handling, and visual representation using SDL2.

The main objective of this project is to understand:

FIFO behavior of queues

Priority handling in traffic systems

File‑based data sharing between programs

Basic graphical visualization of algorithms

🧩 Programs Included
The project consists of three separate programs, each having a specific role:

1️⃣ Traffic Generator
Runs continuously

Generates vehicles every second

Assigns each vehicle a random ID and lane

Stores data in vehicles.data

Example format:

AB3:AL2
2️⃣ Traffic Simulator
Reads vehicle data from file

Uses 12 queues (AL1–DL3) to represent lanes

Implements priority lane logic

If AL2 > 10 vehicles, it becomes priority

Priority continues until only 5 vehicles remain

In normal condition, vehicles are served fairly using average calculation

3️⃣ Traffic Visualizer
Uses SDL2 graphics

Displays:

Highway‑style roads

Red & Green traffic lights

Smooth vehicle movement

Vehicles:

Stop only at stop lines when light is red

Maintain distance between each other

Move faster when light turns green

Priority lane effect is visible graphically

🛠️ Technologies Used
C++

STL Queue (std::queue)

SDL2 Library

File Handling (ifstream / ofstream)

MinGW / MSYS2 (Windows)

▶️ How to Run the Project
🔧 Requirements
Windows OS

g++ compiler

SDL2 installed (MinGW64)

Step 1️⃣ Run Traffic Generator
g++ src/generator.cpp -o generator
.\generator
This continuously writes vehicle data into data/vehicles.data.

Step 2️⃣ Run Traffic Simulator
g++ src/simulator.cpp -o simulator
.\simulator
This processes the queues and shows traffic logic in the terminal.

Step 3️⃣ Run Traffic Visualizer
g++ src/visualizer.cpp -o visualizer -lSDL2
.\visualizer
This opens the graphical traffic junction window.
