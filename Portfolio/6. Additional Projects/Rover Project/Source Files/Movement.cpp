#include "mbed.h"
#include "motor.h"
#include "UltraSonic.h"
#include "clickers.h"
//These externs allow variables and functiones to be used that in another folder/library 
extern bool rStopped, fStopped;
extern Motor Wheel;
extern int HallState;
extern void dist();
extern Serial pc;
int i = 0;//This variable is used to control an if statement
int HallState = 0;//This is used to select which case the switch case will fall in to
void Direction (){//This function will control the movement of the buggy
    switch(HallState)
            {
            case 0:
                if(fStopped == false){
                    dist();//Get the distance
                    if (i<1)//Set the wheel speed only once to save processing
                    {
                        Wheel.Speed(0.65,0.65);//forward 65%
                        pc.printf("Do this once\n\r");
                        i++;
                    }
                    FwdStop();//Check if the buggy has been told to stop
                }
                else if (fStopped == true)//If the buggy has been stopped,change state
                    HallState = 1;//Cange state
                break;
            case 1:
                if(fStopped == true && rStopped == false){//Make the buggy go backwards 
                    Wheel.Speed(-0.65,-0.65);//backward 65%
                    RevStop();//Check if the buggy has benn told to stop going backwards
                    dist();//Check distance
                }
                else if(fStopped == true && rStopped == true)//If the buggy has been stopped,change state
                    HallState = 2;//Cange state
                break;
            case 2:
                Wheel.Speed(0.65,0.65);//forward 65%
                wait_ms(250);
                Wheel.Speed(0.65,-0.65);//Right 65%
                wait_ms(500);
                Wheel.Speed(0.65,0.65);//forward 65%
                wait_ms(500);
                Wheel.Speed(-0.65,0.65);//Left 65%
                wait_ms(500);
                Wheel.Stop();//Stop wheels
                HallState = 3;//Change state
                break;
            case 3:
            {
                Wheel.Speed(-0.65,-0.65);//backward 65%
                RevStop();//Check if the buggy has benn told to stop going backwards
                dist();//Check distance
                HallState = 4;//Change state
            }
            case 4:
                //Reset all variables back to what they were at the beginnig so the code can restart
                i = 0;
                rStopped = false;
                fStopped = false;
                HallState = 0;
            }    
}
