/// File Name:  buzzer.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to include module support board buzzer functionality

#ifndef BUZZER_H
#define BUZZER_H

#include "mbed.h"   // local library file
#include <iostream> // standard library file

// Define buzzer pin(s)
#define BUZZER_PIN PB_13

class buzzer
{
public:
    // specify the different octave registers for tones which are played by the onboard buzzer
    typedef enum
    {
        LOWER_OCTAVE,
        MIDDLE_OCTAVE,
        HIGHER_OCTAVE
    } OCTAVE_REGISTER;

    // initialise the buzzer object
    buzzer(PinName p = BUZZER_PIN) : _buzzer(p)
    {
        volatile double T = periodForNote_us("C");
        _buzzer.period_us(T);
        rest();
    }

    // this function plays a user-specified tone when the user enters a given note and octave register in the arguments
    void playTone(const char *note, OCTAVE_REGISTER octave = MIDDLE_OCTAVE)
    {
        _buzzer.period_us(periodForNote_us(note, octave));
        _buzzer.write(0.5f);
    }

    // this function terminates the tone being played by setting the power of the buzzer to 0.
    void rest()
    {
        _buzzer.write(0.0);
    }

protected:
    PwmOut _buzzer;
    // this function allows the user to specify the duration of the tone. this takes the tone not and octave register as input arguments.
    double periodForNote_us(const char *note, OCTAVE_REGISTER octave = MIDDLE_OCTAVE)
    {
        volatile uint8_t idx = offsetForNote(note);
        volatile double f = note_freq[idx];

        switch (octave)
        {
        case LOWER_OCTAVE:
            f *= 0.5;
            break;
        case MIDDLE_OCTAVE:
            break;
        case HIGHER_OCTAVE:
            f *= 2.0;
            break;
        default:
            break;
        }
        return 1000000.0 / f;
    }

private:
    // create an offest parameter to be used by functions in this class only :)
    uint8_t offsetForNote(const char *noteStr)
    {
        uint8_t res;
        switch (noteStr[0])
        {
        case 'A':
            res = (noteStr[1] == '#') ? 1 : 0;
            break;
        case 'B':
            res = 2;
            break;
        case 'C':
            res = (noteStr[1] == '#') ? 4 : 3;
            break;
        case 'D':
            res = (noteStr[1] == '#') ? 6 : 5;
            break;
        case 'E':
            res = 7;
            break;
        case 'F':
            res = (noteStr[1] == '#') ? 9 : 8;
            break;
        case 'G':
            res = (noteStr[1] == '#') ? 11 : 10;
            break;
        default:
            res = 0;
            break;
        }

        return res;
    }

    double note_freq[12] = {
        220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3};
};

#endif