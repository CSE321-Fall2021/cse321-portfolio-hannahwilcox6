-------------------
About
-------------------
Project Description:

My project will be an automatic trash dispenser, with a purpose of having a hands free way for users to dispose of their garbage. This falls under the autonomous devices design area, and it aids users by not requiring them to have to touch garbage lids or covers when disposing of trash, while also keeping the trash covered so the smell wouldn’t be an issue. 

The project works through input from an ultrasonic sensor, and output to LEDs and a micro servo motor. On each read of 10cm or less from the ultrasonic sensor, the servo motor will rotate to "dump" trash by rotating 90 degrees and returning back to its resting state. Different colored LEDs light up based on if the servo is moving or free to be activated to move.



Contributor List:
- Hannah Wilcox

--------------------
Features
--------------------
- Allows for user to wave their hand in front of an ultrasonic sensor to activate the servo motor
- Rotating servo motor that rotates from a resting position of 0 degrees to 90 degrees.
- Changes which LEDs are lit up based on state of servo motor
- Runs "forever"

--------------------
Required Materials
--------------------
- Nucleo L4R5ZI
- Solderless breadboard
- Jumper cables
- HC-SR04 Ultrasonic Sensor
- LEDs
- SG 90 Micro Servo Motor

--------------------
Resources and References
--------------------
- CSE 321 Lecture Slides
- CSE 321 Project 3 Document
- https://os.mbed.com/questions/79352/Struggling-with-basic-code-for-a-servo-m/
- https://os.mbed.com/handbook/PwmOut
- https://os.mbed.com/components/HC-SR04/
- https://os.mbed.com/docs/mbed-os/v6.15/apis/watchdog.html
- http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
- http://web.eece.maine.edu/~zhu/book/lab/HC-SR04%20User%20Manual.pdf

--------------------
Getting Started
--------------------
Start by attaching the 5V power on the nucleo to the power rail on the breadboard, we will use this to provide both the ultrasonic sensor and the servo motor to 5V power. Then attach a wire from any ground pin on the same side of the breadboard to a ground on the nucleo.

For the ultrasonic sensor, start by attaching the trigger for the sensor to port PE_2, and the echo of the sensor to PE_4. Then connect the ground of the sensor to any ground port on the nucleo and connect the sensor power to the power rail on the breadboard.

Next for the servo motor, connect the ground to any ground pin on the nucleo, the power to the power rail on the breadboard, and attach the PWM to PA_0 on the nucleo.

Lastly for the LEDs, set up two LEDs and partnering resistors on the breadboard on the same side as previous wires were attached. Then attach a wire from one LED to PE_5 on the nucleo, and the other LED to PE_6 on the nucleo. 

--------------------
CSE321_project3_hiwilcox_main.cpp:
--------------------
This file includes all the code to run the system attached from the previous section. Upon running this file in mbed after attaching the nucleo to the machine running the file, the sensor, LEDs, and servo will be active and able to run according to the functionality section.

----------
Things Declared
----------
Declarations for Servo Motor:
- MIN 
    - int used for running the servo motor as the minimum pulsewidth 
- MID 
    - int used or running the serov motor as the mid-point pulsewidth
- MAX 
    - int used for running the servo motor as the maximum pulsewidth
- STEP 
    - int used for running the servo motor as the step pulsewidth between pulses
- TIME 
    - int used for running the servo motor as waiting time in ms between pulses
- myServo 
    - PwmOut used to send pulses to servo motor
- motorRunning 
    - bool flag used to tell when the servo is running or not

Declarations for Ultrasonic Sensor:
- trigger 
    - DigitalOut to send power to trigger on ultrasonic sensor
- echo 
    - InterruptIn for detecting objects in front of the ultrasonic sensor
- timer 
    - Timer used for calculating distance between echo rise and falling edges to calculate distance
- dist 
    - long used for calculating the distance detected in front of the ultrasonic sensor

Synchronization:
- mutex 
    - Mutex used to protect shared resources in both threads
- t1 
    - Thread used for checking in the background if the servo motor needs to be started given some specific distance value and if the motor isn't already running
- t2 
    - Thread used for checking in the background if the servo motor needs to be moved back to its resting position given if the motor is running or not

----------
API and Built In Elements Used
----------
- mbed.h

----------
Custom Functions
----------
* void checkStart()
    * Function used by thread t1 to continuously check if the distance recorded by the ultrasonic sensor is 10cm or less and if the motor is not running. If both of those requirements are fit, then it makes a call to lowerServo(). This serves the purpose of making the servo "dump".
- void checkEnd()
    - Function used by thread t2 to continuously check if the motor is running, and if so will make a call to raiseServo(). This serves the purpose of returning the servo to its resting state.
- void lowerServo()
    - Function used to turn the servo 90 degrees in one direction to "dump". Turns off resting state LED and turns on motor running LED while also setting motorRunning to true.
- void raiseServo()
    - Function used to return servo to resting 0 degrees. Turns off motor running LEd and turns on resting state LED while setting motorRunning back to false.
- void echo_rise()
    - Starts timer on ultrasonic sensor rising edge.
- void echo_fall()
    - Ends timer on utlrasonic sensor falling edge.