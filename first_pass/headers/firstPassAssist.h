/**
 * firstPassAssist.h:
 * ----------------------------------------------------
 * This is the header file for the firstPassAssist.c file, it contain method declaration to be used by other
 * files.
 *
 * The firstPassAssist.c contains functions to  analyse the line after an initial case was determined
 * The cases are: The line in a Directive or the line is a Command
*/

#ifndef PROJECT_COMMANDS_H
#define PROJECT_COMMANDS_H

#include "globalDeclarations.h"

enum COMMAND_TYPE {NO_OPERANDS,ONE_OPERAND,TWO_OPERANDS} ;
enum COMMAND_ENUM { MOVE_COMMAND, CMP_COMMAND, ADD_COMMAND, SUB_COMMAND, NOT_COMMAND,CLR_COMMAND,LEA_COMMAND,INC_COMMAND,
    DEC_COMMAND,JMP_COMMAND,BNE_COMMAND,RED_COMMAND,PRN_COMMAND, JSR_COMMAND,RTS_COMMAND,STOP_COMMAND} ;
enum DIRECTIVE_ENUM {DATA_DIRECTIVE,STRING_DIRECTIVE,ENTRY_DIRECTIVE,EXTERN_DIRECTIVE};

void directiveManager(int directive, int line_counter, int *newDC, bool ifHasLabelFlag, int *newIC);

/**
 * This function is responsible for managing commands in the assembly code. 
 * @param commandNumber The number of the command to manage.
 * @param line_counter The current line number in the assembly code.
 * @param newIC A pointer to the current Instruction Counter (IC).
 */
void cmdManager(int commandNumber, int LineNumber, int *IC);

/**
 * Checks if a given string is a register name.
 * @param word A string to check if it's a register name.
 * @return An integer representing the index of the register name in the registers array, or FALSE if it's not a register name.
 */
int checkIfIsRegister(char * str);

/**
 * Checks if a given string is an immediate value and returns the value if it is.
 * @param mystr A string to check if it's an immediate value.
 * @param line_counter The line number that the string appears on (used for error reporting).
 * @return The immediate value if mystr is a valid immediate value, or MAX_NUM_IMMEDIATE + 1 if it's not.
 */
int checkIfIsImmediate(char *str, int line_counter);

/**
 * Checks if a given string contains any whitespace characters.
 *
 * @param strArr A string to check for whitespace characters.
 * @return TRUE if the string contains whitespace characters, or FALSE otherwise.
 */
bool isHasSpace(char * str);

/**
 * Counts the number of commas in a given string.
 *
 * @param strArr A string to count the commas in.
 * @return An integer representing the number of commas in the string.
 */
int commasNumber(char* str);

#endif 

