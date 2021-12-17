# cse321-portfolio-hannahwilcox6
-------------------
About
-------------------
Includes the work from 3 projects as worked on in CSE 321 (Real-Time and Embedded Operating Systems) with appropriate code to run from any machine on mbed.
Each project was programmed in C++ using mbed studio and a Nucleo L4R5ZI.

-----
Project 1
-----
Turns on a blue LED for a certain amount of time upon a button press

-----
Project 2
-----
Controls the state of an LCD display to count down like an alarm clock from 9 minutes 59 seconds to 0.
User can input a time to count down from by use of a 4x4 matrix keypad, and control the state of the LCD display via inputs of a keypad. 


-----
Project 3
-----
This project was designed by myself within the contraints of using certain elements such as watchdog and various syncrhonization techniques.

My project was modeled to be an automatic trash dispenser, with a purpose of having a hands free way for users to dispose of their garbage. This falls under the autonomous devices design area, and it aids users by not requiring them to have to touch garbage lids or covers when disposing of trash, while also keeping the trash covered so the smell wouldn’t be an issue.
It uses an ultrasonic sensor to read input in from users, and on reading 10cm or less it moves a servo motor to "dump" a trash lid to empty trash into a trash can.
