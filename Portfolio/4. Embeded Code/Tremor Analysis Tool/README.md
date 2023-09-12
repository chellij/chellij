<h1> Tremor Analysis Tool </h1>

<h3> Nov 2022 </h3>


_**This project was assigned by university as a practical way to learn about embedded C++ code, multi-threading and embedded sensors.**_

Using an STM32F429Zi Nucleo Board and a "Module Support Board" designed by the university, I designed a multi-threaded software solution to detect tremors and calculate the frequency of the movement. The solution contained both a simulated result (when a MEMs board was not connected) and a 

#### Hardware Used:
* Nucleo F429ZI <br><img src = "./f429zi.jpg" height = 300>
* Module Support Board <br><img src = "./module%20support%20board.png" height = 300>  
* ST X-Nucleo-IKS01A1 MEMS board <br><img src = "./x-nucleo-iks01a1.jpg" height = 300>  

#### Software Used:
* Mbed Studio
* Github Desktop
* [Starter Project](https://classroom.github.com/a/wZPq2RWk) (Mbed Studio Project and Workspace)

#### Libraries in use
1. mbed-os.lib
2. x_nucleo_iks01a1_fork.lib
3. libuopmsb.lib

#### Instructions for running this code
- This code contains code for both a physical MEMs board and a mocked sensor. To use the real sensor, uncomment line 24 in main.cpp which defines IKS01A1_CONNECTED.
- The critical error runs a buzzer for thirty seconds. If it is preferred, this can be reduced to three seconds in the `hardware.h` file. 
- If the SD card is inserted or removed while the program is running then press the black button to reset and acknoledge the SD card state change.

#### Functionality
- Black Button
  - resets the programme
- Blue Button
  - used to start the programme
  - recalibrate sensors
- Button A
  - interrupt to test critical error
- Button B  
  - before starting code - reads the most recent SD log file   
