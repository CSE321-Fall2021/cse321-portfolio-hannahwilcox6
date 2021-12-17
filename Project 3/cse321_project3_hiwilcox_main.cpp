/** 
  * File:    CSE321_project3_hiwilcox_main.cpp 
  * 
  * Author1:  Hannah Wilcox (hiwilcox@buffalo.edu)
  * Date:     December 2021 
  * Partner:  I worked alone  
  * Course:   CSE 321 (Real-Time and Embedded Operating Systems) 
  * 
  * Purpose of File: To control the state of a servo motor, given user input from an ultrasonic sensor.
  * 
  * Modules/Subroutines: N/A
  *
  * Assignment: Project 3 Stage 2
  *
  * Inputs: Ultrasonic Sensor (Echo)
  *
  * Outputs: LED, Servo Motor, Ultrasonic Sensor (Trigger)
  *
  * Constraints: Different LEDs must light up based on state of motor
  *              Motor must rotate 180 degrees when given proper input from sensor
  *              Motor cannot be interrupted while running
  *
  * Sources: https://os.mbed.com/questions/79352/Struggling-with-basic-code-for-a-servo-m/ - Provided by TA in OH 12/8
  *          https://os.mbed.com/handbook/PwmOut - For controlling servo
  *          https://os.mbed.com/components/HC-SR04/ - For controlling ultrasonic sensor
  *          https://os.mbed.com/docs/mbed-os/v6.15/apis/watchdog.html - For watchdog
  *          http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf - For servo understanding
  *          http://web.eece.maine.edu/~zhu/book/lab/HC-SR04%20User%20Manual.pdf - For getting distance math set up for ultrasonic sensor
  */ 

#include "mbed.h"

//Grabbed from a code example provided from a TA in 12/8 office hours
#define MID         1500
#define MIN         1000
#define MAX         2000
#define STEP          50
//Time delay between steps in milliseconds
#define TIME         100

//Helper functions, each function's purpose is explained above its
//definition at the bottom of this file.
void checkStart();
void checkEnd();
void lowerServo();
void raiseServo();
void echoRise();
void echoFall();

//ultrasonic sensor
DigitalOut trigger(PE_2);
InterruptIn echo(PE_4);
Timer timer;
long dist;

//Servo
PwmOut myServo(PA_0);
bool motorRunning = false;

//Synchronization
Mutex mutex; //Used in threads controlling servo, as they both access motorRunning and modify it
Thread t1;   //Used to check if the servo needs to be started
Thread t2;   //Used to check if the servo needs to returning to resting position

//LEDS will be on PE_5 & PE_6 and controlled by bitwise operations!
//PE_5 tells us that the servo isn't running and we are free to "dump" our trash
//PE_6 tells us that the servo is running and we have to wait to "dump" our trash until it's finished

int main()
{
    // 0001 0001
    // Enable ports  E,A
    RCC->AHB2ENR |= 0x11;
    //Outputs: PE_2 PE_5 PE_6
    // 0000 0000 0000 0000 0001 0100 0001 0000
    GPIOE->MODER&=(0xFFFF141F);
    GPIOE->MODER|=0x1410;

    //Inputs: PE_4
    //---- ---- ---- ----  ---- --00 ---- ----
    GPIOE->MODER &= (0xFFFFFCFF);

    //Make watchdog
    Watchdog &watchdog = Watchdog::get_instance();
    myServo.period_ms(20);
    //Start thread that will continously check out distance to start the servo
    t1.start(checkStart);
    t2.start(checkEnd);

    //Code for ultrasonic sensor echo
    echo.rise(&echoRise);
    echo.fall(&echoFall);
    echo.enable_irq();

    //give the dog 2 seconds before a restart if it isnt kicked beforehand
    watchdog.start(20000);

    GPIOE-> ODR |= (0x20); //Turn on Blue LED
    while (true) {
        //The following code retreives the distance from the ultrasonic sensor
        //The trigger is activated to send a pulse, with 10us between
        //The time is grabbed from our echo interruptin, and calculated to be in cm distance.
        trigger = 1;
        wait_us(10);
        trigger = 0;
        float time = timer.elapsed_time().count();
        timer.reset(); //don't forget to reset the timer!
        //Below code prevents a distance from being read in while the servo is running, since the sensor seems to be
        //bratty at times.
        //Keeping the distance above 10 prevents the threads from running the servo endlessly.
        if(motorRunning == true){
            dist = 11;
        }
        else{
            dist = (time/58);
        }
        printf("Dist: %ld cm\r\n",dist);
        thread_sleep_for(400); //Give the sensor a breather
        //Kick the dog incase something gets stuck
        watchdog.kick();
    }
}

//Called on rising edge of echo from ultrasonic sensor
//Starts timer
void echoRise(){
    timer.start();
}

//Called on falling edge of echo from ultrasonic sensor
//Ends timer
void echoFall(){
    timer.stop();
}

//See if the servo needs to be lowered and "dump" if distance is within proper range and by checking flags
//Used by thread t1
void checkStart(){
    while(true){
        if(dist <= 10 && motorRunning == false){
            lowerServo();
        }
    }
}

//See if the servo needs to be returned to it's original position by checking flags
//Used by thread t2
void checkEnd(){
    while(true){
        if(motorRunning == true){
            raiseServo();
        }
    }
}

//Lowers servo when the sensor reads in a proper distanace
//This is the "dumping" of the trash
void lowerServo(){
    mutex.lock();
    printf("Lower\n");
    GPIOE-> ODR &= ~(0x20);
    GPIOE-> ODR |= (0x40);
    myServo.pulsewidth_us(MID);
    //Following code was provided from an example from a TA in Office Hours 12/8/2021
    for (int i=MIN;i<=MAX;i+=STEP){
        myServo.pulsewidth_us(i);
        ThisThread::sleep_for(TIME);
        }
    ThisThread::sleep_for(350ms);
    motorRunning = true;
    mutex.unlock();
}


//Raises servo back to original resting position after it has already been lowered
void raiseServo(){
    mutex.lock();
    printf("Raise\n");
    myServo.pulsewidth_us(MAX);
    for (int i=MAX;i>=MIN;i-=STEP){
        myServo.pulsewidth_us(i);
        ThisThread::sleep_for(TIME);
    }
    ThisThread::sleep_for(350ms);
    motorRunning = false;
    GPIOE-> ODR |= (0x20);
    GPIOE-> ODR &= ~(0x40);
    mutex.unlock();
}
