/**
 *  assembler.h:
 * ----------------------------------------------------
 * This is the header file for the assembler.c file.
 * It contain method declaration and definition used on assembler.c file.
 * 
*/

#ifndef LATEST_ASSEMBLER_C_H
#define LATEST_ASSEMBLER_C_H

#include "globalDeclarations.h"

/**
 * Runs the assembler program on a given file by performing two passes.
 * Calls firstPass() to construct the symbol table and count instructions and data.
 * Calls secondPass() to translate instructions and data into machine code.
 * Calls GenerateOutFiles() to generate the output files.
 * Frees memory allocated by various data structures.
 * @param file - pointer to the input file.
 * @param name_of_file - the name of the input file.
 * @return TRUE if successful, FALSE otherwise.
*/
int AssemblerActivate(FILE *file, char *name_of_file);

/**
 * @brief Performs the second pass of the assembler on the file.
 * @param IC The current value of the instruction counter.
 * @param DC The current value of the data counter.
 * @return int Returns TRUE if the second pass succeeded, otherwise FALSE.
 * This function performs the second pass of the assembler on the file. It takes in the current values of the instruction counter and data counter,
 * and calculates the size of the file. If the size of the file exceeds the maximum file size, an error message is added to the error list and the
 * function returns FALSE. Otherwise, the function converts the data from the first pass into a list of words, and then frees the memory allocated
 * for the first pass list. The function then returns TRUE.
*/
int secondPass(int IC, int DC);

#endif
