Traffic Junction Simulation Using Data Structures
Name: [Your Name]
Roll Number: [Your Roll Number]
Class: CS I
Subject: Data Structures and Algorithms

1. Summary of Work
In this project, I have built a traffic junction simulation system using C++ to manage vehicle flow at a four-way intersection. The main goal was to understand how queue data structures can be used to solve real-world problems like traffic management.
I divided my work into three separate programs:
Traffic Generator - This program keeps running and creates new vehicles every second. Each vehicle gets a random ID like "AB3" or "XY7" and is assigned to a random lane. All this information is saved in a file called vehicles.data so the other programs can read it.
Traffic Simulator - This program reads the vehicle data from the file and puts each vehicle into the correct lane queue. I created 12 different queues for all the lanes (AL1, AL2, AL3, BL1, BL2, BL3, etc.). The simulator has two main conditions. First is the priority condition - when AL2 lane has more than 10 waiting vehicles, it becomes a priority lane and gets served immediately until only 5 vehicles remain. Second is the normal condition - when there's no priority, the program calculates the average number of vehicles in BL2, CL2, and DL2, and serves that many vehicles from each lane.
Traffic Visualizer - This program uses SDL2 library to show everything visually. I drew the roads, put traffic lights at the junction, and showed vehicles as small rectangles. The lights change color based on which lane is being served. When a light is green, vehicles from that lane can move through the junction.
Overall, this project helped me understand how queues work in a practical situation and how data structures can make traffic management more organized.

2. Data Structures Used
Data StructureHow I Implemented ItWhat I Used It ForQueuestd::queue<string>I created 12 separate queues to store vehicles waiting in each laneQueuestd::queue<Vehicle>Used in visualizer to store vehicle objects for the 4 main lanesStructurestruct VehicleTo store each vehicle's ID, position (x, y), and speedStructurestruct TrafficLightTo keep track of whether light is green and when it last changedFile Handlingifstream and ofstreamTo read vehicle data from file and write new vehicles to it

3. Functions Using Data Structures
In Traffic Generator:

generateVehicleID() - I made this to create random IDs using two random letters and one random number
generateLane() - This function randomly picks one of the 12 lanes (like AL1, BL2, CL3, etc.)
main() - The main loop that keeps generating vehicles and saving them to the file

In Traffic Simulator:

loadVehicles() - Reads all the vehicles from the file and puts them in the right queue
calculateNormalServe() - Calculates how many vehicles should be served from each lane in normal condition by taking the average
main() - This is where I check if AL2 needs priority, and then serve vehicles accordingly

In Traffic Visualizer:

loadVehicles() - Loads vehicles into the four main lane queues (AL2, BL2, CL2, DL2)
updateLights() - Changes traffic lights every 4 seconds and checks if AL2 should get priority
moveVehicles() - Moves the front vehicle in a queue when its light is green
drawRoads() - Draws the horizontal and vertical roads with white dashed lines
drawLights() - Shows the traffic lights as small colored squares (red or green)
drawVehicles() - Displays vehicles as blue rectangles on the road


4. Algorithm Used for Processing Traffic
Here's how my simulator processes the traffic:
Step 1: First, the program starts and opens the vehicles.data file
Step 2: It reads every line from the file. Each line has a vehicle ID and its lane separated by a colon (like "AB3:AL2")
Step 3: For each vehicle, I extract the lane name and push the vehicle into the correct queue
Step 4: After loading all vehicles, the program shows how many vehicles are waiting in each lane
Step 5: Now comes the main logic - I check if AL2 has more than 10 vehicles

If yes, AL2 becomes the priority lane and I keep removing vehicles from AL2 queue until only 5 are left
If no, I calculate the average of vehicles in BL2, CL2, and DL2, then remove that many vehicles from each of these three queues

Step 6: Finally, the program shows the updated queue sizes after serving
For the visualizer, the process is a bit different:
Step 1: SDL window opens showing the roads and junction
Step 2: Vehicles are loaded from the file into queues
Step 3: Every 4 seconds, traffic lights change. If AL2 has more than 10 vehicles, north-south lights turn green (giving AL2 priority). Otherwise, east-west lights turn green
Step 4: Every frame (about 60 times per second), the program moves the front vehicle from whichever lane has a green light
Step 5: When a vehicle crosses the center of the junction, it is removed from the queue
Step 6: Everything is redrawn on the screen - roads, lights, and vehicles

5. Time Complexity Analysis
Let me explain how fast my program runs:
For the Generator:

Creating one vehicle ID takes constant time - O(1)
Writing to the file also takes constant time - O(1)
So generating each vehicle is O(1)

For the Simulator:

Reading n vehicles from the file takes O(n) time because I have to read each line
Putting each vehicle into a queue takes O(1), so for n vehicles it's O(n)
Checking if AL2 has more than 10 vehicles is just O(1)
Serving vehicles depends on how many need to be served. In priority mode, if I need to serve k vehicles from AL2, it takes O(k) time. In normal mode, if I serve m vehicles from each lane, it takes O(m)
Overall, the simulator runs in O(n) time where n is the number of vehicles

For the Visualizer:

Loading vehicles initially takes O(n) time
Updating lights is just checking one condition - O(1)
Moving vehicles is O(1) because I only move the front vehicle from each queue
Drawing everything (roads, 4 lights, 4 vehicles) is O(1)
So each frame runs in constant time O(1)

Summary: The system is quite efficient. Loading vehicles takes linear time based on how many vehicles there are, but the actual processing and visualization happens very fast in constant time. This makes it suitable for real-time simulation.

6. Link to Source Code
GitHub Repository: [Your GitHub Link]

Conclusion
Working on this project taught me a lot about how data structures work in real applications. I learned that queues are perfect for managing things that need to be handled in order, just like vehicles waiting at a traffic light.
What worked well:

The queue implementation was straightforward and handled vehicles in the correct order
The priority logic for AL2 worked as expected - when it got crowded, it got immediate attention
The formula for normal condition (taking average) made sense for fair distribution
The visualization helped me actually see what was happening with the queues

What could be improved:

My simulator only runs once and then stops. It should keep running continuously like real traffic
The three programs don't really work together in real-time. They're more separate pieces
The visualizer only shows one vehicle from each queue at a time, not all the waiting vehicles
I could add more features like showing vehicle counts on screen or making the vehicles look more realistic

What I learned:

How to use queues for FIFO (First In First Out) operations
How to read and write files for data sharing between programs
How to use SDL2 library for graphics
How priority systems can reduce congestion
The importance of time complexity in making programs run efficiently

Overall, I think my project meets the assignment requirements. It demonstrates queue usage, implements the priority lane concept, and provides both simulation logic and visual output. With more time, I would make it more integrated and add continuous simulation, but as a learning project, it helped me understand data structures much better.