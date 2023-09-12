/// File Name:  hardware.h
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to allow functionality for the critical error hardware - i.e
/// the red led and buzzer

#ifndef HARDWARE_H
#define HARDWARE_H

#include "mbed.h"   // local library file
#include "buzzer.h" // local library file
#include "leds.h"   // local library file
#include <iostream> // standard library file

class hardware
{
private:
    leds led;
    buzzer buzzer;

public:
    // sound the buzzer and light the red led for 30 seconds
    void fireAlarm()
    {
        led.redLED = 1;
        buzzer.playTone("C", buzzer::LOWER_OCTAVE); // Plays noise from buzzer
        wait_us(30000000);  // 30 seconds
        //wait_us(3000000); // Swap for this wait to get a 3 second critical error because 30 seconds is annoying :)
        led.redLED = 0;
        buzzer.rest(); // Stops the buzzer
    }

    void reset() // Resets all
    {
        fireAlarm();
        system_reset();
    }
};

#endif