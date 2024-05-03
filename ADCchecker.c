/*
 * ADCchecker.c
 *
 *  Created on: 18 Mar 2024
 *      Author: duncan
 */
#include <msp430.h>
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int blockChange();

void initialiseADC();

void setUpMUXpins(int index);

void setUpADC(int axis);

int readADCvalue();

void initialiseGPIO();





//int errorCheck(int value, int threshold, int operator);

void setUpMUXpins(int index){
    // code for setting up MUX selector pins
    /*
     * 5.1 -> MUX pin C, 9
     * 2.5 -> MUX pin B, 10
     * 2.7 -> MUX pin A, 11
    */
    switch(index){
        case 0:
        P5OUT |= 0x02;
        P2OUT |= 0x20;
        P2OUT &= 0x7F;
        break;
    case 1:
        P5OUT |= 0x02;
        P2OUT &= 0xDF;
        P2OUT &= 0x7F;
        break;
    case 2:
        P5OUT &= 0xFD;
        P2OUT |= 0x20;
        P2OUT &= 0x7F;
        break;
    case 3:
        P5OUT &= 0xFD;
        P2OUT &= 0xDF;
        P2OUT |= 0x80;
        break;
    case 4:
        P5OUT &= 0xFD;
        P2OUT &= 0xDF;
        P2OUT &= 0x7F;
        break;
    case 5:
        P5OUT &= 0xFD;
        P2OUT |= 0x20;
        P2OUT |= 0x80;
        break;
    default: break;
    };
    return;
};

void getInitialValues(int *horizontalADCval, int *vertiADCval){
    int i;
    setUpADC(0); //read x axis values
    for(i = 0; i < 6; i++){
        setUpMUXpins(i);
        horizontalADCval[i] = readADCvalue();
    }
    setUpADC(1); //read y axis values
    for(i = 0; i < 6; i++){
        setUpMUXpins(i);
        vertiADCval[i] = readADCvalue();
    }
    return;
}


/*int errorCheck(int value, int threshold, int operator){

    switch(operator){
    case 0 :
    }
};*/

int readADCvalue(){
    //ADCCTL0 |= 0x0002; //ADCENC = 1

    volatile int ADCdelayReg[10];
    int k = 0;

    for (k = 0; k < 10; k++)
    {
        //ADCMEM0 &= 0x0000;
        ADCCTL0 |= 0x0003; //start conversion
        //ADCCTL0 &= 0xE;
        while(ADCIFG == 0x0000)
        { //wait for conversion
            continue;
        };
        ADCdelayReg[k] = ADCMEM0; //writes ADC value to register
        ADCIFG &= ~0xFF; //clear interrupt flag
    };
    int sumADCval = 0;
    int j;
    for(j=0; j < 2; j++)
    {
        sumADCval += ADCdelayReg[j]; //sum of ADC values
    };
    int averageADCval = sumADCval/2; // averages ADC results
    ADCCTL0 &= 0xFFFC; //turn off ADCENC
    return averageADCval;

}

int blockChange(int ADCdifference){

    int blockRemoved = 1;

    if(ADCdifference < 0){
        ADCdifference = (-1) *ADCdifference;
        blockRemoved = -1;
    }
    int errorCheck = ADCdifference % 8;
    int numberOfBlocks = ADCdifference / 8;
    if(errorCheck < 3){
        numberOfBlocks = numberOfBlocks*blockRemoved;
        return numberOfBlocks;
    }
    else if(errorCheck > 6){
        numberOfBlocks = (numberOfBlocks+1)*blockRemoved;
        return numberOfBlocks;
    }
    else{
        return 0;
    }

};

void setUpADC(int axis){ //sets up adc input pins dependant on axis
    ADCCTL0 &= 0xFFFD; //turn off ADCENC

    if(axis == 1){ //sets ADC input to A9/P8.1,  references AVcc, AVss  Y - axis
        ADC_configureMemory(ADC_BASE, ADC_INPUT_A9,ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS);
    }
    else{  //sets ADC input to A8/P8.0, same references   X - Axis
        ADC_configureMemory(ADC_BASE, ADC_INPUT_A8,ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS);
    };

    ADCCTL0 |= 0x0010; //turn ADC back on since mem config turns it off for some reason????
    //ADCMCTL0 |= 0x10;




};

void initialiseGPIO(){


    P5SEL0 &= 0xFD; //P5.1 as GPIO

    P2DIR |= 0xA0; //P2.5,2.7 as output

    P5DIR |= 0x02; //P5.1 as output
    setUpMUXpins(0);

    //MUX sel pins, outputs initialised at select channel 6 (index 0)

};

//ADC initialisation: configures settings, leaves ADCENC as 0

void initialiseADC(){



    SYSCFG2 |= 0x0300; //enable A9 A8 as ADC pins

    ADCCTL0 &= 0xFFFD; //turn off ADCENC

    ADC_init(ADC_BASE,
         ADC_SAMPLEHOLDSOURCE_SC, //ADCSHSx
         ADC_CLOCKSOURCE_ADCOSC, //ADCSSELx
         ADC_CLOCKDIVIDER_1);  //ADCDIVx

    ADCCTL0 |= 0x10; //turn on ADC

    ADCCTL0 |= 0x410; //Set SHT and turn adc on

    ADCCTL0 &= 0xFF7F; //sets multisample to 0

    ADCCTL2 |= 0x14; //set 10 bit res, 50ksps bitrate

    ADCCTL2 &= 0xFFF7; //unsigned format

    ADCIFG &= ~0x01; //clear interrupt flag

    ADCMCTL0 |= 0x18;

    ADCCTL1 |= 0x0210;



    //ADCCTL1 register modified


    ADCIE |= 0x3F; //enable interrupt

    setUpADC(0);
};
