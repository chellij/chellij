/// File Name:  logger.cpp
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/* File Description: Logger takes all log commands and sends them to the sd card or serial
 * depending on if the SD card is inserted
 */


#include "logger.h" // local library file
#include <cstdint>  // standard library file
#include <cstring>  // standard library file

// logger constructor
logger::logger(){
    log_t.start(callback(&log_q, &EventQueue::dispatch_forever));
    //log_t.set_priority(osPriorityAboveNormal); // set this if prioritising logging over matrix
    sd_t.start(callback(&sd_q, &EventQueue::dispatch_forever));
    //sd_t.set_priority(osPriorityAboveNormal); // set this if prioritising logging over matrix
}

// logger deconstructor
logger::~logger(){}

// This function will output to the serial terminal
void logger::serial(char txt[]){
    cout << txt;
}

// This function will forward the character array passed in to either the SD card (if inserted) or to the serial function
// the choice is given to send the char array directly to the serial terminal with the bool serial
void logger::write(char txt[], bool serial){
    if (!serial && m_SDcard.validSD){
        sd_q.call(callback(&m_SDcard, &SDcard::write_sdcard), txt); 
    }
    else {
        logger::serial(txt);
    }
}