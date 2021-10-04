-------------------
About
-------------------
Project Description: 
Correcting provided code to be cleaner and easier to read
Contribitor List:
Hannah Wilcox

--------------------
Features
--------------------
-Turns on a blue LED for a certain amount of time upon a button press

--------------------
Required Materials
--------------------
-Nucleo L4R5ZI
-Button
-LED

--------------------
Resources and References
--------------------
-CSE 321 Lecture Slides
-CSE 321 Project Document

--------------------
Getting Started
--------------------
Attach a button and LED to your breadboard and plug in your Nucleo.
Ensure that the correct values are in for button and blueLED as according to your peripherals.
Run the code, on pressing the button you should see the LED turn on and off on a scheduled period

--------------------
CSE321_project1_hiwilcox_corrected_code.cpp:
--------------------
This file includes all needed references to peripherals (button, LED) and a Thread to control the state of an LED via button push.
The code is not entirely bare metal as we use mbed.h to allow for use of elements such as Threads, InterruptIn, DigitalOut 
and other various elements to control the state of an LED.

----------
Things Declared
----------
-blueLED: DigitalOut variable to control an LED as an output
-controller: Thread variable for scheduling of blueLED
-button: InterruptIn variable for detecting state of button 
-flagLED, flag: boolean variables for state of LED and button

----------
API and Built In Elements Used
----------
-mbed.h

----------
Custom Functions
----------
-toggleLED: Function controlling the state of the LED
-buttonPushed: Function that lets toggleLED know the button has been pushed 
-buttonReleased: Function that lets toggleLED know the button has been released 