/// File Name:  SDcard.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to include module support board sd card read/ write functionality

#ifndef SDCARD_H
#define SDCARD_H

#include "mbed.h"   // local library file
#include "iostream" // local library file
using namespace std;
#include "fstream"  // local library file
#include "SDBlockDevice.h"  // local library file
#include "FATFileSystem.h"  // local library file

class SDcard {

public:
    bool validSD;

    SDcard();
    ~SDcard();
    int new_file();
    int write_sdcard(char txt[]);
    int read_sdcard();
    void reset();
};

#endif