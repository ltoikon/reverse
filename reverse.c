/*Reverse*/
/*Lauri Ikonen*/
/*Started 02092024*/
/*Modified 10092024*/

/*Program reads a file and add each line as a List to a linked list.
After reading, the linked list order is reversed and then printed.
Memory is freed at the end of program*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define BUFFER 255

typedef struct List {
    char *textData;
    struct List* pNext;
} List;

List * addition(/*Line *pRoot */ char *textLine, List *pRoot){
    List *ptr, *pNewNode;
    char *pLine = NULL;

    //Memory allocation for linked list node
    if ((pNewNode = (List*)malloc(sizeof(List))) == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    
    //Memory allocation for the line of text
    if ((pLine = (char*)malloc(strlen(textLine)+1)) == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    //Values for new List
    if (textLine == NULL){
        strcpy(textLine, "empty, lineread error");
    }
    strcpy(pLine, textLine);

    pNewNode->textData = pLine;
    pNewNode->pNext = NULL;

    //Add new list node to linked list structure
    //Initialize linked list if empty
    if (pRoot == NULL){
        pRoot = pNewNode;
    }

    //Linked list existing already
    else {
        ptr = pRoot;
        int i=0;
        while (ptr->pNext != NULL){
            ptr = ptr->pNext;
            i++;
        }
        ptr->pNext = pNewNode;
    }
    return pRoot;
}

List * reverseList(List *pRoot){
    List *pCurrent = pRoot, *pPrev = NULL, *pNext;
   
    //Change each List's pNext to point previous List
    while (pCurrent != NULL){
        pNext = pCurrent->pNext;
        pCurrent->pNext = pPrev;
        pPrev = pCurrent;
        pCurrent = pNext;
    }
    //return root of the list, as pCurrent is NULL then pPrev must be last valid List
    return pPrev;
};

List * removeList(List *pRoot){
    List *ptr = pRoot;
    //Delete linked list node by node
    while (ptr != NULL){
        pRoot = ptr->pNext;
        free(ptr->textData);
        free(ptr);
        ptr = pRoot;
    }
    return pRoot;
};


   
List * readFile(List *pRoot, char *filename){
    FILE *pFile;
    char *line = NULL;
    size_t len = 0;
    __ssize_t read;

    if ((pFile = fopen(filename, "r")) == NULL){
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }

    while ((read = getline(&line, &len, pFile)) != -1 ){
        char fixedSizeLine[len];
        strcpy(fixedSizeLine, line);

        //first time when adding a List, the root pointer is NULL, thus pointer need to be returned
        pRoot = addition(fixedSizeLine, pRoot);
        
    }
    free(line);
    fclose(pFile);
    return pRoot;
}

List * readInput(List *pRoot){
    char *line = NULL;
    size_t len = 0;
    __ssize_t read;
    //printf("Enter data (End with ctrl + D):\n");

    while ((read = getline(&line, &len, stdin)) != -1 ){
        char fixedSizeLine[len];
        strcpy(fixedSizeLine, line);
        pRoot = addition(fixedSizeLine, pRoot);
        
    }
    free(line);
    return pRoot;
}

int writeList(List *pRoot, char *filename){
    List *ptr = pRoot;

    if (strcmp("stdout", filename) != 0){
        FILE *pFile;
        if ((pFile = fopen(filename, "w")) == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", filename);
            exit(1);
        }
        while (ptr != NULL){
            fprintf(pFile, "%s", ptr->textData);
            ptr = ptr->pNext;
        }
        fclose(pFile);
    }
    else {
        //printf("Output:\n");
        while (ptr != NULL){
        fprintf(stdout, "%s", ptr->textData);
        ptr = ptr->pNext;
        }
    }
    return 0;
}



int main(int argc, char *argv[]){
    List *pRoot = NULL;
    char input[BUFFER];
    char output[BUFFER] = "stdout";


    if (argc == 1){
        pRoot = readInput(pRoot);
    }
   
   /* Parameters are saved to variables for file opening,
   problems from possible buffer overflow is avoided 
   using strncpy and adding null termination */
    if (argc == 2){
        strncpy(input, argv[1], BUFFER);
        input[BUFFER - 1] = '\0'; 

        pRoot = readFile(pRoot, input);
    }
   
    if (argc == 3){
        strncpy(input, argv[1], BUFFER);
        input[BUFFER - 1] = '\0'; 
        strncpy(output, argv[2], BUFFER);
        output[BUFFER - 1] = '\0';

        pRoot = readFile(pRoot, input);
    }

    if (argc >= 4){
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (strcmp(output, input) == 0){
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }

    pRoot = reverseList(pRoot);
    writeList(pRoot, output);
    pRoot = removeList(pRoot);
    
    return 0;
}

/*TODO
Comments and documentation.
*/
