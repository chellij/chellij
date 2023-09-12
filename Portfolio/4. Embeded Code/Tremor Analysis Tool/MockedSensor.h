/// File Name:  MockedSensor.h
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   header file to allow the implementation of the mocked sensor 

#ifndef MockedSensor_H
#define MockedSensor_H
 
#include "mbed.h"       // local library file
#include "SensorReaderBase.h"   // local library file
#include <math.h>       // standard library file

class MockedSensor : public SensorReaderBase {

private:
    Timer tim;
    
protected:
    uint32_t _mask;
    imuOffsets_t _offsets;

public:
    MockedSensor();
    // Function to display the I2C addresses of the sensors
    virtual void displayAddresses();

    // To be called when the imu is at rest to calculate the offsets
    virtual void calculateOffsets(int reps = 200);

    // Returns a sensorData_t containing the float values
    virtual sensorData_t readSensors();  
};
#endif