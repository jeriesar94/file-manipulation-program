#ifndef DOUBLE_ENDED_LINKEDLIST_H_INCLUDED
#define DOUBLE_ENDED_LINKEDLIST_H_INCLUDED

/*-----Structures required to store file information-----*/
struct doubleEndedNode{
    char *data;
    unsigned char deletedStatus;
    struct doubleEndedNode *previous;
    struct doubleEndedNode *next;
};
/*------------Function Prototypes--------------*/
struct doubleEndedNode* createDoubleEndedNode(char *data, struct doubleEndedNode *previous, struct doubleEndedNode *next);
struct doubleEndedNode* addNodeToBeginning(struct doubleEndedNode *head, char *data);
struct doubleEndedNode* addNodeToBeginning(struct doubleEndedNode *head, char *data);
struct doubleEndedNode* removeFrontNode(struct doubleEndedNode *head);
struct doubleEndedNode* lazyDeleteNode(struct doubleEndedNode *node);
struct doubleEndedNode* setNodeStatus(struct doubleEndedNode *node, unsigned char status);

#endif // DOUBLE_ENDED_LINKEDLIST_H_INCLUDED
