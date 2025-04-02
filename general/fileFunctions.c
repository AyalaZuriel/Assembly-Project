/**
 *  fileFunctions.c
 * ------------------
 * A file defining methods for file manipulation: file opening and file creation.
*/

#include "fileFunctions.h"
#include "lists.h"

#define NUM_OF_EXTENTIONS 5  /*number of extensions posible*/
#define WORD_SIZE 14 /*num of bits in a word*/

char* FILE_EXTENTIONS[NUM_OF_EXTENTIONS]={".as",".am",".ob",".ext",".ent"};  /*the file extensions type in string*/

FILE *my_open(char *name_of_file, char *mode_to_open_file, int extension_of_file)
{
    FILE *file; /*the file to open*/
    char name_extention[MAX_LINE_SIZE]; /*the name of the file with extension*/
    if (strchr(name_of_file,'.')){
        fprintf(stderr,"File %s cannot be opened - the file name must be entered without the extension \n",name_of_file);
        return NULL;
    }

    strcpy(name_extention,name_of_file);
    strcat(name_extention,FILE_EXTENTIONS[extension_of_file]);
    file = fopen(name_extention,mode_to_open_file);

    if(file == NULL){
        fprintf(stderr,"File %s cannot be opened",name_of_file);
    }

    else if (mode_to_open_file[0]=='r'){
        long length;
        fseek(file, 0, SEEK_END);
        length =  ftell(file);
        if (length == 0){
            fprintf(stderr, "The file \"%s\" is empty!\n",name_of_file);
            fclose(file);
            file = NULL;
        }
        else{
            rewind(file);
	}
    }
    return file;
}

void GenerateOutFiles(int IC, int DC, char *name)
{
    FILE * ob_file = NULL;/*The object file containing the binary code of the program*/
    FILE * ent_file = NULL;/*The entry file containing the symbols and their addresses for the entry points of the program*/
    FILE * ext_file = NULL;/*The extern file containing the symbols and their addresses for the external references used in the program*/

    unsigned short * Word_binary_Code;/* A pointer to an unsigned short integer that represents the binary code of the instruction word.*/
    instructionsListPtr Instructions_p = NULL;/* A pointer to a linked list of instructions.*/
    data_ptr Data_p = NULL;/* A pointer to a linked list of data.*/
    entry_node_ptr Entry_p = NULL;/* A pointer to a linked list of entry symbols.*/
    extern_node_ptr Extern_p=NULL;/*A pointer to a linked list of external symbols.*/
    
    ob_file = my_open(name, "w", ob_EXTENTION);
    returnInstructionsHeadNode(&Instructions_p);
    fprintf(ob_file, "\t%d\t%d\n", (IC-INITIAL_IC), (DC-INITIAL_DC));

    while (Instructions_p){

        Word_binary_Code= getBinaryValueOfWord(Instructions_p);
        fprintf(ob_file, "%04u\t", getInstructionAddress(Instructions_p));
        print_weird(ob_file, *Word_binary_Code);
        Instructions_p = getNextInstruction(Instructions_p);
    }

    returnDataHead(&Data_p);

    while (Data_p){
        fprintf(ob_file, "%04u\t", getDataAddress(Data_p));
        print_weird(ob_file, getDataValue(Data_p));
        Data_p= getNextData(Data_p);

    }

    returnEntryHeadNode(&Entry_p);

    if (Entry_p){
        ent_file = my_open(name, "w", ent_EXTENTION);

        while (Entry_p){
            fprintf(ent_file, "%s %d\n", getSymbolEntry(Entry_p), getTheEntryAddress(Entry_p));
            Entry_p = getNextNodeEntry(Entry_p);
        }
    }

    returnExternNodeHead(&Extern_p);

    if (Extern_p){
        ext_file = my_open(name, "w", ext_EXTENTION);

        while (Extern_p){
            fprintf(ext_file, "%s %d\n", getTheExternSymbol(Extern_p), getExternNodeAddress(Extern_p));
            Extern_p = getNextNodeExtern(Extern_p);
        }
    }

    if (ob_file)
        fclose(ob_file);
    if (ent_file)
        fclose(ent_file);
    if (ext_file)
        fclose(ext_file);
}

void print_weird(FILE *object, unsigned short int x){
    unsigned short int mask = (unsigned short)(1<<(WORD_SIZE-1)); /*the binary word to print in weird*/
    while (mask){
        if ((x&mask) == 0)
            fprintf(object, ".");

        else
            fprintf(object, "/");
       
        mask >>= 1;
    }
    fprintf(object, "\n");
}

