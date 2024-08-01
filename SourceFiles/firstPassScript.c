#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/firstPassScript.h"

bool endsWithColon(char *str);

void firstPass(char *filesName) {

    char amFileName[MAX_LINE_SIZE];
    char obFileName[MAX_LINE_SIZE];
    FILE *amFile;
    FILE *obFile;

    machineCodeLine *machinecodeline[3];
    bool isSymbolDef;
    char buffer[MAX_LINE_SIZE];
    char *temp;
    int *dataNums;
    int wordNumModifier;
    int IC, DC;
    int size;
    
    IC = DC = 0;
    isSymbolDef = false;
    dataNums = (int *)malloc(sizeof(int));

    strcpy(amFileName, filesName);
    strcat(amFileName, ".am");

    strcpy(obFileName, filesName);
    strcat(obFileName, ".ob");

    amFile = fopen(amFileName, "r");
    obFile = fopen(obFileName, "w");

    if (amFile == NULL) {
        perror("Error opening file"); /*ERROR, DO SOMETHING COOL*/
        exit(1);
    }

    initializeDataArray();
    initializeInstructionsArray();
    initializeSymbolTable();

    while (fgets (buffer, sizeof(buffer), amFile) != NULL ) {

        line *currentLine = (line *)malloc(sizeof(line));
        lineToArrayOfWords(buffer, currentLine);

        if (currentLine->wordsCount != 0 && currentLine->wordsArray[0][0] != ';') {
            if (endsWithColon(currentLine->wordsArray[0])) {
                isSymbolDef = true;
                if (currentLine->wordsCount >= 1) {
                    /*ERORRERE*/
                }
                wordNumModifier = -1;
            }

            else {
                isSymbolDef = false;
                wordNumModifier = 0;
            }

            if (strcmp(currentLine->wordsArray[-wordNumModifier], ".extern") == 0) {
                if (currentLine->wordsCount + wordNumModifier > 2 ) {
                    /*ERRORINO*/
                }
                /* support multiple stuff */
                insertToSymbolTable(currentLine->wordsArray[-wordNumModifier + 1], ".external", 0);
            }

            else if (isSymbolDef) {
                if ( (!isNameLegal (currentLine->wordsArray[0]) ) || (!isMacroName (currentLine->wordsArray[0]) ) || (getSymbolValue (currentLine->wordsArray[0]) == -1) ) { /*needs to check for extern data string*/
                    /*ERRRORRR*/
                }

                if ( (strcmp(currentLine->wordsArray[-wordNumModifier], ".data") == 0) || (strcmp(currentLine->wordsArray[-wordNumModifier], ".string") == 0) ) {


                    if (strcmp(currentLine->wordsArray[-wordNumModifier], ".data") == 0) {
                        size = parseDataArray(currentLine, dataNums);
                        insertArrayToDataArray(dataNums, size);
                    }
                    else {
                        temp = removeQuotions(currentLine->wordsArray[-wordNumModifier+1]);
                        size = strlen(temp) + 1;
                        insertStringToDataArray(temp);
                        free(temp);
                    }
                    insertToSymbolTable(currentLine->wordsArray[0], ".data", DC);
                    DC += size;
                }
            
                else {
                    temp = my_strdup(currentLine->wordsArray[0]);
                    *(temp + strlen(temp) - 1) = '\0';
                    insertToSymbolTable(temp, ".code", IC+100);
                    free(temp);
                }
            }

            if (isInstruction(currentLine->wordsArray[-wordNumModifier])) {
                size = lineToMachineCode(currentLine, -wordNumModifier, machinecodeline);
                insertToInstructionsArray(machinecodeline, size + 1);
                /* for (i = 0; i < size; i++) { */
                    /* fprintf(obFile, "%d\n", decimalToOctal(machineCodeToNum(machinecodeline[i]))); */
                /* } */
                IC+=size;
            }

            else if ( (strcmp(currentLine->wordsArray[-wordNumModifier], ".data") != 0) && (strcmp(currentLine->wordsArray[-wordNumModifier], ".string") != 0) && 
            (strcmp(currentLine->wordsArray[-wordNumModifier], ".entry") != 0) && (strcmp(currentLine->wordsArray[-wordNumModifier], ".extern") != 0) ) {
                /*ERRORRERE*/
            }
            
        }
    }
    free(dataNums);
    addToAllDataInSymbolTable(IC+100);

}

bool endsWithColon(char *str) {
    return *(str + strlen(str) - 1) == ':';
}

int parseDataArray(line *currentLine, int *dataNums) {
    int i;
    int num;
    int size;
    char *str;
    char **dataString;

    dataString = NULL;
    
    size = parseCommas(currentLine, 0, &dataString);
    dataNums = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++) {
        str = *(dataString+i);
        num = stringToNum(str);
        if (num == INT_MIN) {
            /*ERRORR*/
        }

        dataNums[size] = num;
    }

    if (size == 0) {
        /* error */
    }

    return size;
}

int parseCommas(line *currentLine, int startingWord, char ***output) {
    int i;
    int size;
    char *token;
    char *str;
    char *delimiter = ",";
    char temp[MAX_LINE_SIZE];
    
    *output = (char **)malloc(currentLine->wordsCount * sizeof(char*));
    temp[0] = '\0';



    str = currentLine->wordsArray[currentLine->wordsCount-1];
    if ( *(str + strlen(str) - 1) == ',' ) {
        /*EROORRR*/
    }

    for (i = startingWord + 1; i<currentLine->wordsCount; i++) {
        str = currentLine->wordsArray[i];
        if ( (i<currentLine->wordsCount - 1) && (*(str+strlen(str)-1) != ',') && ( *(currentLine->wordsArray[i+1]) != ',' ) ) {
            /*error*/
        }
        strcat(temp, str);
    }

    size = 0;
    if (strstr(temp, ",,") != NULL) {
        /*ERROR*/
    }

    token = strtok(temp, delimiter); /*check if it changes og*/

    while (token != NULL) {
        *(*(output)+size) = my_strdup(token);
        size++;
        token = strtok(NULL, delimiter);
    }

    *output = (char **)realloc(*output, size * sizeof(char*));

    return size;
}

char* removeQuotions(char *str) {
    char *copy;
    copy  = my_strdup(str+1);
    *(copy + ( strlen(copy) - 1 ) ) = '\0';
    return copy;
}

int lineToMachineCode(line *currentLine, int startingWord, machineCodeLine *lines[3]) {
    int i;
    int size;
    char **arguments;
    bool firstIsRegister;

    arguments = NULL;

    size = parseCommas(currentLine, startingWord, &arguments);

    initializeMachineCodeLines(lines);
    insertOpcodeToMachineCodeLine(lines[0], instructionToOpcode(currentLine->wordsArray[startingWord])); /*nope*/
    setMachineCodeARE(lines[0], 2, true); /*nope*/

    if (size != getArgumentsCount(currentLine->wordsArray[startingWord]) ) {
        /* Error */
    }

    firstIsRegister = false;

    for (i = 0; i < size; i++) {
        int num;
        int addressingMethod;

        if (**(arguments+i) == '#') {
            addressingMethod = 0;
            num = stringToNum(*(arguments+i));
            if (num == INT_MIN) { /*check if too small or big somewhere*/
                /*AEROE*/
            }

            setMachineCodeARE(lines[i+1], 2, true);
            insertValuesToMachineCodeLine(lines[i+1], num);
        }

        else if ( (**(arguments+i) == '*') && (isRegister (*(arguments+i) + 1 ) ) ) {
            addressingMethod = 2;
            num = getRegisterNum(*(arguments+i) + 1);

            if (firstIsRegister)
                size--;

            if (i <= 1) 
                setMachineCodeOriginValue(lines[size - 1], num);
            else 
                setMachineCodeDestValue(lines[size - 1], num);

            setMachineCodeARE(lines[i+1], 2, true);
            
            firstIsRegister = true;
        }

        else if (isRegister (*(arguments+i) ) ) {
            addressingMethod = 3;
            num = getRegisterNum(*(arguments+i));

            if (firstIsRegister) {
                setMachineCodeOriginValue(lines[i], num);
                size--;
            }  
            else 
                setMachineCodeDestValue(lines[i+1], num);

            setMachineCodeARE(lines[i+1], 2, true);
            
            firstIsRegister = true;
        }

        else 
            addressingMethod = 1;
        
        
        setMachineCodeValues(lines[0], i*4 + addressingMethod, true);
    }



    return size;
} 



