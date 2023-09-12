/// File Name:  SDcard.cpp
/// Group name: Group M
/// Authors:    Rachel Ireland-Jones, Arryn Smith, Michael Snelling
/// Module:     ELEC351
/// File Description:   source file to get the sd card going uwu
#include "SDcard.h"

#define MOSI PB_5   // Master Out Slave In
#define MISO PB_4   // Master In Slave Out
#define SCLK PB_3   // Serial Clock
#define CS PF_3     // Chip Select

// initialise the sd card
SDBlockDevice card(MOSI, MISO, SCLK, CS);

//sd constructor
SDcard::SDcard(){
    int err;
    err = card.init();  //initialise the sd card
    if (err)    // check if we've got a problem - print some messages
    {
        if (err == -5005){
            printf("\nSD Card Not Found ... printing log to serial interface\n");
        }
        else {
             printf("SD Card Not Initialised ... Error %d \n\n", err);
        }
        validSD = false;    // flag that we've got no sd card present bub
    }
    else {
        // we've deffo got an sd card - start prepping for logging
        printf("\nSD Card Found and Initialised ... printing log to SD Card\n\n");
        validSD = true;     // flag that we actually have an sd card (for the logger)
    }
}

//deconstructor
SDcard::~SDcard(){
    //de initialise the card
    card.deinit();
}

// this functions creates a new file on the sd card to hold all serial outputs
int SDcard::new_file(){
    if (validSD)
    {
        FATFileSystem fs("card", &card);
        FILE *fp = fopen("/card/test.txt", "w");    // create a new file
        if (fp == NULL) // we don't have a file ór it can't be found
        {
            error("Could not open or find file for appending\n");
            card.deinit();
            return -1;
        }
        else
        {
            fprintf(fp, "OUTPUT LOG: ELEC351 Coursework Group M _ Test\n");
            fclose(fp);   
            return 0;
        }
    }
    else {
        return -1;
    }    
}

// this function checks whether the test.txt file, where all of the information is being stored,
// is present, open and writes to this file if this condition is satisfied
// if the condition isn't satisfied print to the terminal
int SDcard::write_sdcard(char txt[]){
    if (validSD)
    {
        FATFileSystem fs("card", &card);
        FILE *fp = fopen("/card/test.txt", "a");
        if (fp == NULL)
        {
            error("Could not open or find file for appending\n");
            card.deinit();
            return -1;
        }
        else
        {
            fprintf(fp, "%s", txt);

            DIR *d = opendir("/card/");
            while (true)
            {
                struct dirent *e = readdir(d);
                if (e == NULL)
                {
                    break;
                }
            }
            int err = closedir(d);
            fclose(fp);   
            return 0;
        }
    }

    else {
        cout << txt << endl;
        return 0;
    }
}

// this function opens the test.txt file in a read-only format and reads the serial outputs
// and prints these values to ther terminal
int SDcard::read_sdcard(){
    if (validSD)
    {
        FATFileSystem fs("card", &card);
        FILE *fp = fopen("/card/test.txt", "r");
        if (fp == NULL)
        {
            error("Could not open or find file for read\n");
            card.deinit();
            return -1;
        }
        else
        {
            //Put some text in the file...
            char buff[100];
            buff[99] = 0;
            while (!feof(fp))
            {
                fgets(buff, 99, fp);
                printf("%s", buff);
            }
            //Tidy up here
            fclose(fp);
        }
    printf("SD Read Finished...\n");
    }
    else{
        printf("No SD to read...\n\n");
    }
    return 0;
}

// this function resets the sd card
void SDcard::reset(){
    card.deinit();
}