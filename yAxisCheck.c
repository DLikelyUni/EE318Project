/*
 * yAxisCheck.c
 *
 *  Created on: 2 Apr 2024
 *      Author: duncan
 */

#include <msp430.h>
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADCchecker.h"
#include "types.h"

int blockValues[] = {8, 16, 32, 64};

int addressOffset(int xIndex, int yIndex){

    int offset;
    offset = 0 + 4*xIndex + 24*yIndex;
    return offset;

}

int sumFunction(int *arr, int size){
    int sum = 0;
    int i;
    for(i = 0; i < size; i++){
        sum += arr[i];
    }
    return sum;
}

int signalConditioning(int diffADC){
    int remainder = diffADC%8;
    if(remainder < 3){
        return diffADC - remainder;
    }
    else if(remainder > 6){
       return diffADC + (8-remainder);
    }
    else{
        return 0;
    }

}

void editBlocks(int *blockTypesArr, int xIndex, int yIndex, int *linearMapData, int remvIndicator){
    int i;
    int offset = addressOffset(xIndex, yIndex);
    for(i = 0; i < 4; i++){  //loop through type counters for changes and saved data
        int k = 0;
        while(k < blockTypesArr[i]){ //loop through number of blocks changed for type of pos i
            if(linearMapData[offset+i] > 0){ //only edit if theres something there to edit
                switch(remvIndicator){ //increment, deincrement data based on indicator
                    case -1:
                        linearMapData[offset+i]--;
                        break;
                    case 1:
                        linearMapData[offset+i]++;
                        break;
                    default:
                        break;
                }
                k++;
            }
            else{
                k++;
            }
        }
    }
    return;
}


void checkBlockType(int *blockValues, int *numBlockTypes, int *numBlockTypesRes, int numberOfBlocks, int index,  int diffADC){

    if(diffADC == 0 && sumFunction(numBlockTypes, 4) == numberOfBlocks){
        int i;
        for(i = 0; i < 4; i++){
            numBlockTypesRes[i] = numBlockTypes[i];
        }
        return;
    }
    while(index < 4 && diffADC - blockValues[index] >= 0){
        numBlockTypes[index]++;
        checkBlockType(blockValues, numBlockTypes, numBlockTypesRes, numberOfBlocks, index, diffADC-blockValues[index]);
        numBlockTypes[index]--;
        index++;
    }
};

void yAxisScan(int numberOfBlocks, int Xindex,volatile int *vertiADCval){
    int i = 0;
    int YreadingErr = 0;
    int removedBlock = 1;
    if(numberOfBlocks < 0){ // flags as blocks being removed
        numberOfBlocks = (-1)*numberOfBlocks; //inverts number of blocks
        removedBlock = -1;
    };

    setUpADC(1);
    while((i < 6)&&(numberOfBlocks != 0)){
        setUpMUXpins(i);
        volatile int ADCvalue;
        //volatile int ADCdiff;

        ADCvalue = readADCvalue();
        volatile int ADCdiff = ADCvalue - vertiADCval[i];

        int magADCdiff = ADCdiff*removedBlock;



        if(magADCdiff < 3){
            i++;
        }
        else{
            int numBlockTypes[] = {0,0,0,0};
            int numBlockTypesRes[] = {0,0,0,0};
            int index = 0;
            checkBlockType(blockValues, numBlockTypes, numBlockTypesRes, numberOfBlocks, index, ADCdiff);



        }

    }
};

