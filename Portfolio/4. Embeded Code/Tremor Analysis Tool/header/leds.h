/// File Name:  leds.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to include support for the module support board LED functionality

#ifndef LED_H
#define LED_H

#include "mbed.h"   // local library file
#include <iostream> // standard library file

// LEDs
#define TRAF_RED1_PIN PC_2
#define TRAF_YEL1_PIN PC_3
#define TRAF_GRN1_PIN PC_6
#define TRAF_WHITE_PIN PF_10

class leds
{
public:
    // MSB LEDs
    DigitalOut redLED;
    DigitalOut whiteLED;
    DigitalOut greenLED;
    DigitalOut yellowLED;

    // Initialise all the pins for the LEDs
    leds(PinName l1 = TRAF_RED1_PIN, PinName l2 = TRAF_GRN1_PIN, PinName l3 = TRAF_YEL1_PIN,
         PinName l4 = TRAF_WHITE_PIN) : redLED(l1), greenLED(l2), yellowLED(l3), whiteLED(l4)
    {
    }
};

#endif