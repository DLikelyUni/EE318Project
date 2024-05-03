/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

/* Duncan Likely
 * 202129663
 * EE318 Project
 * City Planning Board Game Thingy
 */
#include <msp430.h>
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADCchecker.h"
#include "yAxisCheck.h"
#include "types.h"
//******************************************************************************
//!
//!   Empty Project that includes driverlib
//!
//******************************************************************************

#define xlength 6
#define ylength 6
#define typesLen 4

const struct typeData housing={housingType, 3, 10};   //define housing attributes as a constant

//##################################### create constants for other types

// 2D array of portNode NULL pointers
//struct portNode *ListHeads[ylength][xlength];

//int gridMapData[ylength][xlength][4];

volatile int linearMapData[xlength*ylength*typesLen]; //144 long int array storing n of each type at each gridsquare

volatile int horizontalADCval[xlength];
volatile int vertiADCval[ylength];

void programRun(void);

void checkGPIOout(void);

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    //P1DIR |= 0x01;                          // Set P1.0 to output direction


    initialiseADC();

    initialiseGPIO();
    //setUpMUXpins(4);
    while(1){
        readADCvalue();
    }


    getInitialValues(horizontalADCval, vertiADCval); // initialize adc value arrays



    //setUpMUXpins(1);

    /*ADCCTL0 |= 0x0002;
    ADCCTL0 |= 0x0003;
    ADCCTL0 &= 0xE;*/




    programRun();

    /*
     *  Functions below used for testing functionality of ADC, GPIO MUX sel pins
     */

    //checkGPIOout();
    //int testing = readADCvalue();
}

void programRun(){

        while(1){   //infinite loop while MSP430 is running
            setUpADC(0); //sets up ADC for x axis
            //CURRENTLY ON Y AXIS

            int i = 0;
            int readingErr = 0;
            while(i<6){   //loop over vertical voltage values

                setUpMUXpins(i); //sets multiplexer input/output

                int ADCasInt;
                int numberOfBlocks;

                ADCasInt = readADCvalue(); //checks the ADC value for column i

                int ADCdifference = ADCasInt - horizontalADCval[i]; //difference between stored and new ADC values

                if(ADCdifference < -5 || ADCdifference > 5){
                    numberOfBlocks = blockChange(ADCdifference); //finds number of blocks added or removed
                    }
                else{
                    i++;
                    continue; //if no change, increments loop starts over at next column
                }


                if(numberOfBlocks == 0){
                    //log error or something here
                    if(readingErr < 10){
                        readingErr++;
                    }
                    else{
                        readingErr = 0;
                        i++;
                    }
                    continue; //restarts current loop
                }
                else if(numberOfBlocks != 0){
                    horizontalADCval[i] = ADCasInt;
                    //yAxisScan(numberOfBlocks, i, vertiADCval);

                }
                else{
                    int dispVal = horizontalADCval[i];
                    //i++;
                    continue;
                }

            };
        };

};


void checkGPIOout(){
    int i;
    int dosomethingelse;
    for(i = 0; i < 6; i++){
        setUpMUXpins(i);
        dosomethingelse = i + 3;
    }
    return;
}

