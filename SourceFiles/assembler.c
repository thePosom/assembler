#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../HeaderFiles/assembler.h"
#include "../HeaderFiles/preAssembler.h"
#include "../HeaderFiles/firstPassScript.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/util.h"


/**
 * The program prompts the user to enter the length and elements of an array,
 * calculates the partial sums, and prints both the original array and partial sums.
 * לשנות
 */
int main(int argc, char *argv[]) {
    int i;
    for (i=1 ; i<argc ; i++) {
        char *filesName;
        filesName = argv[i];
        expandMacros(filesName);
        firstPass(filesName);
        /*secondPass(filesName);*/ 
        
        
    }
    
    return 0;
}
