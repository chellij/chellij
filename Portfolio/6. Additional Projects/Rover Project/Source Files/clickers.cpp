#include "mbed.h"
#include "motor.h"
#include "clickers.h"
#include "UltraSonic.h"
 
//These externs allow variables and functiones to be used that in another folder/library 
extern DigitalIn microswitch1;       //Instance of the DigitalIn class called 'microswitch1'
extern DigitalIn microswitch2;       //Instance of the DigitalIn class called 'microswitch2'
extern Motor Wheel;
extern Serial pc;
extern int mm;
extern void dist();
extern bool rStopped, fStopped;
 
void RevStop()//This function will stop the buggy and change the bool to true to say the buggy has been stopped from going backwards
{
    if (microswitch1 == true)
    {
        microswitch1.read();//Read the state of the microswitch        
        Wheel.Stop();//Stop wheels 
        rStopped = true;//Change bool state to true 
        dist();//Check distance
    }
}
void FwdStop()//This function will stop the buggy and change the bool to true to say the buggy has been stopped from going forwards
{
    if (mm<= 500)//Do the following if the buggy is less than 500mm to the edge of the arena
    {
        Wheel.Stop();//Stop wheels
        fStopped = true;//Change bool state to true 
        dist();//Check distance
    }
    else
    {
 
        fStopped = false; //Keep bool state false
        dist();//Check distance
    }
}