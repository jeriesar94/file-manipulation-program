#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "includes/double_ended_linkedlist.h"
#include "includes/file_methods.h"



int main(int argCount, char *arguments[]){

    char function;
    FILE *fileName;
    struct doubleEndedNode *fileBuffer = NULL;
    //Check if program is given arguments, otherwise print a text showing correct usage
    if(argCount != 2){
        printf("Correct Usage: %s filepath\n", arguments[0]);
        return 0;
    }
    fileName = fopen(arguments[1], "r");
    //Check if file exits, otherwise print an error message and exit program;
    if(fileName == NULL){
        printf("Error! File not Found, please enter the path for another file.\n");
        exit(NO_FILE);
    }
    //Check if file is empty, otherwise read file.
    printf("\nReading File...\n");
    fileBuffer = readFile(fileName, fileBuffer);
    if(fileBuffer == NULL){
        printf("\n\nEmpty File!\n\n");
        exit(EMPTY_BUFFER);
    }
    printf("\nFile is ready.\n\n");

    //Input Command Loop
    while(1){
        printf("Enter the letter corresponding to desired functionality: \n\n\tp: Dump file contents to terminal.\n\tc: Display the count of characters on terminal.\n\tw: Display the count of words on terminal.\n\tl: Display the count of lines on terminal.\n\td: Display duplicated lines on terminal.\n\tr: Remove duplicated lines from file.\n\tv: Reverse file lines.\n\tq: Save changes back to file and quit program.\n");
        printf("\n\tOption: ");
        scanf(" %c", &function);
        function = tolower(function);
        switch(function){
            case 'p':
                printFileList(fileBuffer);
                break;
            case 'c':
                countFileChars(fileBuffer);
                break;
            case 'w':
                countFileWords(fileBuffer);
                break;
            case 'l':
                countFileLines(fileBuffer);
                break;
            case 'd':
                fileBuffer = printDuplicatesInFileList(fileBuffer);
                break;
            case 'r':
                fileBuffer = removeDuplicatesFromFileList(fileBuffer);
                break;
            case 'v':
                fileBuffer = reverseFileList(fileBuffer);
                break;
            case 'q':
                saveFileList(fileBuffer, arguments[1]);
                exit(0);
            default:
                printf("\n\nInvalid input, please choose one of the listed functionalities.\n\n");
                break;
        }

    }

    //exit(0);
    return 0;
}
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

    //struct doubleEndedNode *currentBeginningNodePrevious = head->previous;
    struct doubleEndedNode *newBeginnningNode = createDoubleEndedNode(data, NULL, head);

    //Make current old beginning Node point it's previous to the new beginning node.
    if(head != NULL){
        head->previous = newBeginnningNode;
    }
    //Make head point to new beginning node
    head = newBeginnningNode;

    return newBeginnningNode;

};
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
//Function prints file content to terminal
void printFileList(struct doubleEndedNode *head){
    struct doubleEndedNode *cursor = head;
    int i = 0;
    if(cursor == NULL){
        printf("EMPTY FILE!\n");
        exit(EMPTY_BUFFER);
    }
    //Traverse to end of linked list and print lines correctly,as the linked list is FILO
    while(cursor->next != NULL){
        cursor = cursor->next;
    }
    printf("\n\n[---------------FILE DUMP---------------]\n\n");
    while(cursor != NULL){

        if(cursor->data[i] == '\0'){
            printf("\n");
            cursor = cursor->previous;
            i = 0;
        }
        else if(cursor->data[i] == '\n'){
            printf("%c", cursor->data[i]);
            cursor = cursor->previous;
            i = 0;
        }
        else{
            printf("%c", cursor->data[i]);
            i++;
        }
    }
    printf("\n\n[------------END OF FILE DUMP-----------]\n\n");
}
//Read file and load it to a linked list and store it in memory.
struct doubleEndedNode* readFile(FILE* fileName, struct doubleEndedNode *head){
    //This is not done, continue working on it tomorrow consider pointer to pointer or pointer array
    struct doubleEndedNode *temp = head;
    char c = fgetc(fileName);
    int i = 0;
    while(c != EOF){
        if(head == NULL){
            head = addNodeToBeginning(temp, malloc(sizeof(char)));
            temp = head;
        }
        else{
            head->data = realloc(head->data, (i+2)*sizeof(char));
            if(head->data == NULL){
                printf("\nERROR ALLOCATING MEMORY!\n");
                exit(MEMORY_UNAVILABLE);
            }
        }
        head->data[i] = c;
        head->data[i+1] = '\0';
        i++;
        if(c == '\n'){
            temp = head;
            head = NULL;
            i = 0;
        }
        c = fgetc(fileName);
    }

    i = 0;
    rewind(fileName);
    head = temp;
    temp = NULL;
    free(temp);
    printf("\n");
    fclose(fileName);
    return head;
}
//Function to reverse file lines, by reversing the file linked list.
struct doubleEndedNode* reverseFileList(struct doubleEndedNode *head){
    struct doubleEndedNode *tempCursor = NULL;
    if(head == NULL){
        return head;
    }
    else if(head->next == NULL){
        return head;
    }
    while(head != NULL){
        tempCursor = head;
        head = head->next;
        tempCursor->next = tempCursor->previous;
        tempCursor->previous = head;
    }
    head = tempCursor;
    printf("%s", head->data);
    printf("\n\nFile is Reversed\n\n");
    return head;
}
//Function to remove duplicates from file, works by comparing linked list nodes with each other, marking them with lazy
//delete, then hard delete the nodes by traversing through the list in another loop.
struct doubleEndedNode* removeDuplicatesFromFileList(struct doubleEndedNode *head){
    struct doubleEndedNode *nodeCursor = head;
    struct doubleEndedNode *listCursor = NULL;
    struct doubleEndedNode *temp = NULL;
    while(nodeCursor != NULL){
            listCursor = nodeCursor->next;
        while(listCursor != NULL){
            int comparisonResult = strcmp(nodeCursor->data, listCursor->data);
            if(comparisonResult == 0){
                listCursor = lazyDeleteNode(listCursor);
            }
            listCursor = listCursor->next;
        }
        nodeCursor = nodeCursor->next;
    }
    nodeCursor = head;
    while(nodeCursor->next != NULL){
        if(nodeCursor->next->deletedStatus == 'Y'){
            temp = nodeCursor->next;
            nodeCursor->next = temp->next;
            nodeCursor = head;
            temp->next = NULL;
            temp->previous = NULL;
            free(temp->data);
            free(temp);
            temp = NULL;
        }
            nodeCursor = nodeCursor->next;
    }
    nodeCursor = head;
    while(nodeCursor->next != NULL){
        nodeCursor->next->previous = nodeCursor;
        nodeCursor = nodeCursor->next;
    }
    printf("\n\nDuplicates removed.\n\n");
    return head;
}
//Function to print duplicates in file, works by comparing linked list nodes with each other, setting their status
//to duplicated, then print the nodes by traversing through the list in another loop.
struct doubleEndedNode* printDuplicatesInFileList(struct doubleEndedNode *head){
    struct doubleEndedNode *nodeCursor = head;
    struct doubleEndedNode *listCursor = NULL;
    while(nodeCursor != NULL){
            listCursor = nodeCursor->next;
        while(listCursor != NULL){
            int comparisonResult = strcmp(nodeCursor->data, listCursor->data);
            if(comparisonResult == 0){
                listCursor = setNodeStatus(listCursor, 'D');
            }
            listCursor = listCursor->next;
        }
        nodeCursor = nodeCursor->next;
    }
    printf("\n\n[-----------------DUPLICATES----------------]\n\n");
    nodeCursor = head;
    while(nodeCursor != NULL){
        if(nodeCursor->deletedStatus == 'D'){
            printf("%s", nodeCursor->data);
        }
        nodeCursor = nodeCursor->next;
    }
    printf("\n\n[-------------END OF DUPLICATES-------------]\n\n");
    return head;
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
//Function to remove file data from memory by removing the linked list nodes one by one.
struct doubleEndedNode* removeFileFromMemory(struct doubleEndedNode *fileBuffer){
    while(fileBuffer!=NULL){
        fileBuffer = removeFrontNode(fileBuffer);
    }
    return fileBuffer;
}
//Function to count number of characters with and without spaces found in file.
unsigned int countFileChars(struct doubleEndedNode *head){
    unsigned int charsWithSpace = 0;
    unsigned int charsWithoutSpace = 0;
    unsigned int i = 0;
    struct doubleEndedNode *cursor = head;
    while(cursor != NULL){
            if(cursor->data[i] != '\n' && cursor ->data[i] != '\0'){
                charsWithSpace++;
                if(cursor->data[i] != ' '){
                    charsWithoutSpace++;
                }
                i++;
            }
            else{
                cursor = cursor->next;
                i = 0;
            }
    }
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    printf("Characters without spaces:\t %d\n", charsWithoutSpace);
    printf("Characters with spaces:\t %d\n", charsWithSpace);
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    return charsWithSpace;
}
//Function to count number of lines found in file.
unsigned int countFileLines(struct doubleEndedNode *head){
    unsigned int lines = 0;
    struct doubleEndedNode *cursor = head;
    while(cursor != NULL){
        lines++;
        cursor = cursor->next;
    }
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    printf("Number of lines in file:\t %d\n", lines);
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    return lines;
}
//Function to count words in file
int countFileWords(struct doubleEndedNode *head){
    struct doubleEndedNode *cursor = head;
    int i = 0;
    int wordsCounter = 0;
    char *temp = malloc(sizeof(char));
    char *tokens;
    if(temp == NULL){
        printf("Not Enough Memory!\n");
        exit(MEMORY_UNAVILABLE);
    }
    while(cursor != NULL){
        i = strlen(cursor->data);
        temp = realloc(temp, (i+1)*sizeof(char));
        if(temp == NULL){
            printf("Not Enough Memory!\n");
            exit(MEMORY_UNAVILABLE);
        }
        strcpy(temp, cursor->data);
        tokens = strtok(temp, " \r\n\t");
        while(tokens != NULL){
            wordsCounter++;
            tokens = strtok(NULL, " \r\n\t");
        }
        cursor = cursor->next;
    }
    free(temp);
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    printf("Words count is: %d \n", wordsCounter);
    printf("\n\n----------------------END OF COUNT INFORMATION---------------------\n\n");
    return wordsCounter;
}
//Function to save changes to file and exit the program
void saveFileList(struct doubleEndedNode *head, char* path){
    FILE *fileName;
    struct doubleEndedNode *cursor = head;
    fileName = fopen(path, "w");
    if(fileName == NULL){
        printf("There was an error saving the file, file not saved...\n");
        head = removeFileFromMemory(head);
        exit(FILE_NOT_SAVED);
    }
    if(head == NULL){
        printf("File is not saved, because file buffer is empty...\n");
        head = removeFileFromMemory(head);
        exit(FILE_NOT_SAVED);
    }

    while(cursor->next != NULL){
        cursor = cursor->next;
    }
    while(cursor != NULL){
        fprintf(fileName, "%s", cursor->data);
        cursor = cursor->previous;
    }
    fclose(fileName);
    head = removeFileFromMemory(head);
    printf("\n File Changes Saved!\n");
}

