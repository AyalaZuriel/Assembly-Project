/**
 * fileFunctions.h:
 * ----------------------------------------------------
 * This is the header file for the fileFunctions.c file, it contain method declaration to be used by other
 * files.
 * The fileFunctions.c contains functions to manipulate files.
*/

#ifndef PROJECT_FUNCTIONSMAIN_H
#define PROJECT_FUNCTIONSMAIN_H
#include "globalDeclarations.h"

/**
 * This function opens a file with a given name and mode and returns a pointer to it.
 * If an extension is specified, it will be added to the filename.
 * If the file cannot be opened, an error message is printed to stderr and NULL is returned.
 * If the file is opened for reading and is empty, an error message is printed to stderr and the file is closed and NULL is returned.
 * @param name_of_file: the name of the file to open
 * @param mode_to_open_file: the mode to open the file in (e.g. "r" for read, "w" for write)
 * @param extension_of_file: an integer representing the extension of the file to be appended to the name_of_file
 * @return: a pointer to the opened file, or NULL if the file could not be opened.
*/
FILE *my_open(char *name_of_file, char *mode_to_open_file, int extension_of_file);

/**
 * generates the output files required for the assembly program in the specified format.
 * @param IC (integer): The instruction counter of the program
 * @param DC (integer): The data counter of the program
 * @param name (character array): The name of the input file (without the extension)
 */
void GenerateOutFiles(int IC, int DC, char *name);

/**
 * This function prints the binary representation of an unsigned short integer in a weird format to a specified file.
 * @param object: Pointer to a FILE object that represents the file to write to.
 * @param x: Unsigned short integer value to be printed.
*/
void print_weird(FILE *object, unsigned short int x); 

enum FILE_EXTENTIONS_ENUM {as_EXTENTION,am_EXTENTION,ob_EXTENTION,ext_EXTENTION,ent_EXTENTION}; /*enumeration for the file extension type*/

#endif 

