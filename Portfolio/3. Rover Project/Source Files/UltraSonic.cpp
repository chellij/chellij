#include "mbed.h"
#include "UltraSonic.h"
 
DigitalOut Trigger(D7);           //Instance of the DigitalInOut class called 'TriggerEcho' WAS D6
DigitalIn  Echo(D2);              //Instance of the DigitalInOut class called 'TriggerEcho'
Timer pulse;                      //Instance of the Timer class called 'pulse' so we can measure timed events
 
 
// Function ultra_sonic_distance() will load the global variable distance with Ultra Sonic Sensor value in mm
// and then send the value to the stdio ouput i.e serial over USB
void ultra_sonic_distance(void)
{
   printf("%dmm \n\r",(int)GetDistance());   
}
  
// Function GetDistance() will return a float value of the Distance from the Ultrasonic Sensor in millimetres typically use as: ‘(float)myDistance=GetDistance();’
float GetDistance()
{                                                       //Function Name to be called
    int EchoPulseWidth=0,EchoStart=0,EchoEnd=0;         //Assign and set to zero the local variables for this function
    Trigger = 1;                                        //Signal goes High i.e. 3V3
    wait_us(100);                                       //Wait 100us to give a pulse width Triggering the Ultrasonic Module
    Trigger = 0;                                        //Signal goes Low i.e. 0V
    pulse.start();                        //Start the instance of the class timer
    pulse.reset();                                      //Reset the instance of the Timer Class
    while(Echo == 0 && EchoStart < 25000){              //wait for Echo to go high
        EchoStart=pulse.read_us();                      //Conditional 'AND' with timeout to prevent blocking!      
    }
    while(Echo == 1 && ((EchoEnd - EchoStart) < 25000)){//wait for echo to return low
        EchoEnd=pulse.read_us();                        //Conditional 'OR' with timeout to prevent blocking!   
    }
    EchoPulseWidth = EchoEnd - EchoStart;               //time period in us
    pulse.stop();                                       //Stop the instance of the class timer
    return (float)EchoPulseWidth/5.8f;                  //calculate distance in mm and return the value as a float
}
 
 
 
