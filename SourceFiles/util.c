#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/util.h"
#include "../HeaderFiles/globals.h"


void lineToArrayOfWords(char *str, line *currentLine) {
    char *strCopy;
    char *token;
    char *delimiter = " \t\n";
    int wordCount;
    int i;

    strCopy = my_strdup(str);
    wordCount = 0;

    for (i = 0; i < MAX_WORDS_IN_LINE; i++)
        currentLine->wordsArray[i] = NULL;
    currentLine->wordsCount = 0;

    token = strtok(strCopy, delimiter);

    while (token != NULL && wordCount < MAX_WORDS_IN_LINE) {
        currentLine->wordsArray[wordCount] = my_strdup(token);
        wordCount++;

        if (strcmp(token, ".string") == 0) { /* fix if after end " there are spaces or tabs */
            char *stringDelimiter = "\"";
            char ptr[MAX_LINE_SIZE] = "\"";

            if (!endsWithQuotationMark(str)) {
                /* do Error? */
                currentLine->wordsCount = ERROR;
                return;
            }
            
            token = strtok(NULL, stringDelimiter);
            strcat(ptr, token);
            strcat(ptr, "\"");
            currentLine->wordsArray[wordCount] = my_strdup(ptr);
            wordCount++;
        }

        token = strtok(NULL, delimiter);
    }

    currentLine->wordsCount = wordCount;
    free(strCopy);
}

int getOperandsCount(char *operation) {
    if ( (strcmp (operation, "mov") == 0 ) ||
    (strcmp (operation, "cmp") == 0 ) || 
    (strcmp (operation, "add") == 0 ) ||
    (strcmp (operation, "sub") == 0 ) ||
    (strcmp (operation, "lea") == 0 ) )
        return 2;

    if ( (strcmp (operation, "clr") == 0 ) ||
    (strcmp (operation, "not") == 0 ) || 
    (strcmp (operation, "inc") == 0 ) ||
    (strcmp (operation, "dec") == 0 ) || 
    (strcmp (operation, "jmp") == 0 ) ||
    (strcmp (operation, "bne") == 0 ) || 
    (strcmp (operation, "red") == 0 ) ||
    (strcmp (operation, "prn") == 0 ) || 
    (strcmp (operation, "jsr") == 0 ) )
        return 1;

    if ( (strcmp (operation, "rts") == 0 ) ||
    (strcmp (operation, "stop") == 0 ) )
        return 0;

    return -1;
    
}

bool isInstruction(char *name) {
    if ( (strcmp (name, "mov") == 0 ) ||
    (strcmp (name, "cmp") == 0 ) || 
    (strcmp (name, "add") == 0 ) ||
    (strcmp (name, "sub") == 0 ) ||
    (strcmp (name, "lea") == 0 ) ||
    (strcmp (name, "clr") == 0 ) ||
    (strcmp (name, "not") == 0 ) || 
    (strcmp (name, "inc") == 0 ) ||
    (strcmp (name, "dec") == 0 ) || 
    (strcmp (name, "jmp") == 0 ) ||
    (strcmp (name, "bne") == 0 ) || 
    (strcmp (name, "red") == 0 ) ||
    (strcmp (name, "prn") == 0 ) || 
    (strcmp (name, "jsr") == 0 ) ||
    (strcmp (name, "rts") == 0 ) ||
    (strcmp (name, "stop") == 0 ) )

    return false;
}

bool isNameLegal(char *name) {
    if ( (strcmp (name, "mov") == 0 ) ||
    (strcmp (name, "cmp") == 0 ) || 
    (strcmp (name, "add") == 0 ) ||
    (strcmp (name, "sub") == 0 ) ||
    (strcmp (name, "lea") == 0 ) ||
    (strcmp (name, "clr") == 0 ) ||
    (strcmp (name, "not") == 0 ) || 
    (strcmp (name, "inc") == 0 ) ||
    (strcmp (name, "dec") == 0 ) || 
    (strcmp (name, "jmp") == 0 ) ||
    (strcmp (name, "bne") == 0 ) || 
    (strcmp (name, "red") == 0 ) ||
    (strcmp (name, "prn") == 0 ) || 
    (strcmp (name, "jsr") == 0 ) ||
    (strcmp (name, "rts") == 0 ) ||
    (strcmp (name, "stop") == 0 ) )
        return false;

    if ( (strlen(name) == 2) && (name[0] == 'r') && (name[1] >= '0') && (name[1] <= '7') )
        return false;

    return true;
}

char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1;
    char *dst = malloc(len);
    if (dst == NULL) 
        return NULL;
    strcpy(dst, src);
    return dst;
}

bool endsWithQuotationMark (char *str) {
    int i;
    char currentChar;
    for (i = strlen(str) - 1; i >= 0 ;i--){
        currentChar = *(str + i);
        if (currentChar == '\"')
            return true;
        if ( (currentChar != ' ') && (currentChar != '\t') && (currentChar != '\n') )
            return false;
    }
    return false;
}

void initializeMachineCodeLine(machineCodeLine *word) {
    word->are[0] = false;
    word->are[1] = false;/*malloc prob*/
    word->are[2] = false;
    word->dest[0] = false;
    word->dest[1] = false;
    word->dest[2] = false;
    word->dest[3] = false;
    word->origin[0] = false;
    word->origin[1] = false;
    word->origin[2] = false;
    word->origin[3] = false;
    word->opcode[0] = false;
    word->opcode[1] = false;
    word->opcode[2] = false;
    word->opcode[3] = false;
} 

void instructionToOpcode (char *instruction, bool opcode[4]) {
    int instructionNum = ERROR;
    if (strcmp (instruction, "mov") == 0) 
        instructionNum = 0;
    else if (strcmp (instruction, "cmp") == 0) 
        instructionNum = 1;
    else if (strcmp (instruction, "add") == 0) 
        instructionNum = 2;
    else if (strcmp (instruction, "sub") == 0) 
        instructionNum = 3;
    else if (strcmp (instruction, "lea") == 0) 
        instructionNum = 4;
    else if (strcmp (instruction, "clr") == 0) 
        instructionNum = 5;
    else if (strcmp (instruction, "not") == 0) 
        instructionNum = 6;
    else if (strcmp (instruction, "inc") == 0) 
        instructionNum = 7;
    else if (strcmp (instruction, "dec") == 0) 
        instructionNum = 8;
    else if (strcmp (instruction, "jmp") == 0) 
        instructionNum = 9;
    else if (strcmp (instruction, "bne") == 0) 
        instructionNum = 10;
    else if (strcmp (instruction, "red") == 0) 
        instructionNum = 11;
    else if (strcmp (instruction, "prn") == 0) 
        instructionNum = 12;
    else if (strcmp (instruction, "jsr") == 0) 
        instructionNum = 13;
    else if (strcmp (instruction, "rts") == 0) 
        instructionNum = 14;
    else if (strcmp (instruction, "stop") == 0) 
        instructionNum = 15;
    
    binaryToBoolArray(instructionNum, opcode, 4);
}

bool unsignedNumToBoolArray(unsigned int num, bool arr[], int size) {
    int i;
    int iPow;

    if (my_pow(2,size)<num) 
        return false;
    
    for (i = 0; i<size; i++) {
        iPow = my_pow(2, i);
        arr[i] = (num % (iPow*2) >= iPow);
    }
    return true;
}

int my_pow(int a, unsigned int b) {
    int i;
    int answer;
    answer = 1;
    for (i = 0; i < b; i++)
        answer *= a;
    return answer;
}

