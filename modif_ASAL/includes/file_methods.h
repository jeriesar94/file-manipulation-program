#ifndef FILE_METHODS_H_INCLUDED
#define FILE_METHODS_H_INCLUDED

#define MEMORY_UNAVILABLE -1
#define EMPTY_BUFFER -2
#define NO_FILE -3
#define FILE_NOT_SAVED -4


/*------------Function Prototypes--------------*/
struct doubleEndedNode* readFile(FILE* fileName, struct doubleEndedNode *head);
struct doubleEndedNode* removeFileFromMemory(struct doubleEndedNode *fileBuffer);
void printFileList(struct doubleEndedNode *head);
struct doubleEndedNode* reverseFileList(struct doubleEndedNode *head);
struct doubleEndedNode* printDuplicatesInFileList(struct doubleEndedNode *head);
struct doubleEndedNode* removeDuplicatesFromFileList(struct doubleEndedNode *head);
unsigned int countFileLines(struct doubleEndedNode *head);
unsigned int countFileChars(struct doubleEndedNode *head);
int countFileWords(struct doubleEndedNode *head);
void saveFileList(struct doubleEndedNode *head, char* path);

#endif // FILE_METHODS_H_INCLUDED
