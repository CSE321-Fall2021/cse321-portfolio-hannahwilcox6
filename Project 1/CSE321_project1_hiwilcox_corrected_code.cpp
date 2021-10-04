/** 
  * File:    CSE321_project1_hiwilcox_corrected_code.cpp 
  * 
  * Author1:  Hannah Wilcox (hiwilcox@buffalo.edu)
  * Date:     September 2021 
  * Partner:  I worked alone  
  * Course:   CSE 321 (Real-Time and Embedded Operating Systems) 
  * 
  * Purpose of File: To turn on an LED via press of a button
  * 
  * Modules/Subroutines: N/A
  *
  * Assignment: Project 1 Part 5
  *
  * Inputs: Button
  *
  * Outputs: LED
  *
  * Constraints: N/A
  *
  * Sources: N/A
  *
  */ 

#include "mbed.h"

// Create a thread to schedule an LED to have an on time of 2000 ms and off time 500 ms
// Starting on when a button is pushed
Thread controller;

void toggleLED();           // Function controlling state of LED
DigitalOut blueLED(LED2);   // establish blue LED as an output

// flagLED indicates state of the LED (False = off, True = on)
// flag indicates if the button is pushed
int flagLED, flag = false;

//Reference to interrupt through a button, to allow for triggered events
//references through .rise and .fall
InterruptIn button(BUTTON1);

void buttonPushed();        // Function controlling state of flag
void buttonReleased();      // Function controlling state of flag and flagLED

int main() {
    // start the allowed execution of the thread
    printf("----------------START----------------\n");
    printf("Starting state of thread: %d\n", controller.get_state());
    controller.start(toggleLED); //Start control of LED state
    printf("State of thread right after start: %d\n", controller.get_state());
    button.rise(buttonPushed);
    button.fall(buttonReleased);
    return 0;
}

// toggleLED takes in no inputs and has not outputs
// Purpose is to control the state of our LED (on or off)
void toggleLED() {
    while (true) {
        if(flagLED == false){ //LED is off
            blueLED = !blueLED;
            printf("Blue LED Toggled On"); //you do need to update the print statement to be correct
            thread_sleep_for(2000); //Thread_sleep is a time delay function, causes a 2000 unit delay
            blueLED = !blueLED;
            printf("Blue LED Toggled off"); 
            thread_sleep_for(500); //Thread_sleep is a time delay function, causes a 500 unit delay
        }
    }
}

// buttonPushed takes in no inputs and has no outputs
// Purpose is to set flag to true, indicating that the button has been pressed
void buttonPushed() {
	flag = true;
}

// buttonReleased takes in no inputs and has no outputs
// Purpose is to reset flag to false, and set flagLED to true
// By setting flagLED to true and flag to false, toggleLED sees that the LED is on
// but the button has been released so the LED must turn off
void buttonReleased() {
    if(flag==true){
        flagLED = true;				    
        flag = false; 
    }
}