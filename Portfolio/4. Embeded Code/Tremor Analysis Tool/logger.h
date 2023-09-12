/// File Name:  logger.h
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file containing class defintions

#ifndef LOGGER_H
#define LOGGER_H

#include "mbed.h"   // local library file
#include "iostream" // standard library file
#include "fstream"  // standard library file
#include "SDcard.h" // standard library file

using namespace std;


class logger {

public:    
    SDcard m_SDcard;
    
    EventQueue log_q;
    EventQueue sd_q;
    Thread log_t;
    Thread sd_t;

    logger();
    ~logger();
    void serial(char[]);
    void write(char[], bool);
};

#endif