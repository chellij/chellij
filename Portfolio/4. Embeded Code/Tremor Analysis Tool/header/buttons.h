/// File Name:  buttons.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to include module support board button functionality

#ifndef BUTTONS_H
#define BUTTONS_H

#include "mbed.h"   // local library file
#include <iostream> // standard library file

// BUTTONS
#define BTN1_PIN PG_0
#define BTN2_PIN PG_1
#define BTN3_PIN PG_2 // No pull down -- this button will have to be handled differently.
#define BTN4_PIN PG_3 // No pull down -- this button will have to be handled differently.
// USER BUTTON
#define BLUE_BUTTON PC_13

    class buttons
    {
    public:
    // Initialise all the pins for the buttons
        buttons(PinName b1 = BTN1_PIN, PinName b2 = BTN2_PIN, PinName b3 = BTN3_PIN, PinName b4 = BTN4_PIN,
         PinName blue = BLUE_BUTTON, PinName  user = USER_BUTTON) : Button1(b1), Button2(b2), Button3(b3, PullDown),
          Button4(b4, PullDown), UserButton(user) 
        {
        }
        DigitalIn Button2, Button3, Button4, UserButton;
        InterruptIn Button1;
    };


#endif
