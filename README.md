# LJ STAND Code 2018

Code for team LJ STAND's 2018 robots.

LJ STAND competed in the Lightweight Soccer division of the RoboCup Junior 2018 championships in Montreal, Canada.
We successfully achieved first place. Videos of our competition can be found at https://www.youtube.com/watch?v=TCXnbb6RAY0&list=PLo6mctJCslLm9HMXaA4l_UsdaM2-xzUFP

This is our robot firmware, built for our custom robot platform. It made use of 4 Teensy 3.5s, along with an OpenMV camera for image processing.

------------------------------------------------

This project is built using PlatformIO, for the Teensy 3.5 platform.

### Folder Structure
lib: Libraries

master: main program file for the master Teensy

slave_motor: main program file for the motor slave Teensy

slave_sensor: main program file for the sensor slave Teensy

slave_debug: main program file for the debug slave Teensy

camera: program file for the OpenMV camera
