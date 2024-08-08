#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/secondPassScript.h"
#include "../HeaderFiles/firstPassScript.h"
#include "../HeaderFiles/preAssembler.h"
#include "../HeaderFiles/assembler.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/errors.h"
#include "../HeaderFiles/util.h"


int main(int argc, char *argv[]) {
    int i;

    if (argc == 1) {
        printGeneralError(ERROR_CODE_1);
        exit(EXIT_FAILURE);
    }

    for (i=1 ; i<argc ; i++) {
        char *filesName;
        filesName = argv[i];
        expandMacros(filesName);
        if (firstPass(filesName))
            secondPass(filesName);
        else {
            freeInstructionsArray();
            freeMacroNamesTable();
            freeSymbolTable();
            freeDataArray();
        }
    }
    
    return 0;
}
