/// File Name:  main.cpp
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   main file where all the magic happens

#include "MockedSensor.h" // local library file
#include "SDcard.h"       // local library file
#include "SensorReader.h" // local library file
#include "hardware.h"     // local library file
#include "leds.h"         // local library file
#include "buttons.h"      // local library file
#include "matrix.h"       // local library file
#include "logger.h"       // local library file
#include <iostream>       // standard library file

using namespace std;

// ---- definitions ---- //
#define newDftAvailable 98  // Flag
#define matrixUpdated 97    // Flag
#define BUFFER_SIZE 64
#define M_PI 3.14159265358979323846
//#define IKS01A1_CONNECTED

// ---- constructors ---- //
buttons m_buttons;
leds m_leds;
matrix m_matrix;
hardware m_hardware;
Timer m_timer;
logger m_log;
#ifdef IKS01A1_CONNECTED
SensorReader sensors;
#else
MockedSensor sensors;
#endif

// ---- threads ---- //
Thread thread_readSens; // used for data gathering
Thread thread_dft; // used for DFT analysis
Thread thread_matrix; // used for barchart on LED matrix

// ---- thread safety ---- //
Mutex safetyLock;

// ---- queues ---- //
EventQueue fixSampling;

class m_dft{
public:    
    double real;
    double img;

    m_dft(){
        real = 0.0;
        img = 0.0;
    }
};

typedef struct {
    float res[64];
    float power[64];
} matValues;

// ---- variables ---- //
int buffer_index = 0;

matValues xyz;

SensorReaderBase::sensorData_t buffer1[64];
SensorReaderBase::sensorData_t buffer2[64];
Mail<SensorReaderBase::sensorData_t, 64> gatherMailbox;
SensorReaderBase::sensorData_t* bufferPointer;
bool _buffer;

// ---- functions and methods ---- //
void critical();
void gather_data();
void process_buffer();
void dft_calc(float[]);
void t1_call();
void matrix_scan();

//*******************************************************************************************************//
int main() {
    // print start up messages to terminal
    m_log.write((char*)"Press button B to read previous log \n", true);
    m_log.write((char*)"or Press the blue button to start \n", true);
    m_log.write((char*)"To test the critical error, press Button A \n", true);

    // if the blue button has not been pressed, allow to user to view previous results
    // write them to the terminal
    while (m_buttons.UserButton == 0) // Wait for blue button
    {
        if (m_buttons.Button2 == 1){
            ThisThread::sleep_for(250ms); //debounce
            m_log.write((char*)"Previous Log: \n", true);
            m_log.m_SDcard.read_sdcard();   // read the results from the sd card. print to terminal
        }
    }; 

    // create a new .txt file to hold all the results and serial outputs and start all the calcs
    m_log.m_SDcard.new_file();      
    sensors.displayAddresses();     // Call this to print the I2C addresses of the sensors
    sensors.calculateOffsets();     // Calculate the gyro offsets (board should be level and stationary)
    sensors.setFilterCoeffs(0.18f); // Set the coefficients for the complementary filter
    
    // start all threads defined in main and prioritise the thread to read the sensors
    m_log.log_q.call(callback(&m_log, &logger::write), (char*)"Setup finished, measurements starting:\n\n", true);
    thread_readSens.start(t1_call);
    thread_readSens.set_priority(osPriorityRealtime); 
    thread_dft.start(process_buffer);
    thread_dft.flags_set(matrixUpdated);
    thread_matrix.start(matrix_scan);

    m_buttons.Button1.rise(&critical);  // press button A for critical error simulation -- see hardware.h to decrease buzz time

    // we don't want to be here
    thread_readSens.join();
    thread_dft.join();
    thread_matrix.join();
    cout << "all threads terminated\n" << endl;
}

//*******************************************************************************************************/
// this function is the ISR function which calls reset on a timeout
void critical()
{
    m_buttons.Button1.disable_irq();
    m_hardware.reset(); 
    m_buttons.Button1.enable_irq();
}

// this is the main sensor reader function and gathers 1 reading of data and sends all of the readings once
// 64 have been recorded and sends them to gatherMailbox
void gather_data() {
    bufferPointer = _buffer ? buffer1 : buffer2;
    if (buffer_index >= 64){
        // send full buffer to mailbox
        osStatus ok  = gatherMailbox.put(bufferPointer);

        if (ok == 0){   // use other buffer, restart filling at index 0
            _buffer =! _buffer;
            buffer_index = 0;
            m_leds.redLED = !m_leds.redLED;
        }
        else {
            printf("ERROR: gatherData couldn't add buffer to mailbox queue");
            critical();
        }
        
    }
    SensorReaderBase::sensorData_t d = sensors.readSensors();
    bufferPointer[buffer_index] = d;
    buffer_index++;
}

// this function processes the buffer :) (and calculates the angles)
void process_buffer() {
    while(1){  
        // MAILBOX BUFFER
        SensorReaderBase::sensorData_t *buffer;
        SensorReaderBase::sensorData_t *buff[64];
        SensorReaderBase::sensorData_t d;
        
        buffer = gatherMailbox.try_get_for(1s);
        if(buffer == nullptr){
            printf("Error Recieving Mailbox 1\n");
            critical();
        }
        else {
            float angles[64];
            for (int i = 0; i < 64; i ++){
                buff[i] = (SensorReaderBase::sensorData_t*)buffer + i; // get individual sensor data arrray from buffer
                d = *buff[i];                                          // use pointer to find sensorData values 
                SensorReaderBase::imuAngles_t a;
                a = sensors.calcAngles(d);
                angles[i] = a.y;
            }
            dft_calc(angles);
        }
    }
}

// this function runs all of the dft calcs across an array of 64 floats
void dft_calc(float wave[BUFFER_SIZE]) {
    float res_change = 0.25;
    float K = 16 / res_change;

    float res = 0;
    m_dft complexMag[(int)K];
    char msg[120];

    uint32_t flags = ThisThread::flags_get();
    if (flags == matrixUpdated) {
        ThisThread::flags_clear(matrixUpdated);
        bool ok = safetyLock.trylock_for(1s);
        if (ok){
            for (int k = 0; k < K; k++) {
                for (int n = 0; n < BUFFER_SIZE; n++) {
                    
                    float cosineVal = cos((2 * n * res * M_PI) / K);
                    float sineVal = sin((2 * n * res * M_PI) / K);

                    complexMag[k].real += wave[n] * cosineVal;
                    complexMag[k].img += wave[n] * sineVal;
                }
                double power = complexMag[k].real * complexMag[k].real;
                xyz.res[k] = res;
                xyz.power[k] = power;
                res += res_change;
                sprintf(msg, " %2.2f Hz\tComplex Magnitude = %.2f + %.2fi\tPower = %.2f\n", res, complexMag[k].real, complexMag[k].img, power);
                m_log.log_q.call(callback(&m_log, &logger::write), msg, false);
            }
            m_leds.yellowLED = !m_leds.yellowLED;//visual representation of dft being completed
        }
        else{
            printf("DFT calculator could not recieve safetyLock lock");
            critical();
        }
        thread_matrix.flags_set(newDftAvailable);
        safetyLock.unlock();
    }
}

// this function runs the event queue for the sampling
void t1_call() {
    fixSampling.call_every(10ms, gather_data); // 8ms chosen as tests concluded it took 7.8ms for the MEMs board to readSensors
    fixSampling.dispatch_forever();
}


// this function plots the barchart on the matrix
void matrix_scan() {
    float power[64];
    float res[64];
    int critvalue = 0;
    while (1) {
        int index;
        int freq_min = 3;
        float p[64];
        float r[64];

        uint32_t flags = ThisThread::flags_get();
        if (flags == newDftAvailable){
            ThisThread::flags_clear(newDftAvailable);
            bool ok = safetyLock.trylock();
            // DATA IN = 16 bins (3 to 6.75 Hz)
            if (ok){
                critvalue = 0;
                for (int i = 0; i < 64; i++) {
                    power[i] = xyz.power[i];
                    res[i] = xyz.res[i];
                }
            }
            else{
                critvalue++;
                if (critvalue > 1000){
                    printf("DFT calculator could not recieve safetyLock lock");
                    critical();
                }
            }   
            safetyLock.unlock();
            thread_dft.flags_set(matrixUpdated);
           
            for (int i = 0; i < 64; i++) {
                if (res[i] == freq_min) {
                index = i;
                break;
                }
            }
            for (int i = 0; i < 16; i++) {
                p[i] = power[i + index];
                r[i] = res[i + index];
            }
        }
        float max = 1000000; // *max_element(p, p + 15);
        float min = 0;       // *min_element(p, p + 15);

        // x = (inValue - minInRange) / (maxInRange - minInRange);
        // height = minOutRange + (maxOutRange - minOutRange) * x
        int height[16];
        for (int i = 0; i < 16; i++) {
            float x = (p[i] - min) / (max - min);
            height[i] = 0 + (7 - 0) * x;
        }

        // null's a bad word sooo
        int nul = 0x00;
        // take the Power level for each column on the LHS
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j <= height[i]; j++) {
                m_matrix.cs = 0;            // send data to Matrix
                m_matrix.spi_matrix.write(0x00);   // COL RHS
                m_matrix.spi_matrix.write(1 << i); // move through each column
                m_matrix.spi_matrix.write(j);      // ROW RHS
                m_matrix.cs = 1; // low to high will effectivelly LATCH the Shift register to
                        // output
            }
        }

        // take the Power level for each column on the RHS
        for (int i = 8; i < 16; i++) {
            for (int j = 0; j <= height[i]; j++) {
                m_matrix.cs = 0;                  // send data to Matrix
                m_matrix.spi_matrix.write(1 << (i - 8)); // move through each column
                m_matrix.spi_matrix.write(0x00);         // COL LHS
                m_matrix.spi_matrix.write(j);            // ROW RHS
                m_matrix.cs = 1; // low to high will effectivelly LATCH the Shift register to output
            }
        }
        m_matrix.cs = 0;
        m_matrix.spi_matrix.write(1 << 9);
        m_matrix.spi_matrix.write(0x00);
        m_matrix.spi_matrix.write(0x00);
        m_matrix.cs = 1;
        ThisThread::sleep_for(3ms);
    }
}

/*
- 1. SENSOR :           MINIMIZE JITTER
- 3. DFT POWER SPECT :  DOUBLE BUFFERED
                        ANALYSIS NEEDS TO BE COMPLETE BEFORE THE BUFFERS ARE SWAPPED AND PROCESS REPEATS
- 4. SD CARD :          INITIAL IMPLEMENTATION W BUFFERING/QUEUE (Data should be buffered in internal memory and only written to the SD card in blocks.) LOG ALL
- 5. MATRIX :           INITIAL IMPLEMENTATION
                        THREADSAFE C++ DRIVER CLASS AND SUITABLE API (application programming interface)
- 6. MIN POWR CONSUMP : Where spin-locks or busywait loops are used, justify why in comments
- 7. NO DEADLOCKS :     MITIGATE AGAINST THREAD STARVATION (using timeouts with any blocking functions) TIMEOUTS -> CRITICAL ERROR
- 8. CRITICAL ERROR :   RESET
                        INTERRUPT BASED INSTEAD OF WHILE LOOP
                        LOG ERRORS TO __SERIAL INTERFACE__
                        MAKE HAPPEN ON ERROR TOO INSTEAD OF JUST TEST :/
*/
