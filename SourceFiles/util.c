#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../HeaderFiles/util.h"
#include "../HeaderFiles/errors.h"

/*
 * stringToArrayOfWords
 *
 * Helper function for lineToArrayOfWords
 * This function processes a quoted string in the assembly code, extracting the content
 * and storing it as an individual word in the `line` structure.
 *
 * Parameters:
 *   currentLine - A pointer to the `line` structure where the words will be stored.
 *   wordCount   - A pointer to an integer tracking the current word count.
 *   token       - A pointer to a string containing the quoted content to process.
 *
 * Returns:
 *   true if the string was processed successfully, false otherwise.
 */
bool stringToArrayOfWords(line *currentLine, int *wordCount, char **token);

/*
 * endsWithQuotationMark
 *
 * Checks if a given string ends with a quotation mark.
 *
 * Parameters:
 *   str - A string to check.
 *
 * Returns:
 *   true if the string ends with a quotation mark, false otherwise.
 */
bool endsWithQuotationMark (char *str);

/*
 * startsWithQuotationMark
 *
 * Checks if a given string starts with a quotation mark.
 *
 * Parameters:
 *   str - A string to check.
 *
 * Returns:
 *   true if the string starts with a quotation mark, false otherwise.
 */
bool startsWithQuotationMark (char *str);

/*
 * cutUntilNonWhiteSpace
 *
 * Skips over leading whitespace in a string and returns
 * the first non-whitespace character.
 *
 * Parameters:
 *   str - The string to process.
 *
 * Returns:
 *   A pointer to the first non-whitespace character in the string.
 */
char* cutUntilNonWhiteSpace(char *str);


void lineToArrayOfWords(char *str, line *currentLine, char *filename, bool checkForErrors) {
    char *strCopy;
    char *token;
    char *delimiter = " \t\n";
    int wordCount;
    int i;

    /* Create a duplicate of the input string for tokenization */
    strCopy = my_strdup(str);
    wordCount = 0;

    /* Initialize the wordsArray to NULL */
    for (i = 0; i < MAX_WORDS_IN_LINE; i++)
        currentLine->wordsArray[i] = NULL;
    
    currentLine->wordsCount = 0;

    /* Tokenize the string based on whitespace */
    token = strtok(strCopy, delimiter);

    while (token != NULL && wordCount < MAX_WORDS_IN_LINE) {
        /* Store each word in the currentLine's wordsArray */
        currentLine->wordsArray[wordCount] = my_strdup(token);
        wordCount++;

        /* Handle the .string specially */
        if (strcmp(token, ".string") == 0) {
            token = strtok(NULL, "");
            if (stringToArrayOfWords(currentLine, &wordCount, &token) == false && checkForErrors) {
                printErrorByBuffer(ERROR_CODE_5, filename, str);
                currentLine->wordsCount = ERROR;
                free(strCopy);
                return;
            }

            token = strtok(NULL, delimiter);
            if (token != NULL && checkForErrors) {
                printErrorByBuffer(ERROR_CODE_27, filename, str);
                currentLine->wordsCount = ERROR;
                free(strCopy);
                return;
            }
            break;
        }

        /* Continue tokenizing */
        token = strtok(NULL, delimiter);
    }

    /* Update the words count in the currentLine structure */
    currentLine->wordsCount = wordCount;
    free(strCopy);
}

bool stringToArrayOfWords(line *currentLine, int *wordCount, char **token) {
    bool error;
    size_t length;
    char ptr[MAX_LINE_SIZE] = "\"";
    
    /* Initialize error flag to false */
    error = false;

    /* Remove leading white spaces */
    *token = cutUntilNonWhiteSpace(*token);

    /* Check if the token starts and ends with a quotation mark */
    if ((!endsWithQuotationMark(*token)) || (!startsWithQuotationMark(*token)))
        error = true;
    else {
        char *closingQuote;

        /* Skip the opening quotation mark */
        (*token)++;

        /* Find the closing quotation mark */
        closingQuote = strchr(*token, '"');
        length = closingQuote - *token;

        /* Copy the content between quotation marks to ptr */
        strncat(ptr, *token, length);
        strcat(ptr, "\"");

        /* Store the word in the currentLine's wordsArray */
        currentLine->wordsArray[*wordCount] = my_strdup(ptr);
        (*wordCount)++;

        /* Update the token pointer to point after the closing quote */
        *token = closingQuote + 1;
    }

    /* Return whether an error occurred */
    return !error;
}

char* cutUntilNonWhiteSpace(char *str) {
    int i;
    char currentChar;

    /* Iterate over the string to find the first non-white space character */
    for (i = 0; i < strlen(str); i++) {
        currentChar = *(str + i);

        /* Return the pointer to the first non-white space character */
        if ((currentChar != ' ') && (currentChar != '\t'))
            return str + i;
    }

    /* If the string is entirely white space, return NULL */
    return NULL;
}

void freeLine(line *lineToFree) {
    int i;
    char *buffer;

    /* Initialize buffer with the first word in the wordsArray */
    buffer = lineToFree->wordsArray[0];

    /* Free each word in the wordsArray */
    for (i = 0; buffer != NULL && i < MAX_WORDS_IN_LINE; i++) {
        free(lineToFree->wordsArray[i]);

        /* Move to the next word */
        buffer = lineToFree->wordsArray[i + 1];
    }

    /* Free the line structure itself */
    free(lineToFree);
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

int getArgumentsCount(char *operation) {
    /* Check if the operation requires two arguments */
    if ((strcmp(operation, "mov") == 0) ||
        (strcmp(operation, "cmp") == 0) || 
        (strcmp(operation, "add") == 0) ||
        (strcmp(operation, "sub") == 0) ||
        (strcmp(operation, "lea") == 0))
        return 2;

    /* Check if the operation requires one argument */
    if ((strcmp(operation, "clr") == 0) ||
        (strcmp(operation, "not") == 0) || 
        (strcmp(operation, "inc") == 0) ||
        (strcmp(operation, "dec") == 0) || 
        (strcmp(operation, "jmp") == 0) ||
        (strcmp(operation, "bne") == 0) || 
        (strcmp(operation, "red") == 0) ||
        (strcmp(operation, "prn") == 0) || 
        (strcmp(operation, "jsr") == 0))
        return 1;

    /* Check if the operation requires no arguments */
    if ((strcmp(operation, "rts") == 0) ||
        (strcmp(operation, "stop") == 0))
        return 0;

    /* Return error for unknown operation */
    return ERROR;
}

bool isNameLegal(char *name) {
    /* Check if the name is a reserved word */
    if ((strcmp(name, "mov") == 0) ||
        (strcmp(name, "cmp") == 0) || 
        (strcmp(name, "add") == 0) ||
        (strcmp(name, "sub") == 0) ||
        (strcmp(name, "lea") == 0) ||
        (strcmp(name, "clr") == 0) ||
        (strcmp(name, "not") == 0) || 
        (strcmp(name, "inc") == 0) ||
        (strcmp(name, "dec") == 0) || 
        (strcmp(name, "jmp") == 0) ||
        (strcmp(name, "bne") == 0) || 
        (strcmp(name, "red") == 0) ||
        (strcmp(name, "prn") == 0) || 
        (strcmp(name, "jsr") == 0) ||
        (strcmp(name, "rts") == 0) ||
        (strcmp(name, "stop") == 0))
        return false;

    /* Check if the name is a valid register */
    if ((strlen(name) == 2) && (name[0] == 'r') && (name[1] >= '0') && (name[1] <= '7'))
        return false;

    /* Return true if the name is legal */
    return true;
}

char* my_strdup(const char *src) {
    int len;
    char *dst;

    /* Calculate the length of the source string */
    len = strlen(src) + 1;

    /* Allocate memory for the new string */
    dst = (char *)malloc(len * sizeof(char));
    if (dst == NULL) {
        printGeneralError(ERROR_CODE_4);
        exit(EXIT_FAILURE);
    }

    /* Copy the source string to the new location */
    strcpy(dst, src);

    /* Return the pointer to the duplicated string */
    return dst;
}

bool endsWithQuotationMark(char *str) {
    int i;
    char currentChar;

    /* Loop through the string from the end to the beginning */
    for (i = strlen(str) - 1; i >= 0 ; i--) {
        currentChar = *(str + i);

        /* Check if the current character is a quotation mark */
        if (currentChar == '\"')
            return true;

        /* Check if the current character is not a whitespace or newline */
        if ((currentChar != ' ') && (currentChar != '\t') && (currentChar != '\n'))
            return false;
    }

    return false;
}

bool startsWithQuotationMark(char *str) {
    int i;
    char currentChar;

    /* Loop through the string from the beginning */
    for (i = 0; i < strlen(str); i++) {
        currentChar = *(str + i);

        /* Check if the current character is a quotation mark */
        if (currentChar == '\"')
            return true;

        /* Check if the current character is not a whitespace */
        if ((currentChar != ' ') && (currentChar != '\t'))
            return false;
    }

    return false;
}

int instructionToOpcode (char *instruction) {

    /* Check for each known instruction and return its opcode */
    if (strcmp (instruction, "mov") == 0) 
        return MOV_OPCODE;
    if (strcmp (instruction, "cmp") == 0) 
        return CMP_OPCODE;
    if (strcmp (instruction, "add") == 0) 
        return ADD_OPCODE;
    if (strcmp (instruction, "sub") == 0) 
        return SUB_OPCODE;
    if (strcmp (instruction, "lea") == 0) 
        return LEA_OPCODE;
    if (strcmp (instruction, "clr") == 0) 
        return CLR_OPCODE;
    if (strcmp (instruction, "not") == 0) 
        return NOT_OPCODE;
    if (strcmp (instruction, "inc") == 0) 
        return INC_OPCODE;
    if (strcmp (instruction, "dec") == 0) 
        return DEC_OPCODE;
    if (strcmp (instruction, "jmp") == 0) 
        return JMP_OPCODE;
    if (strcmp (instruction, "bne") == 0) 
        return BNE_OPCODE;
    if (strcmp (instruction, "red") == 0) 
        return RED_OPCODE;
    if (strcmp (instruction, "prn") == 0) 
        return PRN_OPCODE;
    if (strcmp (instruction, "jsr") == 0) 
        return JSR_OPCODE;
    if (strcmp (instruction, "rts") == 0) 
        return RTS_OPCODE;
    if (strcmp (instruction, "stop") == 0) 
        return STOP_OPCODE;
    
    /* Return ERROR if no match is found */
    return ERROR;
}

int my_pow(int a, unsigned int b) {
    int i;
    int answer;

    answer = 1;

    /* Multiply the base 'a' with itself 'b' times */
    for (i = 0; i < b; i++)
        answer *= a;

    return answer;
}

void freeStringArray(char **arr, int size) {
    int i;

    /* Free each individual string */
    for (i = 0; i < size; i++)
        free(*(arr + i));

    /* Free the array itself if it's not empty */
    if (size != 0)
        free(arr);
}

int stringToNum(char *str) {
    char *endptr;
    long num;

    /* Convert the string to a long integer */
    num = strtol(str, &endptr, INPUT_BASE);

    /* Check if the conversion failed */
    if (endptr == str || *endptr != '\0')
        return INT_MIN;

    /* Check if the number is out of the range for an int */
    if (num > INT_MAX || num < INT_MIN)
        return INT_MIN;

    return (int)num;
}

bool isRegister(char *str) {
    return (*str == 'r') && ( (*(str+1) >= '0') || (*(str+1) < '8') ) && (*(str+2) == '\0');
}

int getRegisterNum (char *str) {
    return (*(str + 1) - '0');
}

bool endsWithColon(char *str) {
    return *(str + strlen(str) - 1) == ':';
}

FILE* openFile(char *fileName, char *fileType, char* openType) {
    char fileNameCopy[MAX_LINE_SIZE];

    /* Create the full filename by appending the file type to the name */
    strcpy(fileNameCopy, fileName);
    strcat(fileNameCopy, fileType);

    /* Open the file with the specified mode */
    return fopen(fileNameCopy, openType);

}

int findRowInOriginFileByBuffer(char *filesName, char *buffer) {
    int rowNum;
    char newBuffer[MAX_LINE_SIZE];
    FILE *asFile;

    /* Open the original file */
    asFile = openFile(filesName, ".as", "r");

    if (asFile == NULL) {
        printGeneralError(ERROR_CODE_26);
        exit(EXIT_FAILURE);
    }

    rowNum = 0;

    /* Read the file line by line and compare with the buffer */
    while (fgets(newBuffer, sizeof(newBuffer), asFile) != NULL) {
        rowNum++;

        if (strcmp(newBuffer, buffer) == 0)
            return rowNum;
    }

    return ERROR;
}

int findRowInOriginFileByCurrentLine(char *filesName, line *lineToFind) {
    int rowNum;
    char buffer[MAX_LINE_SIZE];
    FILE *asFile;

    /* Open the input file for reading */
    asFile = openFile(filesName, ".as", "r");
    if (asFile == NULL) {
        printGeneralError(ERROR_CODE_26);
        exit(EXIT_FAILURE);
    }

    rowNum = 0;
    while (fgets(buffer, sizeof(buffer), asFile) != NULL) {

        /* Allocate memory for a new line structure */
        line *currentLine = (line *)malloc(sizeof(line));
        if (currentLine == NULL) {
            printGeneralError(ERROR_CODE_4);
            exit(EXIT_FAILURE);
        }

        /* Convert the current buffer line to an array of words */
        lineToArrayOfWords(buffer, currentLine, filesName, false);

        rowNum++;
        
        /* Compare the current line with the line to find */
        if (isEqualLines(lineToFind, currentLine)) {
            freeLine(currentLine);
            return rowNum;
        }
        
        /* Free the memory allocated for the current line */
        freeLine(currentLine);
    }

    /* If no matching line is found, return an error code */
    return ERROR;
}

bool isEqualLines(line *line1, line *line2) {
    int i;

    /* Compare the number of words in both lines */
    if (line1->wordsCount != line2->wordsCount)
        return false;
    
    /* Compare each word in both lines */
    for (i = 0; i < line1->wordsCount; i++) {
        if (strcmp(line1->wordsArray[i], line2->wordsArray[i]) != 0)
            return false;
    }
    
    return true;
}

bool isLetter(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    else
        return false;
}

bool isAnyRegister(char *str) {
    return isRegister(str) || ( (*str == '*') && (isRegister (str + 1 ) ) );
}


