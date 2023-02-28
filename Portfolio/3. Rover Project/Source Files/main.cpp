#include "mbed.h"
#include "motor.h"
#include "UltraSonic.h"
#include "tunes.h"
#include "clickers.h"
#include "Movement.h"
#define TIME_PERIOD 2             //Constant compiler Values here 2 equates to 2ms or 500Hz base Frequency
#define DUTY 0.9                  //DUTY of 1.0=100%, 0.4=40% etc.,
 
DigitalIn microswitch1(D4);       //Instance of the DigitalIn class called 'microswitch1'
DigitalIn microswitch2(D3);       //Instance of the DigitalIn class called 'microswitch2'
Motor Wheel(D13,D11,D9,D10);      //Instance of the Motor Class called 'Wheel' see motor.h and motor.cpp 
DigitalIn myButton(USER_BUTTON);  //USER_BUTTON is the Blue Button on the NUCLEO Board
DigitalOut led(LED3);             //LED1 is the Green LED on the NUCLEO board
                                  //N.B. The RED LED is the POWER Indicator
                                  //and the Multicoloured LED indicates status of the ST-LINK Programming cycle
 
Serial pc(USBTX,USBRX);           //Instance of the Serial class to enable much faster BAUD rates then standard 9600 i.e. 115200
                                  //This is Pseudo RS232 over USB the NUCLEO will appear as a COMx Port see device Manager on PC used
                                  //Use PuTTY to monitor check COMx and BAUD rate (115200)
int mm; //This variable will hold the distance in mm between the buggy and the object infront of it 
bool fStopped, rStopped = false;// these variables are used to determine weather the buggy has been stopped, either from going forwards or backwards
 
void dist(){//This function will get the distance in mm and set the mm variable to this value
    ultra_sonic_distance();
    mm = (int)GetDistance();
}
int main ()
{
    pc.baud(9600);               //BAUD Rate to 9600
    pc.printf("ROCO104 Demonstration Robot Buggy Plymouth University 2018/19\n\r");
    Wheel.Period_in_ms(TIME_PERIOD);//Set frequency of the PWMs
    Wheel.Stop();
    
    close_encounter(1);     //tune to play Announce start!
    //twinkle(1);           //see tunes.h for alternatives or make your own!
    //jingle_bells(1);
    
    while(myButton==0)
    {                       //Wait here for USER Button (Blue) on Nucleo Board (goes to zero when pressed)
        led=0;              //and flash green LED whilst waiting
        wait(0.1);
        led=1; 
        wait(0.1);
        //Test Microswitches with two different tones see tunes.cpp tunes.h or flash (led1 and led2) onboard LEDs
    }
    Wheel.Period_in_ms(2);//Set frequency of the PWMs 500Hz
    
    while(true)                             //Repeat the following forever NB always true!
    {
        Direction();// run the Direction function in the Movement library
    } // go back to start of while loop
}  //end of int main()
 
            