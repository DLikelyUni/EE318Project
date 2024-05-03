/*
 * yAxisCheck.h
 *
 *  Created on: 2 Apr 2024
 *      Author: duncan
 */

#ifndef YAXISCHECK_H_
#define YAXISCHECK_H_

int addressOffset(int xIndex, int yIndex);

int sumFunction(int *arr, int size);

int signalConditioning(int diffADC);

void checkBlockType(int *blockValues, int *numBlockTypes, int *numBlockTypesRes, int numberOfBlocks, int index,  int diffADC);

void yAxisScan(int numberOfBlocks, int Xindex,volatile int *vertiADCval);

void editBlocks(int *blockTypesArr, int xIndex, int yIndex, int *linearMapData, int remvIndicator);

#endif /* YAXISCHECK_H_ */
