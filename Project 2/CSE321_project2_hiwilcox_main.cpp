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

int row = 0;
std::string timeRemainingStr = "";
int timeRemaining = 0;
bool flagD, flagRun, flagInput = false;
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
    display.print("0 Min 0 Sec");
    
    //This loop continously updates the LCD display remaining time, and changes which row is being powered
    while (1) {
        //Row 1 Powered
        row = 0; //Rows go from range 0-3
        if(row == 0){
            GPIOE->ODR |= 0x400;
            thread_sleep_for(80);
            GPIOE->ODR &= 0xFFFFFBFF;
            thread_sleep_for(80);
        }
        row += 1;
        if(row == 1){
            GPIOE->ODR |= 0x1000;
            thread_sleep_for(80);
            GPIOE->ODR &= 0xFFFFEFFF;
            thread_sleep_for(80);
        }
        row += 1;
        if(row == 2){
            GPIOE->ODR |= 0x4000;
            thread_sleep_for(80);
            GPIOE->ODR &= 0xFFFFBFFF;
            thread_sleep_for(80);
        }
        row += 1;
        if(row == 3){
            GPIOE->ODR |= 0x8000;
            thread_sleep_for(80);
            GPIOE->ODR &= 0xFFFF7FFF;
        }

        if(flagD == true){
            display.clear();
            display.setCursor(0,0);
            display.print("Enter Time:");
            flagD = false;
            flagInput = true;
        }
        else if(flagRun == true and flagInput == false){
            //timeRemaining += stoi(timeRemainingStr);
            int seconds = timeRemaining / 1000;
            int min = floor(seconds / 60);
            int sec = seconds % 60;
            // std::string text = to_string(min) + " min " + to_string(sec) + " sec";
            // display.clear();
            // display.setCursor(0,0);
            // display.print("Time Remaining:");
            // display.setCursor(0,1);
            // flagRun = false;
            // display.print(text.c_str());
        }
    }
    return 0;
}

void isr_Col1(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 1\n");
    if (row==0){
        queue.call(printf,"found A\n");
        flagRun = true;
    } 
    else if(row ==1){
        queue.call(printf,"found B\n");
        flagRun = false;
    }
    else if(row ==2){
        queue.call(printf,"found C\n");
    }
    else{
        queue.call(printf,"found D\n");
        flagD = true;
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col2(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 2\n");
    if (row==0){
        queue.call(printf,"found 3\n");
        if(flagInput == true){
            timeRemainingStr += "3";
        }
    } 
    else if(row ==1){
        queue.call(printf,"found 6\n");
        if(flagInput == true){
            timeRemainingStr += "6";
        }
    }
    else if(row ==2){
        queue.call(printf,"found 9\n");
        if(flagInput == true){
            timeRemainingStr += "9";
        }
    }
    else{
        queue.call(printf,"found #\n");
        flagInput = false;
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col3(void) { 
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 3\n");
    if (row==0){
        queue.call(printf,"found 2\n");
        if(flagInput == true){
            timeRemainingStr += "2";
        }
    } 
    else if(row ==1){
        queue.call(printf,"found 5\n");
        if(flagInput == true){
            timeRemainingStr += "5";
        }
    }
    else if(row ==2){
        queue.call(printf,"found 8\n");
        if(flagInput == true){
            timeRemainingStr += "8";
        }
    }
    else{
        queue.call(printf,"found 0\n");
        if(flagInput == true){
            timeRemainingStr += "0";
        }
    } 
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

void isr_Col4(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("test Col 4\n");
    if (row==0){
        queue.call(printf,"found 1\n");
        if(flagInput == true){
            timeRemainingStr += "1";
        }
    } 
    else if(row ==1){
        queue.call(printf,"found 4\n");
        if(flagInput == true){
            timeRemainingStr += "4";
        }
    }
    else if(row ==2){
        queue.call(printf,"found 7\n");
        if(flagInput == true){
            timeRemainingStr += "7";
        }
    }
    else{
        queue.call(printf,"found *\n");
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}