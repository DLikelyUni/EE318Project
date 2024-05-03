/*
 * types.h
 *
 *  Created on: 2 Apr 2024
 *      Author: duncan
 */

#ifndef TYPES_H_
#define TYPES_H_

enum buildingType{housingType, powerPlantType, greenSpaceType, transitType};

struct typeData{  //initialises structure for types + their attributes
    enum buildingType type;
    unsigned voltageDiff;
    int pollutionFactor;
    int powerUsage;

//##################################### add more factors to building types
};

//linked list struct for port blocks
//stores type
//stores next address
struct portNode{
    enum buildingType typeNode;
    struct portNode *next;
};

void insertStart(struct portNode **head, enum buildingType data);

void deleteStart(struct portNode **head);

#endif /* TYPES_H_ */
