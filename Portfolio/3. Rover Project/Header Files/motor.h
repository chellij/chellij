#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "mbed.h"
 
class Motor
{
public:
    Motor(PinName pinName1, PinName pinName2, PinName pinName3, PinName pinName4);
    void Fwd(float time);
    void Rev(float time);
    void Left(float time);
    void Right(float time);
    void Stop(void);
    int Speed(float speedA, float speedB);
    void Period_in_ms(int msPeriod);
private:
    PwmOut pin1;
    PwmOut pin2;
    PwmOut pin3;
    PwmOut pin4;
};
 
//int motor(float speedA, float speedB);
 
#endif
 