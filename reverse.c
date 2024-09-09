/*Reverse*/
/*Lauri Ikonen*/
/*Started 02092024*/
/*Modified 09092024*/

/*Program reads a file and add each line as a node to a linked list.
After reading, the linked list order is reversed and then printed.
Memory is freed at the end of program*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define BUFFER 6

typedef struct Node {
    char textData[BUFFER];
    //Line textLine;
    struct Node* pNext;
} Node;

typedef struct Line {
    char dataCharacter;
    struct Line* pNext;
} Line;


Node * addition(/*Line *pRoot */ char *textLine, Node *pRoot){
    Node *ptr, *pNewNode;

    //Memory allocation
    if ((pNewNode = (Node*)malloc(sizeof(Node))) == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    //Values for new node
    if (textLine == NULL){
        strcpy(textLine, "empty, lineread error");
    }
    strcpy(pNewNode->textData, textLine);
    pNewNode->pNext = NULL;
   
    //Add new node to list
    //Initialize linked list if empty
    if (pRoot == NULL){
        pRoot = pNewNode;
    }

    //Linked list existing already
    else {
        ptr = pRoot;
        while (ptr->pNext != NULL){
            ptr = ptr->pNext;
        }
        ptr->pNext = pNewNode;
    }
    return pRoot;
}

Node * reverseList(Node *pRoot){
    Node *pCurrent = pRoot, *pPrev = NULL, *pNext;
   
    //Change each node's pNext to point previous node
    while (pCurrent != NULL){
        pNext = pCurrent->pNext;
        pCurrent->pNext = pPrev;
        pPrev = pCurrent;
        pCurrent = pNext;
    }
    //return root of the list, as pCurrent is NULL then pPrev must be last valid node
    return pPrev;
};

Node * removeList(Node *pRoot){
    Node *ptr = pRoot;
    //Delete linked list node by node
    while (ptr != NULL){
        pRoot = ptr->pNext;
        free(ptr);
        ptr = pRoot;
    }
    return pRoot;
};


   
Node * readFile(Node *pRoot, char *filename){
    FILE *pFile;
    char fixedSizeLine[BUFFER];

    if ((pFile = fopen(filename, "r")) == NULL){
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }
   
    while (fgets(fixedSizeLine, BUFFER, pFile)){
        strtok(fixedSizeLine, "\n");
        //first time when adding a node, the root pointer is NULL, thus pointer need to be returned
        pRoot = addition(fixedSizeLine, pRoot);
        }

    fclose(pFile);
    return pRoot;
}

int printList(Node *pRoot){
    Node *ptr = pRoot;

    while (ptr != NULL){
        fprintf(stdout, "%s\n", ptr->textData);
        ptr = ptr->pNext;
    }
    return 0;
}

int writeList(Node *pRoot, char *filename){
    Node *ptr = pRoot;
    FILE *pFile;

    if ((pFile = fopen(filename, "w")) == NULL){
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }
   
    while (ptr != NULL){
        fprintf(pFile, "%s\n", ptr->textData);
        ptr = ptr->pNext;
    }
    fclose(pFile);
    return 0;
}



int main(int argc, char *argv[]){
    Node *pRoot = NULL;
    char input[BUFFER];
    char output[BUFFER] = "stdout";

        char *line = NULL;
    size_t n = 0;
    size_t result = getline(&line, &n, stdin);
    printf("result = %zd, n = %zu, line = \"%s\"\n", result, n, line);
    free(line);

    if (argc == 1){
        fprintf(stderr, "No input file given\n");
        exit(1);
    }
   
    if (argc == 2){
        strcpy(input, argv[1]);
    }
   
    if (argc == 3){
        strcpy(input, argv[1]);
        strcpy(output, argv[2]);
       
    }

    if (argc >= 4){
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (strcmp(output, input) == 0){
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }

    pRoot = readFile(pRoot, input);
    pRoot = reverseList(pRoot);
   
    if (argv[2] == NULL){
        printList(pRoot);
        //writeList(pRoot, output);
    }

    if (argv[2] != NULL){
        writeList(pRoot, output);
    }

    pRoot = removeList(pRoot);
    return 0;
}

/* TODO */
/*String length: You may not assume anything about how long a line should be.
Thus, you may have to read in a very long input line...*/
