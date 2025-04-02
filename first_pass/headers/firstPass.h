/**
 * firatPass.h:
 * ------------------------------------
 * This is the header file for the firstPass.c file, it contain method declaration and definition used on firstPass.c
 * 
*/

#ifndef PROJECT_FIRSTPASS_H
#define PROJECT_FIRSTPASS_H

#include "globalDeclarations.h"

typedef enum {DEFINITION,USAGE}labelType;

/**
 * The firstPass function performs the first pass of the assembler, reading the input file line by line and 
 * parsing each line to identify symbols, commands, and directives. It updates the IC (Instruction Counter) and 
 * DC (Data Counter) values accordingly, as well as the symbol table, error list, and entry list.
 *
 * @param fp A pointer to the input file to be read.
 * @param IC A pointer to the Instruction Counter.
 * @param DC A pointer to the Data Counter.
 */
void firstPass(FILE *fp, int *IC, int *DC);

/**
 * The isLabel function checks if the given string is a valid label according to the specified labelType. It 
 * returns TRUE if the label is valid and FALSE if it is not. It also updates the error list if necessary.
 *
 * @param temp_string A pointer to a string containing the label to check.
 * @param line_counter An integer representing the line number of the label.
 * @param typeOfLabel An enum representing the type of label to check (definition or usage).
 * @return TRUE if the label is valid, FALSE if it is not.
 */
int isLabel(char *, int, labelType type);

/**
 * The strtok_single function is a modified version of strtok that handles a single character delimiter. It 
 * takes a pointer to the string to tokenize and a pointer to the delimiter character, and returns a pointer 
 * to the next token in the string.
 *
 * @param word A pointer to the string to tokenize.
 * @param delimiters A pointer to the delimiter character.
 * @return A pointer to the next token in the string.
 */
char * myStrtok(char *str, char const *delims);

/**
 * The isDirective function checks if the given word is a valid directive string. It returns the index of the
 * directive in the DIRECTIVES array if it exists, or FALSE if it does not.
 *
 * @param word A pointer to a string containing the word to check.
 * @return The index of the directive in the DIRECTIVES array, or FALSE if it does not exist.
 */
int isDirective(char *);

/**
 * The isCommand function checks if the given word is a valid command string. It returns the index of the
 * command in the COMMAND_STRING array if it exists, or FALSE if it does not.
 *
 * @param word A pointer to a string containing the word to check.
 * @return The index of the command in the COMMAND_STRING array, or FALSE if it does not exist.
 */
int isCommand(char *);

enum DELETE_SPACE_CASES {FIRST_DELETE,NORMAL_DELETE}; /*enumaration for the case to remove for the function deleteSpaces()*/

enum DELETE_SPACE_RESULTS {EMPTY_SPACE,EXTRA_CONTENT,REMOVED_SPACES}; /* enumaration for the result case for the function deleteSpaces()*/

/**
 * A method for removing spaces form a given string by the case it was given as described above:
 * FIRST_DELETE- removes only the spaces up to the first non space char
 * NORMAL_DELETE-removes all the spaces found in the string.
 * The method returns the a flag as follows:
 * EMPTY_SPACE- the string was found to be null or contained only whitespaces
 * EXTRA_CONTENT- a non space char was found after a string, meaning two "words" were found in the string
 * REMOVED_SPACES- the space removal was succsesful
 * 
 * @param word : a pointer to a pointer to a string, representing the string to modify.
 * @param RemoveCase : representing the case to remove spaces.
 * @return flag indicating whether extra content is found or not.
 */
int deleteSpaces(char **str, int RemoveCase);

/**
 * The function removes all leading and trailing whitespaces from the string
 * @param word: pointer to the string to be processed
 * @return: pointer to the new string, or NULL if 'word' is NULL or memory if allocation fails.
*/
char* removeBlanks(char* word);

#endif 


