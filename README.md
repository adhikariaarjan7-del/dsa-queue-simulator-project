Project Description
This project is a Traffic Junction Simulation System developed using C++ to demonstrate the practical application of Queue data structures in real‑world problems.
The system simulates vehicle movement at a four‑way traffic junction with proper traffic light control, priority lane handling, and visual representation using SDL2.

The main objective of this project is to understand:

FIFO behavior of queues

Priority handling in traffic systems

File‑based data sharing between programs

Basic graphical visualization of algorithms

Programs Included
The project consists of three separate programs, each having a specific role:

1️.Traffic Generator
Runs continuously

Generates vehicles every second

Assigns each vehicle a random ID and lane

Stores data in vehicles.data

Example format:

AB3:AL2
2️.Traffic Simulator
Reads vehicle data from file

Uses 12 queues (AL1–DL3) to represent lanes

Implements priority lane logic

If AL2 > 10 vehicles, it becomes priority

Priority continues until only 5 vehicles remain

In normal condition, vehicles are served fairly using average calculation

3️. Traffic Visualizer
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

3. How to Run the Project
🔧 Requirements
Windows OS

g++ compiler

SDL2 installed (MinGW64)

Step 1️. Run Traffic Generator
g++ src/generator.cpp -o generator
.\generator
This continuously writes vehicle data into data/vehicles.data.

Step 2️.Run Traffic Simulator
g++ src/simulator.cpp -o simulator
.\simulator
This processes the queues and shows traffic logic in the terminal.

Step 3️. Run Traffic Visualizer
g++ src/visualizer.cpp -o visualizer -lSDL2
.\visualizer
This opens the graphical traffic junction window.
Screenshots of the output
<img width="794" height="551" alt="Screenshot 2025-12-27 141428" src="https://github.com/user-attachments/assets/4525ba05-852a-4074-8358-a65478af59a9" />
<img width="1872" height="1033" alt="Screenshot 2025-12-27 190900" src="https://github.com/user-attachments/assets/e1f4816a-5aa9-4db2-8262-146e6e72144b" />
<img width="327" height="906" alt="Screenshot 2025-12-27 190934" src="https://github.com/user-attachments/assets/0f48b65e-23e0-4000-9260-c6ba793b315f" />
<img width="1583" height="952" alt="Screenshot 2025-12-27 191024" src="https://github.com/user-attachments/assets/77da6c2b-efa4-4aa0-bb4d-84d9fe550b4c" />
<img width="1440" height="907" alt="Screenshot 2025-12-27 191132" src="https://github.com/user-attachments/assets/872aa766-d45e-4259-9132-3fdf61db29b9" />
<img width="1460" height="917" alt="Screenshot 2025-12-27 141519" src="https://github.com/user-attachments/assets/b39525ce-86e7-4e9f-ba95-d12439a770bc" />

Data Structures Used
Data Structure	Implementation	Purpose
Queue	std::queue<string>	Store vehicles per lane (simulator)
Queue	std::queue<Vehicle>	Store vehicle objects (visualizer)
Structure	struct Vehicle	Vehicle ID & position
Structure	struct TrafficLight	Light state & timing
File Handling	ifstream / ofstream	Data sharing
⏱️ Time Complexity Overview
Vehicle generation: O(1)

File reading: O(n)

Queue insertion/removal: O(1)

Priority serving: O(k)

Visualizer frame update: O(1)

This makes the simulation efficient and suitable for real‑time behavior.

 References
SDL2 Documentation – https://wiki.libsdl.org

C++ STL Reference – https://cplusplus.com

Queue Data Structure – DSA Lecture Notes

Open‑source SDL examples

Source Code
GitHub Repository:https://github.com/adhikariaarjan7-del/dsa-queue-simulator-project.git


 Conclusion
This project successfully demonstrates how queues and priority logic can be used to manage traffic efficiently.
The combination of DSA concepts, file handling, and graphical visualization makes this project complete and suitable for academic evaluation.





