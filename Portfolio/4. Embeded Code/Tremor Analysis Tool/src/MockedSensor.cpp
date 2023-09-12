/// File Name:  MockedSensor.cpp
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   This file crates a mocked version of the member functions
/// in SensorReader and inherits from SensorReaderBase

#include "SensorReader.h"   // Local library file
#include "MockedSensor.h"   // Local library file
#include <iostream>         // Standard library file
#include <complex>          // Standard library file

#define M_PI 3.14159265358979323846

MockedSensor::MockedSensor()
{
    tim.start();
}

// Returns a sensorData_t containing the float values
MockedSensor::sensorData_t MockedSensor::readSensors(){
    
    MockedSensor::sensorData_t data;

    data.dt = tim.elapsed_time().count();
    float t = data.dt/1000000.0f;

    int freq = 8; 
    int amp = 1;
    
    {
        data.gyro[0] = 0.0f;
        data.gyro[1] = 0.0f;
        data.gyro[2] = 0.0f;

        data.acc[0] = amp * sin(2 * M_PI * t * freq);
        data.acc[1] = 0.0f;
        data.acc[2] = 0.0f;

        data.mag[0] = 0.0f;    
        data.mag[1] = 0.0f;
        data.mag[2] = 0.0f;

        data.pressure = 1.0f;
        data.humidity = 70;
    } 
    return data;
}

//Function to display the I2C addresses of the sensors
void MockedSensor::displayAddresses(){
    printf("HTS221  humidity & temperature    = Mocked\r\n");
    printf("LPS25H  pressure & temperature    = Mocked\r\n");
    printf("LIS3MDL magnetometer              = Mocked\r\n");
    printf("LSM6DS0 accelerometer & gyroscope = Mocked\r\n");
}

//To be called when the imu is at rest to calculate the offsets
void MockedSensor::calculateOffsets(int reps){
    
     // Reset the offsets to 0
    _offsets.ax = 0.0f;
    _offsets.ay  =0.0f;
    _offsets.az = 0.0f;
    _offsets.gx = 0.0f;
    _offsets.gy = 0.0f;
    _offsets.gz = 0.0f;

    tim.reset();
}
