#include <stdio.h>
#include <stdlib.h>
#include "includes/double_ended_linkedlist.h"
#include "includes/_error.h"

//Function to create a linked list double ended node
struct doubleEndedNode* createDoubleEndedNode(char *data, struct doubleEndedNode *previous, struct doubleEndedNode *next){
    struct doubleEndedNode *newNode  = malloc(sizeof(struct doubleEndedNode));
    if(newNode == NULL){
        printf("Error creating a new node. Memory is not available.");
        exit(MEMORY_UNAVILABLE);
    }
    newNode->data = data;
    if(data == NULL){
        printf("Error allocating memory for file node data.\n");
        free(data);
        exit(MEMORY_UNAVILABLE);
    }
    newNode->deletedStatus = 'N';
    newNode->previous = previous;
    newNode->next = next;
    return newNode;
}
//Function to append node to head of list
struct doubleEndedNode* addNodeToBeginning(struct doubleEndedNode *head, char *data){

    struct doubleEndedNode *newBeginnningNode = createDoubleEndedNode(data, NULL, head);

    //Make current old beginning Node point it's previous to the new beginning node.
    if(head != NULL){
        head->previous = newBeginnningNode;
    }
    //Make head point to new beginning node
    head = newBeginnningNode;

    return newBeginnningNode;

}
//Function to prepend node to end of list
struct doubleEndedNode* addNodeToEnd(struct doubleEndedNode *head, char *data){

    struct doubleEndedNode *currentNodeCursor = head;
    struct doubleEndedNode *newEndNode = NULL;
    //Create a linked list cursor
    //Loop and increment linked list cursor until we reach the last node (currentNodeCursor->next == NULL)
    //Create a new node, with its previous pointer pointing to current end node and its next pointer pointing to current end node next.
    //Let current end node next pointer point to new end node.
    while(currentNodeCursor->next != NULL){
            currentNodeCursor = currentNodeCursor->next;
    }
    newEndNode = createDoubleEndedNode(data, currentNodeCursor, currentNodeCursor->next);
    currentNodeCursor->next = newEndNode;

    return newEndNode;
}
//Set deleted status of node (Allows to give node any status)
struct doubleEndedNode* setNodeStatus(struct doubleEndedNode *node, unsigned char status){
    node->deletedStatus = status;
    return node;
}
//Set deleted status of node specifically to 'Y', which means deleted.
struct doubleEndedNode* lazyDeleteNode(struct doubleEndedNode *node){
    node->deletedStatus = 'Y';
    return node;
}
//Function to remove top node from linked list
struct doubleEndedNode* removeFrontNode(struct doubleEndedNode *head){
    if(head == NULL){
        return head;
    }
    struct doubleEndedNode *front = head;
    if(head->next == NULL){
        head = head->next;
        free(front->data);
        free(front);
        return head;
    }
    head = head->next;
    head->previous = NULL;
    front->next = NULL;
    front->previous = NULL;
    free(front->data);
    free(front);
    return head;
}
