/// File Name:  buzzer.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   Header file to include module support board buzzer functionality

#ifndef MATRIX_H
#define MATRIX_H

#include "mbed.h" // local library file

#define M_PI 3.14159265358979323846

// MATRIX
#define LATCH_PIN PB_6
#define OE_PIN PB_12
#define SCK_PIN PC_10
#define MISO_PIN PC_11
#define MOSI_PIN PC_12

class matrix
{
public:
    // LED Matrix
    SPI spi_matrix; // MOSI, MISO, SCLK
    DigitalOut cs;  // Chip Select ACTIVE LOW
    DigitalOut oe;  // Output Enable ACTIVE LOW
    
    matrix() : spi_matrix(MOSI_PIN, MISO_PIN, SCK_PIN), cs(LATCH_PIN), oe(OE_PIN)
    {
    }

private:
    void clearMatrix(void);
    void matrix_scan(void);
};

#endif