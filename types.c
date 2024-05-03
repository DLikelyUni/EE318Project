/*
 * types.c
 *
 *  Created on: 2 Apr 2024
 *      Author: duncan
 */

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to add node to list
void insertStart(struct portNode **head, enum buildingType data){
    //allocates memory for new node at *newPortNode
    struct portNode *newPortNode = (struct portNode *) malloc (sizeof(struct portNode));

    newPortNode ->
    typeNode = data; // assigns block type
    newPortNode ->
    next = *head; // sets current head to new node's 'next' address

    //  sets head value to address of new node
    *head = newPortNode;
};


// delete node function
void deleteStart(struct portNode **head){
    struct portNode *temp = *head;

    if(*head != NULL){
        *head = (*head)->next; // changes head value to next element address
            free(temp); // delete node}
        // port is empty
    }

};
