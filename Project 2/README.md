-------------------
About
-------------------
Project Description:
Control the state of an LCD display to count down like an alarm clock from 9 minutes 59 seconds to 0.
User can input a time to count down from by use of a 4x4 matrix keypad, and control the state of the LCD display via inputs of a keypad. 
Contribitor List:
Hannah Wilcox

--------------------
Features
--------------------
-Turns on an LED upon keypad press
-Allows for user to use a keypad to input a time for an LCD display timer
-Allows for user to use a keypad to pause the timer
-Allows for user to use a keypad to resume to timer
-Changes an LCD display to show the state of the countdown updating in real time

--------------------
Required Materials
--------------------
-Nucleo L4R5ZI
-Solderless breadboard
-Jumper cables
-4x4 Matrix keypad
-LEDs
-LCD display

--------------------
Resources and References
--------------------
-CSE 321 Lecture Slides
-CSE 321 Project 2 Document

--------------------
Getting Started
--------------------
Start by attaching wires from pins PE2, PE4, PE5, and PE6 on the Nucleo to the rows 8 7 6 and 5 in that order on the 4x4 matrix keypad.
Next attach wires from pins PE10, PE12, PE14, and PE15 on the Nucleo to the rows 1 2 3 and 4 in that order on the 4x4 matrix keypad.

Then to hook up the LCD display, connect the ground from the display to a ground on the Nucelo and the power on the display to a power on the Nucleo.
Next attach the SDA port on the display to pin PB9 on the Nucleo and the SCL port on the display to pin PB8 on the Nucleo.

Lastly we will set up the LEDS by attaching the pins PE8, PE9 and PE7 on the Nucelo to LEDS on the solderless breadboard.
Each LED will need to be set up with a resistor and there will also need to be a wire connecting from ground on the Nucleo to a ground on the breadboard.

Once all connections are properly made, connect the Nucleo to a machine with this code, clean build, and then run.
From there one should be able to control the LCD display with inputs from the keypad by setting a time for the timer,
starting the timer, and stopping the timer.

--------------------
CSE321_project2_hiwilcox_main.cpp:
--------------------
This file includes all needed references to peripherals (LED, keypad) 
Included code works to print the button press on the keypad and light up an LED

----------
Things Declared
----------
Integers:
-row: which row is currently being powered on the keypad
-inputIndex: where in the char * we are for inputting time
-minutes: minutes remaining on the timer
-seconds: sceonds remaining on the timer
-sec1: first digit of remaining second
-sec2: second digit of remaining second

Booleans:
-flagD: True if D has been pressed
-flagRun: True if the countdown should be counting down or not
-flagInput: True if the user is inputting a time
-flagSet: True if the code needs to update the display to show a time needs to be entered
-flagReached: True if the countdown has finished

InterruptIns:
-Column1: InterruptIn for Column 1 of the keypad
-Column2: InterruptIn for Column 2 of the keypad
-Column3: InterruptIn for Column 3 of the keypad
-Column4: InterruptIn for Column 4 of the keypad

-t1: Ticker schedules every second for when the LCD display should update
-timeRemainingStr: char * for remaining time in the LCD display, populated by characters of the input numbers



----------
API and Built In Elements Used
----------
-mbed.h
-cstudio

----------
Custom Functions
----------
-isr_Col1: Function that keeps track of which button is pressed on column 1 and lights up an LED,
           If A is pushed, the countdown timer is started 
           If B is pushed, the countdown timer is paused 
           If D is pushed, the user is prompted to enter a time in terms of m:ss for the timer to start on the next A press
    
-isr_Col2: Function that keeps track of which button is pressed on column 2 and lights up an LED,
           If any number is pushed and the code is currently taking inputs for a time, append these numbers to timeRemainingStr as needed
           If the code isn't taking inputs for a time, do nothing.

-isr_Col3: Function that keeps track of which button is pressed on column 3 and lights up an LED,
           If any number is pushed and the code is currently taking inputs for a time, append these numbers to timeRemainingStr as needed
           If the code isn't taking inputs for a time, do nothing.

-isr_Col4: Function that keeps track of which button is pressed on column 4 and lights up an LED,
           If any number is pushed and the code is currently taking inputs for a time, append these numbers to timeRemainingStr as needed
           If the code isn't taking inputs for a time, do nothing.

-updateTime: Function to update the remainting time of the countdown, updates the varaibles: minutes, seconds, sec1, sec2
