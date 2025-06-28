Hi! We are the Team TecnoTitans from Venezuela,and we are gonna participate The Categorie Future Engineer WRO 2025!

## Team Members 👨‍💻

**Jiezhan Zheng:**

Age:17

He is the kind of person who, when facing a problem, prefers to explore and analyze solutions independently first. He believes that the process of solving a challenge alone helps deepen understanding—but he’s also not afraid to reach out to teammates when collaboration is the best way forward.

He enjoys immersing himself in the digital world, whether it’s exploring new technologies, browsing forums, or losing track of time coding in silence. That said, he has a strong sense of balance and never lets the virtual world distract from his responsibilities in the real one.

Jiezhan's personality is calm and composed, often taking a rational approach to challenges. Still, beneath that logical surface lies a thoughtful side that sometimes gives way to sentiment. He sees beauty in precision—but also in moments of quiet creativity.

His role in the team is focused on programming, optimizing robot performance through thoughtful code, and contributing to both the logic and the emotional harmony of the group.

**Jhonny Wu:**

Age: 17

He is the quietest member of the team, a man of few words and always calm. He likes to take things easy—sleeping and eating are among his favorite activities. Although he doesn’t talk much, he’s always there when needed. 
His role on the team is that of a mechanic, responsible for assembling, adjusting, and making sure every part of the robot works properly.

**Gabriel Granadillo:**

Age: 15

He is very persistent and always tries to find a better solution, and remembers the team to always tries and smile through the problems, like to relax bye drawing anime/comics and singing. 

Role on the team: Presenter 

He is the one who understands a bit of programming and the logic of the mechanical built but he is a good strategist and designer for the esthetic of the vehicle.

**Floresmy Escalona**

She is the **Coach** of the team


## Ours History
The idea of immersing ourselves in the world of robotics was born thanks to the introduction of the extracurricular subject of Robotics at our institution, Unidad Educativa Colegio Santa Rosa. This new learning space awakened in us a great curiosity for technology and motivated us to explore beyond the classroom. As we progressed through the activities, we discovered skills we didn’t know we had: creativity to design solutions, logic to program, manual dexterity to build circuits, and, above all, a great ability to work as a team.

At the same time, the dream of creating an efficient autonomous vehicle emerged. From the beginning, we knew it wouldn’t be easy, but that didn’t stop us. We dove into this project with great enthusiasm, facing challenges from planning and design to programming and integrating each component. Sometimes, the cost of materials and the time each stage required became obstacles, but our motivation always outweighed everything else.

To learn what we needed, we turned to tutorials, educational videos, and all kinds of online resources. Little by little, our ideas began to take shape, and we managed to go from simple sketches to a functional model.
During the first tests, we realized that our prototypes were too heavy and hard to maneuver. That’s when the idea of building a high-profile vehicle came up, with an elevated base that would allow better turns. This decision was key, as it greatly improved the vehicle's mobility, especially in spaces with obstacles or sharp curves.

## 🔧 **Hardware Description**

The vehicle design focused on achieving a balance between stability, efficiency, maneuverability, and ease of maintenance, integrating compatible, accessible, and reliable components for educational competitive environments:

- **Main Controller:**  
  Arduino Uno R3, chosen for its stability, large support community, and compatibility with various analog and digital peripherals.

- **Motor Control Module:**  
  Dual H-bridge driver (L298N), used to independently control the rotation direction and speed of two DC motors via PWM signals.

- **Drive System:**  
  - Rear-wheel drive using a single type C DC motor (3V–6V).  
  - Mechanical torque transmission to both rear wheels via a transverse axle.  
  - Differential 2WD rear configuration, enhancing stability and reducing mechanical complexity.

- **Front Servo Motor:**  
  Controls steering by adjusting the angle of the front wheels with precision, enabling tight turns and fine maneuvers—crucial for navigating curves or narrow paths.

- **Chassis Design:**  
  The vehicle features a high chassis, offering several advantages:  
  - Increased ground clearance to avoid interference with track elements.  
  - Sufficient space for modular sensor mounting and internal wiring.  
  - Facilitates maintenance and quick testing during competition.

- **Power System:**  
  Electrical separation between sensors and motors. A regulated power source (3V–6V) ensures stable supply and prevents voltage drops that could affect sensor accuracy or motor control.


🎯 **Sensor System and Environmental Perception**

The perception system is designed to provide reliable environmental recognition and autonomous decision-making, combining sensors of different types:

- **3 HC-SR04 Ultrasonic Sensors:**  
  Positioned at the front and sides (left and right), they detect obstacles and walls using sound wave reflection, triggering automatic evasive responses.

- **2 Infrared Reflection Sensors (IR):**  
  Used for detecting high-contrast lines (black/white), essential for alignment, edge detection, and path correction.

- **1 Color Sensor (rear zone):**  
  Detects specific colors (e.g., an orange line) to identify environmental events such as lap counting or recognition of special circuit zones.



🧠 **Programming Logic and Autonomous Behavior**

The vehicle’s programming is based on a finite state machine (FSM) architecture, enabling robust autonomous control in various environmental scenarios:

- **Obstacle Avoidance:**  
  Ultrasonic sensors trigger turning, reversing, or stopping decisions when walls or nearby objects are detected.

- **Stall Recovery:**  
  The robot detects stuck conditions (no progress over X cycles) and executes pivot or reverse routines to regain movement.

- **Efficient Turning Maneuvers:**  
  Thanks to the steering servo and weight distribution, the robot achieves precise turns with a smaller turning radius—ideal for sharp path changes.

- **Color-Based Event Handling:**  
  The rear sensor allows the robot to change behavior upon detecting visual markers (colored lines or zones), such as initiating lap counting.


⚙️ **Code Architecture**

The code is organized into independent functional modules, following best practices in embedded programming:

- **Sensor Module:**  
  Continuous data reading and filtering (moving averages) to improve accuracy under noisy conditions.

- **Actuator Controllers:**  
  PWM signal generation to regulate motor speed and steering servo angle.

- **Navigation and FSM Logic:**  
  Real-time environment evaluation to trigger state transitions (e.g., from “free movement” to “obstacle detected”).

- **Safety and Recovery System:**  
  Monitors conditions like collisions, invalid readings, or lack of movement, and activates emergency routines if needed.


🛠️ **Mechanical Design and Assembly**

- **Plastic Gearbox:**  
  The main motor is coupled to a lightweight gearbox that reduces speed and increases torque, allowing more precise robot movement.

- **High Chassis:**  
  Provides greater tolerance to minor terrain irregularities, facilitates multi-level sensor installation, and improves internal ventilation.

- **Weight Distribution:**  
  Weight is concentrated on the rear axle (driven wheels), improving traction and reducing the risk of skidding or tipping during fast maneuvers.

- **Sensor Mounting:**  
  Strategically distributed to cover blind spots and avoid interference between different signal types (sound vs. light).

We worked on the design and electronic simulation in Tinkercad, an online platform that allowed us to virtually build the vehicle’s circuit and test the components before assembling them physically. Thanks to this tool, we could experiment with different wiring configurations between the Arduino, motors, H-bridge, servomotor, and other elements, which helped us reduce errors during the actual assembly. This led to a more precise and efficient integration of the electronic system, making it easier to control and move the robot.

And so, in the midst of all that excitement, the name of our team was born: TecnoTitans. We wanted a name that reflected both our passion for technology and the strength and determination with which we faced every challenge. “Tecno” for the technological focus of our project, and “Titans” as a symbol of power, effort, and teamwork. We quickly identified with that name, and since then, we carry it with pride.

Beyond the technical result, this project meant a lot to us. We learned to solve real problems, make decisions as a team, and apply our knowledge to something tangible. In the end, we managed to build an agile, reliable, and functional autonomous vehicle—and most importantly, we did it together, with effort, learning, and a deep passion for technology.

## **Conclusion**

This project integrates the fundamentals of mechatronic engineering—electronics, programming, and mechanical design—focused on building a stable, functional, and competitive autonomous vehicle.

Thanks to its modular architecture, both physically and in software, the system is easily scalable, allowing for quick adjustments, improvements, or repairs—crucial during competition.

We are confident that this design meets the objectives of the Future Engineers category and reflects the effort, creativity, and technical rigor of the participating team.
