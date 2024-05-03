/*
 * ADCchecker.h
 *
 *  Created on: 2 Apr 2024
 *      Author: duncan
 */

#ifndef ADCCHECKER_H_
#define ADCCHECKER_H_

int blockChange();

void initialiseADC();

void setUpMUXpins(int index);

void getInitialValues();

void setUpADC(int axis);

int readADCvalue();

void initialiseGPIO();


#endif /* ADCCHECKER_H_ */
