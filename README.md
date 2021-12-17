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

The project works through input from an ultrasonic sensor, and output to LEDs and a micro servo motor. On each read of 10cm or less from the ultrasonic sensor, the servo motor will rotate to "dump" trash by rotating 90 degrees and returning back to its resting state. Different colored LEDs light up based on if the servo is moving or free to be activated to move.
