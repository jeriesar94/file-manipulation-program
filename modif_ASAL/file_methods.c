#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/file_methods.h"
#include "includes/double_ended_linkedlist.h"


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
//Function to count words in file
int countFileWords(struct doubleEndedNode *head){
    struct doubleEndedNode *cursor = head;
    //int prev = 1;
    int i = 0;
   // char c = '\0';
    int wordsCounter = 0;
    //char character;
    //Check STRTOK for word counting functionality
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
        tokens = strtok(temp, " \n\t");
        while(tokens != NULL){
            wordsCounter++;
            tokens = strtok(NULL, " \n\t");
        }
        //printf("%s", temp);
        cursor = cursor->next;
    }
    free(temp);
    //printf("%s", temp);
//    if(cursor != NULL){
//        temp = realloc(temp, (strlen(cursor->data)+1)*sizeof(char));
//        strcpy(temp, cursor->data);
//        tokens = strtok(temp, " \n,");
//        cursor = cursor->next;
//    }
//    while(cursor != NULL){
//            while(tokens != NULL){
//                tokens = strtok(NULL, " \n,");
//                //printf("%s-", tokens);
//                //if()
//                wordsCounter++;
//            }
//            temp = realloc(temp, (strlen(cursor->data)+1)*sizeof(char));
//            strcpy(temp, cursor->data);
//            tokens = strtok(temp, " \n,");
//            cursor = cursor->next;
//    }
//    free(temp);
//    while(cursor != NULL){
//        c = cursor->data[i];
//        if((c != ' ' && c != '\n' && c != '\0') && prev == 1){
//                wordsCounter++;
//                prev = 0;
//        }
//        else if(c == '\n' || c == ' ' || c == '\t'){
//            prev = 1;
//        }
//        i++;
//        if(c == '\0'){
//            cursor = cursor->next;
//            i = 0;
//        }
//    }
    printf("\n\n-------------------------COUNT INFORMATION-------------------------\n\n");
    printf("Words count is: %d \n", wordsCounter);
    printf("\n\n----------------------END OF COUNT INFORMATION---------------------\n\n");
    return wordsCounter;
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
//Function to remove file data from memory by removing the linked list nodes one by one.
struct doubleEndedNode* removeFileFromMemory(struct doubleEndedNode *fileBuffer){
    while(fileBuffer!=NULL){
        fileBuffer = removeFrontNode(fileBuffer);
    }
    return fileBuffer;
}
