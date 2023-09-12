#ifndef SensorReaderBase_H
#define SensorReaderBase_H
 
//#include "mbed.h"
#include <array>
#include <stdint.h>
#include <math.h>

#define RAD_TO_DEG_FACTOR  (180.0f/3.14f)

class SensorReaderBase {

public:
    
    // Template struct for holding all sensor values. Temp,Hum,Pres always float. Acc,Gyro,Mag are template
    // X Y and Z defines allow axis to be specified eg .mag[X]
    template <class Type>
    struct sensorData{
        uint32_t dt = 0;
        float temp;
        float humidity;
        float pressure;
        // Use of unions allow array index or dot notation .mag[0]== .mag[X] == .mx
        union{
            Type mag[3];
            struct{
                Type mx,my,mz;
            };
        };
        union{
            Type acc[3];
            struct{
                Type ax,ay,az;
            };
        };
        union{
            Type gyro[3];
            struct{
                Type gx,gy,gz;
            };
        };
    };

    // Shorthands for sensor data types for acc, gyro and mag as float or int32
    typedef struct sensorData<float> sensorData_t;
    typedef struct sensorData<int32_t> sensorDataInt_t;

    struct imuOffsets_t {
        union{
            struct {float ax; float ay; float az;};
            float acc[3];
        };
        union{
            struct {float gx; float gy; float gz;};
            float gyro[3];
        };
        imuOffsets_t() {
            ax = 0; ay = 0; az = 0;
            gx = 0; gy = 0; gz = 0;
        }
    } ;

    // imuAngles_t struct holds acc, gyro and filtered angle
    struct imuAngles_t {
        float ax=0,ay=0,az=0;
        float gx=0,gy=0,gz=0;
        float x=0,y=0,z=0;
        std::array<float,9> asArray() {
            std::array<float,9> dd = {ax,ay,az,gx,gy,gz,x,y,z};
            return dd;
        }
    } ;

public:
    SensorReaderBase(float accelerometer_coeff = 0.18f) { 
        setFilterCoeffs(accelerometer_coeff);
    }

protected:
    imuAngles_t _current_angles;
    float _acc_coeff = 0.02;
    float _gyro_coeff = 0.98;

public:
    // Function to display the I2C addresses of the sensors
    virtual void displayAddresses() = 0;

    // To be called when the imu is at rest to calculate the offsets
    virtual void calculateOffsets(int reps = 200) = 0;

    // Returns a sensorData_t containing the float values
    virtual sensorData_t readSensors() = 0;

    // Set the coefficients for the complementary filter 
    virtual void setFilterCoeffs(float accelerometer_coeff){
        if(accelerometer_coeff<1.0f && accelerometer_coeff>0.0f){
            _acc_coeff = accelerometer_coeff;
            _gyro_coeff = 1.0f - accelerometer_coeff;
        }
    }

    // Returns a imuAngles_t with the acc,gyro and filtered angles using a complementary filter
    virtual imuAngles_t calcAngles(sensorData_t d) {
        imuAngles_t angles;
        
        // Computing accel angles and convert to degrees
        angles.ax = RAD_TO_DEG_FACTOR*(atan2(d.ay, sqrt(d.az * d.az + d.ax * d.ax)));
        angles.ay = RAD_TO_DEG_FACTOR*(-atan2(d.ax, sqrt(d.az * d.az + d.ay * d.ay)));
        angles.az = RAD_TO_DEG_FACTOR*(-atan2(sqrt(d.ax * d.ax + d.ay * d.ay), d.az));
        _current_angles.ax = angles.ax;
        _current_angles.ay = angles.ay;
        _current_angles.az = angles.az;

        // Computing gyro angles
        float dt = (float) d.dt*0.000001f;  //Convert from us to secs
        _current_angles.gx = _current_angles.gx + (d.gx *dt);
        _current_angles.gy = _current_angles.gy + (d.gy *dt);
        _current_angles.gz = _current_angles.gz + (d.gz *dt); 

        //Copy values to the current_angles struct
        angles.gx = _current_angles.gx;
        angles.gy = _current_angles.gy;
        angles.gz = _current_angles.gz;

        // Apply the complementary Filter
        angles.x = (_acc_coeff * angles.ax) +  (_gyro_coeff *(_current_angles.x + (d.gx *dt)));
        angles.y = (_acc_coeff * angles.ay) +  (_gyro_coeff *(_current_angles.y + (d.gy *dt)));
        angles.z = (_acc_coeff * angles.az) +  (_gyro_coeff *(_current_angles.z + (d.gz *dt)));

        // Copy to the output struct
        _current_angles.x = angles.x;
        _current_angles.y = angles.y;
        _current_angles.z = angles.z;

        return angles;
    }

 };

 #endif