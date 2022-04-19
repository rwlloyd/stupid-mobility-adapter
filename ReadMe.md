# stupid-mobility-adapter

A repository for various microcontroller breakout boards that can be used to control mobile platforms (Modified Mobility Scooters).

## History of the stupid-mobility-adapter

During the COVID lockowns of 2020/21, it became necessary for me to have a Robotic platform that would work as a testing analogue to the one I didn't have access to at the time. This is how 'Scooterbot' was born. Initially intended as a personal, stipped back platform, liberated from the scrapheap, and built at minimal cost (<£250) the controller was fried and it had no steering column. These were replaced by an atmega 328 microcontroller and two BTS7960 half-bridge drivers. The PWM controlled drivers control the main DC drive motor and a steering motor. In this case, a 24V transaxle motor rated a 300-400W and a 24V linear actuator with feedback via a potentiometer (0-5V). Additionally, the drive motor has a 24V solenoid brake. 

After returning to work after lockdowns. A redesign of the platform I had been working on allowed me to use a modified version of the Scooterbot code to advance the project for work. This involved adding the ability to control a 'Curtis' style motor controller (Essentially three additional output signals. Enable, Direction and Speed (TTL, TTL, 0-5V)).

In addition to having a microcontroller to tell the motors, actuators etc. what to do. We also need to tell the microcontroller what to do. In previous projects, the communication with the microcontroller has been done over Serial connection with a simple communication protocol. With the microcontroller acting as a reciever, the sender was a companion computer (Raspberry Pi 3B+) reading inputs from a bluetooth games controller with custom python code. 

The addition of a companion computer adds a great deal of connectivity and processing power. Unfortunately, it also increases the complexity of the setup, and points of failure. Considering that basic control of the platform is a safety critical system, it is desirable to simplify the design. Additionally, as the prototypes of this project have performed on a variety of hardware, it is desirable to make this hardware intentionally generic, while keeping in mind its intended purpose.

### stupid mobility adapter v2 

The culmination of the work on the ScooterBot and ARWAC v4 platforms. This board is intentionally generic. 
At the time of writing. It is not intended to fabricate this board. However, the PCB design is complete to the point of passing all Electrical Rules Checks

Important Hardware:

- Arduino Nano
- Adafruit MCP4725 12-bit DAC
- ILD615-2 Dual Optoisolator
- MP1584 DCDC Step Down (28V 3A)

Optional:

- 2 x BTS7960 Half-Bridge motor Drivers
- 2 x Electric Linear Actuator
- Mobility Scooter with Curtis style I/O

### stupid mobility adapter v3

As above, but ported to ESP32 with the addition of a Sony Dualshock3 Games contoller. Probably add some more bells and whistles too. 

Possibly:
- CAN Bus Tranciever
- encoder input
- Relays - Done!
- Status LEDS - Done on  the board. Headers for the panel?
- OLED for feedback/errors/messages - This may end up being done with a web interface

and that's just the hardware. Because we're using an esp32, we have the option for adding things like:

- Wifi
- A web interface/feedback
- ROS support (using micro-ROS)