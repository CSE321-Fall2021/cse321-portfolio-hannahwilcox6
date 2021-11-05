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

//The following functions are made for detecting inputs from different columns
//I structured them columns 1-4, with column 1 starting on the right side of the keypad
//EX: Column 1 includes the buttons [A,B,C,D]
void isr_Col1(void);
void isr_Col2(void);
void isr_Col3(void);
void isr_Col4(void);
void updateTime(void);

// These intergers keep track of multiple items throughout the code
// row: which row is currently being powered on the keypad
// inputIndex: where in the char * we are for inputting time
// minutes: minutes remaining on the timer
// seconds: sceonds remaining on the timer
// sec1: first digit of remaining second
// sec2: second digit of remaining second
int row, inputIndex, minutes, seconds, sec1, sec2 = 0;

Ticker t1; //Ticker schedules when the LCD display should update

// The following sets up a char * for remaining time in the LCD display
const char* s = "";
char* timeRemainingStr = (char*)malloc(strlen(s)+1);

// These flags keep track of multiple items throughout the code:
// flagD: If D has been pressed
// flagRun: If the countdown should be counting down or not
// flagInput: If the user is inputting a time
// flagSet: If the code needs to update the display to show a time needs to be entered
// flagReached: If the countdown has finished
bool flagD, flagRun, flagInput, flagSet, flagReached = false;

// Event Queue object to allow for print statements in the isr function
EventQueue queue(32*EVENTS_EVENT_SIZE);

// Setting up the LCD display
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
    //PE 8, 9, 7 to 01 for output mode
    // 0000 0000 0000 0101 0100 0000 0000 0000
    GPIOE->MODER&=(0xFFF54FFF);
    GPIOE->MODER|=0x54000;

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

    // Following code will turn on the LCD display with a default to being set with 0 minutes and
    // 00 seconds remaining.
    display.begin();
    display.setCursor(0,0);
    display.print("Time Remaining:");
    display.setCursor(0,1);
    display.print("0 Min 00 Sec");

    // This ticker will run every second to update the time remaining for the LCD display
    t1.attach(&updateTime, 1s);
    
    //This loop continously updates the LCD display, and changes which row is being powered
    while (1) {
        row = 0;
        if(row == 0){
            //Row 1 Powered
            GPIOE->ODR |= 0x400;
            thread_sleep_for(50);
            GPIOE->ODR &= 0xFFFFFBFF;
            thread_sleep_for(50);
        }
        row += 1;
        if(row == 1){
            //Row 2 Powered
            GPIOE->ODR |= 0x1000;
            thread_sleep_for(50);
            GPIOE->ODR &= 0xFFFFEFFF;
            thread_sleep_for(50);
        }
        row += 1;
        if(row == 2){
            //Row 3 Powered
            GPIOE->ODR |= 0x4000;
            thread_sleep_for(50);
            GPIOE->ODR &= 0xFFFFBFFF;
            thread_sleep_for(50);
        }
        row += 1;
        if(row == 3){
            //Row 4 Powered
            GPIOE->ODR |= 0x8000;
            thread_sleep_for(50);
            GPIOE->ODR &= 0xFFFF7FFF;
        }

        // This conditional runs if the user has pressed D
        // Sets LCD display to prompt user to enter a time, sets the input flag to true
        // Resets LEDS in case the time was previously reached, resets all other flags 
        if(flagD == true){
            if(flagReached== true){
                GPIOE->ODR &= ~0x200;
                GPIOE->ODR &= ~0x80;
                flagReached = false;
            }
            display.clear();
            display.setCursor(0,0);
            display.print("Enter Time:");
            flagD = false;
            flagInput = true;
            flagSet = false;
            flagRun = false;
        }
        // This conditional runs if the countdown has finished, turns on multiple LEDS and updates LCD display
        // Sets flagRun to false, since we aren't running anymore
        else if(flagReached == true){
            display.clear();
            display.setCursor(0,0);
            display.print("Times Up!");
            //Turn on 2 LEDS
            GPIOE->ODR |= 0x200;
            GPIOE->ODR |= 0x80;
            flagRun = false;
        }
        // This conditional runs if this is the start of a newly input countdown
        else if(flagRun == true and flagInput == false and flagSet == true){
            //Following sets up the seconds and minutes to be put on the display
            sec1 = (timeRemainingStr[2])-'0';
            sec2 = (timeRemainingStr[3])-'0';
            int min = (timeRemainingStr[0]);
            minutes = min - '0';
            seconds = ((sec1)*10) + (sec2);
            const char* c = " min  sec";
            char* text = (char*)malloc(strlen(c)+1);
            //Set up the char* for the display
            text[0] = min;
            text[1] = ' ';
            text[2] = 'm';
            text[3] = 'i';
            text[4] = 'n';
            text[5] = ' ';
            text[6] = sec1+'0';
            text[7] = sec2+'0';
            text[8] = ' ';
            text[9] = 's';
            text[10] = 'e';
            text[11] = 'c';
            text[12] = '\0';
            //Run the display
            display.clear();
            display.setCursor(0,0);
            display.print("Time Remaining:");
            display.setCursor(0,1);
            //sets flagSet to false since we're done initializing the LCD
            flagSet = false;
            //reset input index incase another time is inputted
            inputIndex = 0;
            display.print(text);
        }
        // This conditonal runs to update the display during a countdown
        else if(flagRun == true and flagInput == false and flagSet == false){
            const char* c = " min  sec";
            char* text = (char*)malloc(strlen(c)+1);
            //fills in text for the LCD display
            text[0] = minutes + '0';
            text[1] = ' ';
            text[2] = 'm';
            text[3] = 'i';
            text[4] = 'n';
            text[5] = ' ';
            text[6] = sec1 + '0';
            text[7] = sec2 + '0';
            text[8] = ' ';
            text[9] = 's';
            text[10] = 'e';
            text[11] = 'c';
            text[12] = '\0';
            display.clear();
            display.setCursor(0,0);
            display.print("Time Remaining:");
            display.setCursor(0,1);
            //reset input index incase another time is inputted
            inputIndex = 0;
            display.print(text);
        }
    }
    return 0;
}

// The following ISR functions operate to tell us which button the user has pressed
// On each press an LED lights up and turns back off
// Depending on the button press we may have to change the behavior of the timer
// The char num in the functions for columns 2-4 controls which number was pressed

// ISR function for column 1 [A,B,C,D]
void isr_Col1(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("Col 1\n");
    if (row==0){
        queue.call(printf,"found A\n");
        //Run the timer
        flagRun = true;
    } 
    else if(row ==1){
        queue.call(printf,"found B\n");
        //Stop the timer
        flagRun = false;
    }
    else if(row ==2){
        queue.call(printf,"found C\n");
    }
    else{
        queue.call(printf,"found D\n");
        //Get user input
        flagD = true;
    }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

// ISR function for column 2 [3,6,9,#]
void isr_Col2(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("Col 2\n");
    char num = ' ';
    if (row==0){
        num = '3';
    } 
    else if(row ==1){
        num = '6';
    }
    else if(row ==2){
        num = '9';
    }
    else{
        queue.call(printf,"found #\n");
    }
    // If we're inputting a time, this properly updates the timeRemainingStr with the current num
    // Has the LCD display update if every input for time has been entered (3 numbers)
    if(flagInput == true){
            timeRemainingStr[inputIndex] = num;
            // The following adds the : for m:ss 
            if(inputIndex == 0){
                timeRemainingStr[inputIndex + 1] = ':';
                inputIndex += 1;
            }
            inputIndex += 1;
            if(inputIndex == 4){
                flagInput = false;
                flagSet = true;
            }
        }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

// ISR function for column 3 [2,5,8,0]
void isr_Col3(void) { 
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("Col 3\n");
    char num = ' ';
    if (row==0){
        num = '2';
    } 
    else if(row ==1){
        num = '5';
    }
    else if(row ==2){
        num = '8';
    }
    else{
        num = '0';
    } 
    // If we're inputting a time, this properly updates the timeRemainingStr with the current num
    // Has the LCD display update if every input for time has been entered (3 numbers)
    if(flagInput == true){
            timeRemainingStr[inputIndex] = num;
            // The following adds the : for m:ss 
            if(inputIndex == 0){
                timeRemainingStr[inputIndex + 1] = ':';
                inputIndex += 1;
            }
            inputIndex += 1;
            if(inputIndex == 4){
                flagInput = false;
                flagSet = true;
            }
        }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}


// ISR function for column 4 [1,4,7,*]
void isr_Col4(void) {
    GPIOE->ODR |= 0x100; //Turn on an LED
    printf("Col 4\n");
    char num = ' ';
    if (row==0){
        num = '1';
    } 
    else if(row ==1){
        num = '4';
    }
    else if(row ==2){
        num = '7';
    }
    else{
        queue.call(printf,"found *\n");
    }
    // If we're inputting a time, this properly updates the timeRemainingStr with the current num
    // Has the LCD display update if every input for time has been entered (3 numbers)
    if(flagInput == true){
            timeRemainingStr[inputIndex] = num;
            // The following adds the : for m:ss 
            if(inputIndex == 0){
                timeRemainingStr[inputIndex + 1] = ':';
                inputIndex += 1;
            }
            inputIndex += 1;
            if(inputIndex == 4){
                flagInput = false;
                flagSet = true;
            }
        }
    GPIOE->ODR&=~(0x100); //Turn off an LED
}

// This function is called by our ticker, and runs every seconds
// Checks if the countdown needs to be updated and if so, properly updates it for
// the LCD display to run in main()
void updateTime(void){
    if(flagRun == true and flagInput == false and flagSet == false){
            seconds -= 1;
            // If we're at the end of the countdown
            if(seconds <= 0 and minutes <= 0){
                seconds = 0;
                minutes = 0;
                flagReached = true;
            }
            // If we need to update from going down a minute
            // Keeps it from displaying negative seconds or the wrong minute
            if(seconds < 0 and minutes >= 1){
                minutes -= 1;
                seconds = 59;
            }
            //Updates sec1 and sec2 for LCD display input
            sec1 = int(seconds / 10);
            sec2 = seconds % 10;
        }
}