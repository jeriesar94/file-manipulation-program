#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "includes/double_ended_linkedlist.h"
#include "includes/file_methods.h"
#include "includes/_error.h"


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
