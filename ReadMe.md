# stupid-mobility-adapter

A repository for various microcontroller breakout boards that can be used to control mobile platforms (Modified Mobility Scooters).

## History of the stupid-mobility-adapter

During the COVID lockowns of 2020/21, it became necessary for me to have a Robotic platform that would work as a testing analogue to the one I didn't have access to at the time. This is how 'Scooterbot' was born. Initially intended as a personal, stipped back platform, liberated from the scrapheap, and built at minimal cost (<£250) the controller was fried and it had no steering column. These were replaced by an atmega 328 microcontroller and two BTS7960 half-bridge drivers. The PWM controlled drivers control the main DC drive motor and a steering motor. In this case, a 24V transaxle motor rated a 300-400W and a 24V linear actuator with feedback via a potentiometer (0-5V). Additionally, the drive motor has a 24V solenoid brake. 

After returning to work after lockdowns. A redesign of the platform I had been working on allowed me to use a modified version of the Scooterbot code to advance the project for work. This involved adding the ability to control a 'Curtis' style motor controller (Essentially three additional output signals. Enable, Direction and Speed (TTL, TTL, 0-5V)).

In addition to having a microcontroller to tell the motors, actuators etc. what to do. We also need to tell the microcontroller what to do. In previous projects, the communication with the microcontroller has been done over Serial connection with a simple communication protocol. With the microcontroller acting as a reciever, the sender was a companion computer (Raspberry Pi 3B+) reading inputs from a bluetooth games controller with custom python code. 

The addition of a companion computer adds a great deal of connectivity and processing power. Unfortunately, it also increases the complexity of the setup, and points of failure. Considering that basic control of the platform is a safety critical system, it is desirable to simplify the design. Additionally, as the prototypes of this project have performed on a variety of hardware, it is desirable to make this hardware intentionally generic, while keeping in mind its intended purpose.

The next version of the controller changed the microcontroller used to the ESP32. This allows us to cut out the companion computer for basic remote control using a bluetooth controller. There is also the addition of wifi, OTA updates using suitable sized compiled firmware and a large number of interface options. Overall simplification along the thinking of 'the best part is no part'. The drawback of this system is a reduction in CPU processing power.  

---

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

---

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

## v3 PCB Problems

- The GND next to the VIN pin on the ESP32 Socket must be isolated
    - REASON: I misread the name when Making the footprint. It's actually CMD ( Something to do with the SPI Falsh chip. If you don't disconnect it, the ESP32 Can't Boot)
- LED 'D1' has the polarity reversed on the silkscreen.
- FB_0 has 5V and GND silkscreen swapped.
- 5V label on FB_0 and FB_1 should be 3V3
- The Board needs reverse polarity protection. Don't ask me how I know.
- A power LED on the 24V input would be useful.
- Should have broken out some power next to communicaton connections (Like I did with the relays)
- The Buzzer footprint needs to be correct. And connected to a pin capable of output :( 
- The Battery divider cannot be used while Wifi is enabled. Only discovered after install :(
    - Note: ADC2 pins cannot be used when Wi-Fi is used. So, if you’re using Wi-Fi and you’re having trouble getting the value from an ADC2 GPIO, you may consider using an ADC1 GPIO instead, that should solve your problem. from https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

### Should have dones;
- A way to adjust the actuator endpoints on the board?
- A way to adjust the potential divider for the raw battery monitor. They're already talking about 48V!

However, we're running out of pins and It's probably time to embrace CAN bus, make the esp32 the 'master' node, then have nodes for the motor driver(s) ;), Actuators and any other custom bits. We should also start to be able to begin to use automotive devices....  

## Version 3.1 ideas

before I've even really finished the v3 hardware design, I've found things that I think it would be remiss of me to plan for. With 20/20 hindsight, some of these should have been included from the beginning.

### Servo PWM control From flight controllers

Control of the vehicle by more common means should be included. The first one that jumps to mind is the PWM contol that usually output by model radio recievers,
such as the ardupilot variety - that this hardware is eventually intendended to coexist alongside.

To implement this, we would need to 

- hand over control to the ardupilot system, taking commands from the autopilot in the form of throttle and steering inputs. 

- converting Servo signals to suitable outputs for the curtis unit / pwm driver / servo / hoverboardwheel / odrive combo that you need. 
https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/

This would currently be controlled by the ps3 controller inputs, allowing the user to select different control methods while keeping the ablilty to contol the platform in case of emergencies or testing. 

### ESP acts as a radio control reciever 

### Servo Input to Flight Controllers

### Ros Input control

Here we are at a point where I need external input. HOw do people want to control all of this?