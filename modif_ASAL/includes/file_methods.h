#ifndef FILE_METHODS_H_INCLUDED
#define FILE_METHODS_H_INCLUDED



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
