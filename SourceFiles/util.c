#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../HeaderFiles/util.h"




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

int getArgumentsCount(char *operation) {
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
        return true;
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

int instructionToOpcode (char *instruction) {
    if (strcmp (instruction, "mov") == 0) 
        return 0;
    else if (strcmp (instruction, "cmp") == 0) 
        return 1;
    else if (strcmp (instruction, "add") == 0) 
        return 2;
    else if (strcmp (instruction, "sub") == 0) 
        return 3;
    else if (strcmp (instruction, "lea") == 0) 
        return 4;
    else if (strcmp (instruction, "clr") == 0) 
        return 5;
    else if (strcmp (instruction, "not") == 0) 
        return 6;
    else if (strcmp (instruction, "inc") == 0) 
        return 7;
    else if (strcmp (instruction, "dec") == 0) 
        return 8;
    else if (strcmp (instruction, "jmp") == 0) 
        return 9;
    else if (strcmp (instruction, "bne") == 0) 
        return 10;
    else if (strcmp (instruction, "red") == 0) 
        return 11;
    else if (strcmp (instruction, "prn") == 0) 
        return 12;
    else if (strcmp (instruction, "jsr") == 0) 
        return 13;
    else if (strcmp (instruction, "rts") == 0) 
        return 14;
    else if (strcmp (instruction, "stop") == 0) 
        return 15;
    
    return ERROR;
}

bool unsignedNumToBoolArray(unsigned int num, bool arr[], int size) {
    int i;
    int pow;

    if (my_pow(2,size)<num) 
        return false;
    pow = 1;
    for (i = 0; i<size; i++) {
        arr[i] = (num % (pow*2) >= pow);
        pow *= 2;
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

void freeStringArray (char **arr, int size) {
    int i;
    for (i = 0; i < size; i++) 
        free (*(arr+i));
    free (arr);
}

int stringToNum(char *str) {
    int num;
    num = atoi(str);
    if (num == 0) {
        if ( (strcmp(str, "0" ) != 0) && (strcmp(str, "+0" ) != 0) && (strcmp(str, "-0" ) != 0) )
            return INT_MIN;
    }
    if (num == INT_MAX || num == INT_MIN) 
            return INT_MIN;

    return num;
}

bool isRegister(char *str) {
    return (*str == 'r') && ( (*(str+1) >= '0') || (*(str+1) < '8') ) && (*(str+2) == '\0');
}

int getRegisterNum (char *str) {
    return (*(str + 1) - '0');
}

/* int decimalToOctal */

