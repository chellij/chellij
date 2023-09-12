#include "SensorReader.h"


SensorReader::SensorReader(uint32_t mask, float accelerometer_coeff)  {
    _mask = mask;
    mems_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15);
    gyroscope = mems_expansion_board->GetGyroscope();
    accelerometer = mems_expansion_board->GetAccelerometer();
    magnetometer = mems_expansion_board->magnetometer;
    humidity_sensor = mems_expansion_board->ht_sensor;
    pressure_sensor = mems_expansion_board->pt_sensor;
    temp_sensor1 = mems_expansion_board->ht_sensor;
    temp_sensor2 = mems_expansion_board->pt_sensor;
    tim.start();
}

// Returns a sensorDataInt_t containing 32-bit signed integers (private)
SensorReader::sensorDataInt_t SensorReader::readSensorsInt(){
    SensorReader::sensorDataInt_t new_data;
    float val;
    int err=0;
    if(_mask & READ_ACC_MASK){
        // Read accelerometer values in mg to int32_t
        err = accelerometer->get_x_axes(new_data.acc);
        if(err!=0){
            printf("Error reading accelerometer\r\n");
        }
    }
    if(_mask & READ_GYRO_MASK){
        // Read gyro values in mg to int32_t
        err = gyroscope->get_g_axes(new_data.gyro); 
        if(err!=0){
            printf("Error reading gyroscope\r\n");
        }
        // Check timer value in us for dt then reset
        //new_data.dt = tim.read_us();
        new_data.dt = tim.elapsed_time().count();
        tim.reset(); 
    }
    if(_mask & READ_MAG_MASK){
        // Read magnetommeter values in mgauss to int32_t
        err = magnetometer->get_m_axes(new_data.mag);
        if(err!=0){
            printf("Error reading magnetometer\r\n");
        }
    }
    if(_mask & READ_TEMP_MASK){
        // Read temperature values in Celsius to float
        err = temp_sensor2->get_temperature(&val);
        if(err!=0){
            printf("Error reading temperature\r\n");
        }
    }
    if(_mask & READ_PRESS_MASK){
        // Read pressure values in mbar to float
        err = pressure_sensor->get_pressure(&new_data.pressure);
        if(err!=0){
            printf("Error reading pressure\r\n");
        }  
    }
    if(_mask & READ_HUM_MASK){
        // Read humidity values in % to float
        err = humidity_sensor->get_humidity(&new_data.humidity);
        if(err!=0){
            printf("Error reading humidity\r\n");
        }
    }
    return new_data;
}

// Returns a sensorData_t containing the float values 
SensorReader::sensorData_t SensorReader::readSensors(){
    // Fetch the data in integer form
    SensorReader::sensorDataInt_t rd = SensorReader::readSensorsInt();
   
    // Gyro, Acc and Mag values converted to floats arrays
    SensorReader::sensorData_t data;

    for(int axis=0;axis<XYZ;axis++){

        if(_mask & READ_GYRO_MASK){
            // Cast to float, convert from mdps to dps and remove offset
            data.gyro[axis] = (((float)rd.gyro[axis])/1000.0f)- _offsets.gyro[axis]; 
        }
        if(_mask & READ_ACC_MASK){
            // Cast to float, convert from mg to g
            data.acc[axis] = (float)rd.acc[axis]/1000.0f;
        }
        if(_mask & READ_MAG_MASK){
            // Cast to float, convert from mgauss to gauss
            data.mag[axis] = (float)rd.mag[axis]/1000.0f;
        }
    }
    if(_mask & READ_PRESS_MASK){
        // Cast to float, convert from mbar to bar
        data.pressure = (float) rd.pressure/1000.0f;
    }

    if(_mask & READ_HUM_MASK){
        // Copy % humidity to new struct
        data.humidity =rd.humidity;
    }

    // Copy dt in us to new struct
    data.dt=rd.dt;

    return data;
}


//Function to display the I2C addresses of the sensors
void SensorReader::displayAddresses(){
    uint8_t id;
    humidity_sensor->read_id(&id);
    printf("HTS221  humidity & temperature    = 0x%X\r\n", id);
    pressure_sensor->read_id(&id);
    printf("LPS25H  pressure & temperature    = 0x%X\r\n", id);
    magnetometer->read_id(&id);
    printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
    gyroscope->read_id(&id);
    printf("LSM6DS0 accelerometer & gyroscope = 0x%X\r\n", id);
}

//To be called when the imu is at rest to calculate the offsets
void SensorReader::calculateOffsets(int reps){
    float ax=0.0f,ay=0.0f,az=0.0f,gx=0.0f,gy=0.0f,gz=0.0f;
    
     // Reset the offsets to 0
    _offsets.ax = 0.0f;
    _offsets.ay  =0.0f;
    _offsets.az = 0.0f;
    _offsets.gx = 0.0f;
    _offsets.gy = 0.0f;
    _offsets.gz = 0.0f;

    // Read the sensors a bunch of times,sum the values and calculate an average
    for(int i=0;i<reps;i++){
        sensorDataInt_t data;
        accelerometer->get_x_axes(data.acc);
        gyroscope->get_g_axes(data.gyro); 

        ax+=data.ax/1000.0f;
        ay+=data.ay/1000.0f;
        az+=data.az/1000.0f;
        gx+=data.gx/1000.0f;
        gy+=data.gy/1000.0f;
        gz+=data.gz/1000.0f;
    }
    // Calculate the average values and save to member variables
    _offsets.ax = ax/reps;
    _offsets.ay = ay/reps;
    _offsets.az = az/reps;
    _offsets.gx = gx/reps;
    _offsets.gy = gy/reps;
    _offsets.gz = gz/reps;

    // Reset the saved data
    _current_angles.ax = RAD_TO_DEG_FACTOR*(atan2(_offsets.ay, sqrt(_offsets.az * _offsets.az + _offsets.ax * _offsets.ax)));
    _current_angles.ay = RAD_TO_DEG_FACTOR*(-atan2(_offsets.ax, sqrt(_offsets.az * _offsets.az + _offsets.ay * _offsets.ay)));
    _current_angles.az = RAD_TO_DEG_FACTOR*(-atan2(sqrt(_offsets.ax * _offsets.ax + _offsets.ay * _offsets.ay), _offsets.az));
    _current_angles.gx = 0.0f;
    _current_angles.gy = 0.0f;
    _current_angles.gz = 0.0f;
    _current_angles.x = _acc_coeff * _current_angles.ax;
    _current_angles.y = _acc_coeff * _current_angles.ay;
    _current_angles.z = _acc_coeff * _current_angles.az;
    printf("Offsets Calculated\n");
    tim.reset();

}


