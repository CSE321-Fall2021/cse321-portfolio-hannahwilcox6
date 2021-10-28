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
#include "lcd1602.h"
#include <cstdio>
#include <string>

//The following functions are made for detecting inputs from different columns
//I structured them columns 1-4, with column 1 starting on the right side of the keypad
//EX: Column 1 includes the buttons [A,B,C,D]
void isr_Col1(void);
void isr_Col2(void);
void isr_Col3(void);
void isr_Col4(void);
void update(void);

int row = 0;
int timeRemaining = 0;
bool flag = false;
//Ticker t1;
EventQueue queue(32*EVENTS_EVENT_SIZE);
CSE321_LCD display(16, 2, LCD_5x10DOTS, PB_9, PB_8);

//Input Pins
//TODO: ENABLE AS INPUTS
InterruptIn Column1(PE_2, PullDown); 
InterruptIn Column2(PE_4, PullDown); 
InterruptIn Column3(PE_5, PullDown);
InterruptIn Column4(PE_6, PullDown);

//Output Pins
//PE10
//PE12
//PE14
//PE15

int main() {
    // 0001 0010
    // Enable ports  E, B
    RCC->AHB2ENR |= 0x12;
    //PE_8 to 01 for output mode
    GPIOE->MODER&=~(0x20000);
    GPIOE->MODER|=0x10000;

    //Enable pins PE10,12,14,15 as outputs
    //0101 0001 0001 0000 0000 0000 0000 0000
    GPIOE->MODER &= (0x511FFFFF);
    GPIOE->MODER |= (0x51100000);

    //Enable pin PE 2,4,5,6 as inputs
    //---- ---- ---- ----  --00 0000 --00 ----
    GPIOB->MODER &= (0xFFFFC0CF);

    //Interrupts for the keypad
    
    //On rise, we'll be reading an input from the keypad and having to determine which button was pressed
    //We also need to turn on the LED!
    Column1.fall(&isr_Col1);
    Column2.fall(&isr_Col2);
    Column3.fall(&isr_Col3);
    Column4.fall(&isr_Col4);

    display.begin();
    display.setCursor(0,0);
    display.print("Time Remaining:");
    display.setCursor(0,1);
    display.print("0 Min 99 Sec");
    display.setCursor(0,1);
    
    //This loop continously updates the LCD display remaining time, and changes which row is being powered
    while (1) {
        //t1.attach(queue.event(update),1);
        //Row 1 Powered
        GPIOE->ODR |= 0x400;
        row %= 4; //Rows go from range 0-3
        thread_sleep_for(80);
        GPIOE->ODR &= 0xFFFFFBFF;
        //Row 2 Powered
        GPIOE->ODR |= 0x1000;
        row += 1;
        thread_sleep_for(80);
        GPIOE->ODR &= 0xFFFFEFFF;
        //Row 3 Powered
        GPIOE->ODR |= 0x4000;
        row += 1;
        thread_sleep_for(80);
        GPIOE->ODR &= 0xFFFFBFFF;
        //Row 4 Powered
        GPIOE->ODR |= 0x8000;
        row += 1;
        thread_sleep_for(80);
        GPIOE->ODR &= 0xFFFF7FFF;
    }
    return 0;
}

void isr_Col1(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 1\n");
    if (row==0){
        queue.call(printf,"found A\n");
    } 
    else if(row ==1){
        queue.call(printf,"found B\n");
    }
    else if(row ==2){
        queue.call(printf,"found C\n");
    }
    else{
        queue.call(printf,"found D\n");
        display.clear();
        display.setCursor(0,0);
        display.print("Enter Time:");
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col2(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 2\n");
    if (row==0){
        queue.call(printf,"found 3\n");
    } 
    else if(row ==1){
        queue.call(printf,"found 6\n");
    }
    else if(row ==2){
        queue.call(printf,"found 9\n");
    }
    else{
        queue.call(printf,"found #\n");
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col3(void) { 
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 3\n");
    if (row==0){
        queue.call(printf,"found 2\n");
    } 
    else if(row ==1){
        queue.call(printf,"found 5\n");
    }
    else if(row ==2){
        queue.call(printf,"found 8\n");
    }
    else{
        queue.call(printf,"found 0\n");
    } 
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col4(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 4\n");
    if (row==0){
        queue.call(printf,"found 1\n");
    } 
    else if(row ==1){
        queue.call(printf,"found 4\n");
    }
    else if(row ==2){
        queue.call(printf,"found 7\n");
    }
    else{
        queue.call(printf,"found *\n");
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void update(void){
    display.clear();
    display.setCursor(0,0);
    display.print("Time Remaining:");
    display.setCursor(0,1);
    display.print("0 Min 99 Sec");
}