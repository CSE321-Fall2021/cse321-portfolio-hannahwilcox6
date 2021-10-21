/** 
  * File:    CSE321_project2_hiwilcox_main.cpp 
  * 
  * Author1:  Hannah Wilcox (hiwilcox@buffalo.edu)
  * Date:     October 2021 
  * Partner:  I worked alone  
  * Course:   CSE 321 (Real-Time and Embedded Operating Systems) 
  * 
  * Purpose of File: To control the count down of an alarm on an LCD display via user input from a keypad
  * 
  * Modules/Subroutines: N/A
  *
  * Assignment: Project 2 Stage 2
  *
  * Inputs: Keypad
  *
  * Outputs: LED, LCD Display
  *
  * Constraints: LED must turn on for each press on the keypad
  *              LCD Display must display the time remaining from user input on the keypad
  *              LCD Display must be able to stop and start given user input on the keypad
  *
  * Sources: October 5 Lecture Code Example
  *
  */ 

#include "mbed.h"
#include "mbed_events.h"
#include <cstdio>

//The following functions are made for detecting inputs from different columns
//I structured them columns 1-4, with column 1 starting on the right side of the keypad
//EX: Column 1 includes the buttons [A,B,C,D]
void isr_Col1(void);
void isr_Col2(void);
void isr_Col3(void);
void isr_Col4(void);
void LED_Off(void);

int row = 0;
int timeRemaining = 0;
EventQueue queue(32*EVENTS_EVENT_SIZE);


InterruptIn Column1(PB_8);
InterruptIn Column2(PB_9);
InterruptIn Column3(PB_10);
InterruptIn Column4(PB_11);

int main() {
    RCC->AHB2ENR |= 0x6;
    //PC_8 to 01 for output mode
    GPIOC->MODER&=~(0x20000);
    GPIOC->MODER|=0x10000;

    //Interrupts for the keypad
    Column1.rise(&isr_Col1);
    Column2.rise(&isr_Col2);
    Column3.rise(&isr_Col3);
    Column4.rise(&isr_Col4);

    Column1.fall(&LED_Off);
    Column2.fall(&LED_Off);
    Column3.fall(&LED_Off);
    Column4.fall(&LED_Off);
    
    //This loop continously updates the LCD display remaining time
    while (1) {
        if(timeRemaining > 0 ){
            timeRemaining -= 1000; //Remove a second every loop iteration
        }
        else{
            timeRemaining = 0; //This is so that the time isn't in the negative
        }
        if (row == 1) {
            //GPIOC->ODR |= 0x100;
        }
        else {
            GPIOC->ODR&=~(0x100);
        }
        row += 1;
        row %= 4; //Rows go from range 0-3
    }
    return 0; 
}

void isr_Col1(void) {
    GPIOC->ODR |= 0x100; //Turn on an LED
    printf("LED ON\n");
    if (row==0){
        queue.callback(printf("found A\n"));
    } 
    else if(row ==1){
        printf("found B\n");
    }
    else if(row ==2){
        printf("found C\n");
    }
    else{
        printf("found D\n");
    }
}

void isr_Col2(void) {
    GPIOC->ODR |= 0x80; 
    if (row==0){
        printf("found 3\n");
    } 
    else if(row ==1){
        printf("found 6\n");
    }
    else if(row ==2){
        printf("found 9\n");
    }
    else{
        printf("found #\n");
    }
}

void isr_Col3(void) { 
    GPIOC->ODR |= 0x80;
    if (row==0){
        printf("found 2\n");
    } 
    else if(row ==1){
        printf("found 5\n");
    }
    else if(row ==2){
        printf("found 8\n");
    }
    else{
        printf("found 0\n");
    } 
}

void isr_Col4(void) {
    GPIOC->ODR |= 0x80; 
    if (row==0){
        printf("found 1\n");
    } 
    else if(row ==1){
        printf("found 4\n");
    }
    else if(row ==2){
        printf("found 7\n");
    }
    else{
        printf("found *\n");
    }
}

void LED_Off(void){
    GPIOC->ODR&=~(0x100);
    printf("LED OFF\n");
}