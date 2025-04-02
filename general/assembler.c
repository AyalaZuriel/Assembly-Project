/**
 * assembler.c
 * -------------------
 * A file containing several function for analysing a a given file.
 * The output can be a list of errors found in its assembly syntax, or the creation of output files corresponding
 * to the syntax.
 * 
*/

#include "globalDeclarations.h"
#include "firstPass.h"
#include "assembler.h"
#include "lists.h"
#include "symbolTable.h"
#include "fileFunctions.h"

int AssemblerActivate(FILE *file, char *name_of_file){
     /* flag if AssemblerActivate() function succeed, what means if the first and the second pass succeed */
    int flag_to_return=TRUE;

    /*flag to check if there are errors*/
    int flag_of_error; 

    /*Instruction counter variable for the given file*/
    int new_IC=INITIAL_IC; 

    /*Data counter variable for the given file*/
    int new_DC=INITIAL_DC; 

    firstPass(file, &new_IC, &new_DC);

    if((flag_of_error=checkIfHasError())!=NOTHING) {
        errorListPrinting(name_of_file);

        if (flag_of_error==ERROR){
            firstPasslistFree();
            freeErrorList();
            flag_to_return=FALSE;
        }
    }

    if (flag_of_error!=ERROR) {
	    if (secondPass(new_IC, new_DC) == TRUE){
            GenerateOutFiles(new_IC, new_DC, name_of_file);
        }
        else {
            errorListPrinting(name_of_file);
            freeErrorList();
            flag_to_return=FALSE;
        }
    }

    freeInstructionslist();
    freeDataList();
    freeSymbolTable();
    entryListFree();
    externListFree();

    return flag_to_return;
}



