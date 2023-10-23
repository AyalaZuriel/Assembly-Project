/**
 * lists.h:
 * ------------------------------------
 * This is the header file for the lists.c file, it contain method declaration and definition and data strucure used on lists.c
 * 
*/

#include "globalDeclarations.h"

enum OPERAND_TYPES {SOURCE_OPERAND,DEST_OPERAND} ; /*type of operand*/
enum ENCODING_TYPES {ABSOLUTE_ENCODING,EXTERNAL_ENCODING,RELOCATABLE_ENCODING}; /*type of encoding*/
enum WORDS_NUMBER {FIRST_WORD,IMMEDIATE_WORD,REGISTER_WORD}; /*the type of the word*/

        /***************************************DATA LIST***********************************/

typedef struct dataNodeStruct * data_ptr; /*data linked list pointer*/

typedef struct dataNodeStruct{
    int address; /*address of the data*/
    unsigned int value:14; /*the binary code of the ascii represent of the data*/
    data_ptr  next; /*next node on the linked list*/
}Data_node;

/**
 * a method for creating and inserting a node from given values into the list
 * @param number the number to insert
 * @param DC data counter
*/
void insertToDataList(int number, int DC);

/**
 * a method for update a node to be the head of the data list
 * @param node the node to update to be the head of list
*/
void returnDataHead(data_ptr *node); /*used only on the file fileFunctions.c on line 66*/

/**
 * A method for updating the addresses of the data entries with the final IC
 * @param IC instruction counter
*/
void UpdateDataAddress(int IC);

/**
 * a method for geting the numerical representation of the data stored in the node
 * @param nose the node of the data to get the data value from
 * @return the data value
*/
unsigned short getDataValue(data_ptr node); /*used only on file fileFunctions.c on line 70*/

/**
 * a method for returning the next node to the given node
 * @param node the given node to get its next
 * @return the next of the given node
*/
data_ptr getNextData(data_ptr node); /*used only on file fileFunctions.c on line 71*/

/**
 * a method for getting the address of the given node
 * @param node the given node to get its address
 * @return the address of the given node
*/
int getDataAddress(data_ptr node); /*used only on file fileFunctions.c on line 69*/

/**
 * a method for freeing the data list when it is not needed anymore
*/
void freeDataList();


        /*******************************ERROR LIST************************************/

/*enumerating the type of message that will be displayed to the user*/
typedef enum {ERROR,WARNING,NOTHING}errorTypes;

/*definition of a pointer to a node in the error list*/
typedef struct errorNodeStruct *error_node_ptr;

struct errorNodeStruct{ /*linked list for the error list*/
    errorTypes type; /*type of error*/
    char errorTxt[MAX_LINE_SIZE]; /*the error message to the user*/
    int lineNumber; /*line number where the error happened*/
    error_node_ptr next;/*next node*/
};
/**
 * a method for allocated memory for error node
 * @return the error node pointer
*/
error_node_ptr AllocateNewErrorNode(); 

/**
 * a method for inserting a node from given values into the error list
 * @param Msg the message that would write to the user
 * @param LineNum the line number where the error found
 * @param errorType the type of the error
 * @param DC data counter
*/
void addToErrorList(char *Msg, int LineNum, errorTypes errorTypes); 

/**
 * Printing methods the prints all the nodes in the list
 * @param fileName the name of the file
*/
void errorListPrinting(char *fileName); 

/**
 * A methods that scans the list and return the Error type found within
 * @return NOTHING if no nodes are present, ERROR otherwise (even if there are Warnings)
*/
errorTypes checkIfHasError();

/**
 * a method for freeing the error list when it is not needed anymore
*/
void freeErrorList();


        /*******************************INSTRUCTION LIST*****************************/

/*an enumeration of the addressing method for assembly words*/       
enum ADDRESSING_TYPES  {IMMEDIATE_ADDRESING,DIRECT_ADDRESING,JUMP_ADDRESING,REGISTER_ADDRESING,NIL_ADDRESING};

/*an enumeration of the addressing methods for the parameter used in jump instructions*/
enum JMP_PARAMS_ADDRESSING  {IMMEDIATE_JMP,LABEL_JMP,NIL_JMP,REGISTER_JMP};
typedef struct { /*struct for encoding the general word*/
    unsigned int ERA: 2; /*bits 0-2*/
    unsigned int DestAddressing: 2; /*bits 2-4*/
    unsigned int SourceAddressing: 2; /*bits 4-6*/
    unsigned int Opcode: 4; /*bits 6-10*/
    unsigned int Parameter2: 2; /*bits 10-12*/
    unsigned int Parameter1: 3; /*bits 12-14*/
} FirstWord;

typedef struct { /*struct for encoding the immediate word*/
    unsigned int ERA: 2; /*bits 0-2*/
    unsigned int Immidiate: 12; /*bits 2-14*/
} ImmediateWord;

typedef struct { /*struct for encoding the rgisters word*/
    unsigned int ERA: 2; /*bits 0-2*/
    unsigned int DestRegister: 6; /*bits 2-8*/
    unsigned int SourceRegister: 6; /*bits 8-14*/
} RegisterWord;

typedef union{ /*type of the actual word*/
    FirstWord FIRST; /*the instruction word*/
    ImmediateWord immediate; /*immediate type word*/
    RegisterWord reg; /*register type word*/
} word_union;

/*a definition of a pointer to a instructionsList node*/
typedef struct wnode * instructionsListPtr;

typedef struct wnode{ /*instruction linked list*/
    int type; /*type of word*/
    int address; /*address of word*/
    word_union current_word; /*which word we are*/
    instructionsListPtr next; /*next node*/
}Instruction_node;

/**
 * inserts a command and its operands into the instructions list.
 * @param command The opcode of the command to be inserted.
 * @param SourceOperand The source operand of the command.
 * @param SourceFlag The addressing mode of the source operand.
 * @param returnValueSource The value of the source operand (either register or address).
 * @param DestOperand The destination operand of the command.
 * @param DestFlag The addressing mode of the destination operand.
 * @param returnValueDest The value of the destination operand (either register or address).
 * @param LineNum The number of the line in the source file where the command appears.
 * @param IC The current instruction counter
*/
void addCmdsToWordsList(int command, char *SourceOperand, int SourceFlag, int returnValueSource, char *DestOperand, int DestFlag, int returnValueDest, int LineNum, int IC);

/**
 * Inserts a jump command instruction to the instruction list.
 * @param commandNum The opcode of the jump command.
 * @param DestOperand The destination operand of the jump command.
 * @param returnValueDest The value of the destination operand if it's a number, or 0 if it's a label.
 * @param param1 The value of the first parameter of the jump command, or 0 if there's no first parameter.
 * @param param1_flag The encoding type of the first parameter.
 * @param param1_char The string representation of the first parameter if it's a label, or NULL otherwise.
 * @param param2 The value of the second parameter of the jump command, or 0 if there's no second parameter.
 * @param param2_flag The encoding type of the second parameter.
 * @param param2_char The string representation of the second parameter if it's a label, or NULL otherwise.
 * @param LineNum The line number of the command in the source file.
 * @param IC The current instruction counter value.
*/
void addJumpCmds(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag, char *param1_char, int param2, int param2_flag, char *param2_char, int LineNum, int IC);

/**
 * A method for updating a given node to the list's head
 * @param node the node to update
*/
void returnInstructionsHeadNode(instructionsListPtr *node); 

/**
 * Sets the register word of an instruction in the given instructions list pointer with the given type of operand and current value.
 * @param current a pointer to the current instruction in the instructions list
 * @param OperandType the type of operand, either SOURCE_OPERAND or DESTINATION_OPERAND
 * @param value the current value of the operand to be set in the register word
*/
void registerWordSetting( instructionsListPtr current,int OperandType, int value);

/**
 * Sets the immediate word of an instruction in the given instructions list pointer with the given Command_Operand_Flag, Jump_Operand_Flag, Value, IC and str.
 * @param Command_Operand_Flag the flag indicating the addressing mode of the command operand, either IMMEDIATE_ADDRESING or DIRECT_ADDRESING
 * @param Jump_Operand_Flag the flag indicating the addressing mode of the jump operand, either IMMEDIATE_JMP or LABEL_JMP
 * @param Current a pointer to the current instruction in the instructions list
 * @param Value the current value of the operand to be set in the immediate word
 * @param IC the current instruction counter
 * @param str a string representation of the operand, which is only used if Jump_Operand_Flag is LABEL_JMP
*/
void wordSetting(int Command_Operand_Flag,int Jump_Operand_flag, instructionsListPtr Current, int Value, int IC, char *str);

/**
 * adds a new instruction node to the end of the linked list of instructions. If the list is empty,
 * the new node becomes the head and the tail of the list. If the list is not empty, the new node is
 * added to the end of the list and becomes the new tail.
 * @param node A pointer to the instruction node that needs to be added to the list.
*/
void insertToWordsList(instructionsListPtr node);

/**
 * allocates memory for a new instruction node, initializes the fields of the node with the specified values,
 * and returns a pointer to the new node.
 * @param address An integer value representing the address of the instruction node.
 * @return A pointer to the newly created instruction node.
*/
instructionsListPtr allocateNewInstuctionNode(int address);

/**
 * A method for returning the binary value of the word stored in the word pointer
 * @param node the eord node to get its binary value
 * @return the binary value of the word
*/
unsigned short int * getBinaryValueOfWord(instructionsListPtr node);

/**
 * Allocates memory for a new instruction node and initializes its values.
 * @param address The address value to set for the new instruction node.
 * @return A pointer to the new instruction node.
*/
instructionsListPtr getNextInstruction(instructionsListPtr node);

/**
 * a method to get an instruction address of a given node
 * @param node the node to get its address
 * @return the address of the given node
*/
int getInstructionAddress(instructionsListPtr node);

/**
 * a method for freeing the instruction list when it is not needed anymore
*/
void freeInstructionslist();


        /*******************************FIRST PASS LIST********************************/

typedef struct{ /*a node of a normal command flags and operands*/
    int commandNumber; /*command number as defined bf the commands enum*/
    char DestOperand[MAX_SYMBOL_SIZE]; /*string to hold the Destination Operand*/
    int returnValueDest; /*numerical value of the destination Operand if not a label, a register number or an immediate*/
    int DestFlag; /*flag signaling the Destinations operand's addressing method*/
    char SourceOperand[MAX_SYMBOL_SIZE];/*a string to hold the Destination Operand*/
    int SourceFlag;/*flag signaling the Source operand's addressing method*/
    int returnValueSource;/*numerical value of the source Operand if not a label, a register number or an immediate*/
    int LineNumber; /*line number in the .as file from which the command was read*/
    int IC; /*instruction counter fot the word*/
}CmdFlags;

typedef struct /*a node of a jump type commands flags and operands*/
{
    int commandNumber;/*command number as defined bf the commands enum*/
    char DestOperand[MAX_SYMBOL_SIZE];/*string to hold the Destination Operand*/
    int returnValueDest; /*flag signaling wheter the symbol is complete*/
    int param1; /*if parameter 1 is not a label this flag holds it numerical value, a register or an immediate*/
    int param1_flag; /*the jump encoding of the parameter*/
    char param1_char[MAX_SYMBOL_SIZE];/*tring to hold the label for parameter1*/
    int param2;/*if parameter 2 is not a label this flag holds it numerical value, a register or an immediate*/
    int param2_flag;/*the jump encoding of the parameter*/
    char param2_char[MAX_SYMBOL_SIZE];/*string to hold the label for parameter2*/
    int LineNumber; /*ine number in the .am file from which the command was read*/
    int IC; /*instruction counter fot the word*/
}jumpFlagsNode;

typedef union{ /* a union representation an actul command in the list*/
    jumpFlagsNode jump_command;
    CmdFlags command;
} cmdListNode;

typedef struct cmdNumStruct* firstPassListPtr;  /*a definition of a pointer to a list node*/

typedef struct cmdNumStruct{
    bool isJump; /*flag if a command is from jump type*/
    cmdListNode current;
    firstPassListPtr next;
}numberOfCmdNode;

/**
 * Inserts the given flags and strings of a scanned jump type command into the First Pass List via the
 * first_pass_node_insert method.
 * @param commandNum - The command number of the jump type command.
 * @param DestOperand - A string representing the destination operand.
 * @param returnValueDest - An integer representing the return value of the destination operand.
 * @param param1 - The value of the first parameter of the jump type command.
 * @param param1_flag - The flag of the first parameter of the jump type command.
 * @param param1_char - A string representation of the first parameter of the jump type command.
 * @param param2 - The value of the second parameter of the jump type command.
 * @param param2_flag - The flag of the second parameter of the jump type command.
 * @param param2_char - A string representation of the second parameter of the jump type command.
 * @param LineNum - The line number where the command was found.
 * @param IC - The instruction counter for the command.
 */
void  addFirstPassJump(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag,char *param1_char, int param2, int param2_flag, char *param2_char, int LineNum, int IC);

/**
 * Inserts the given flags and strings of a scanned jump type command into the First Pass List via the
 * first_pass_node_insert method.
 * @param commandNum - The command number of the jump type command.
 * @param DestOperand - A string representing the destination operand.
 * @param returnValueDest - An integer representing the return value of the destination operand.
 * @param param1 - The value of the first parameter of the jump type command.
 * @param param1_flag - The flag of the first parameter of the jump type command.
 * @param param1_char - A string representation of the first parameter of the jump type command.
 * @param param2 - The value of the second parameter of the jump type command.
 * @param param2_flag - The flag of the second parameter of the jump type command.
 * @param param2_char - A string representation of the second parameter of the jump type command.
 * @param LineNum - The line number where the command was found.
 * @param IC - The instruction counter for the command.
 */
void addCmdsFirstPass(int commandNumber, char *SourceOperand, int SourceFlag, int returnValueSource,char *DestOperand, int DestFlag, int returnValueDest, int LineNumber, int IC);

/**
 * This function converts the data stored in the first-pass list into machine code instructions 
 * and stores them in the instructions list
*/
void firstPassListMoveToWordsList();

/**
 * Inserts a node into the InstructionsList, which in turn creates the actual assembly word representations to be later
 * printed out by the second pass method.
 * @param node - The node to be inserted into the InstructionsList.
 */
void addFirstpassToInstructions(firstPassListPtr node);

/**
 * a method for freeing the instruction list when it is not needed anymore
*/
void firstPasslistFree();


        /****************************ENTRY LIST********************************/

typedef struct entry_node_struct * entry_node_ptr; /*a definition of a pointer to the entry node*/

typedef struct entry_node_struct{  /*linked list for the entry labels*/
    int address; /*address of symbol*/
    char symbol[MAX_SYMBOL_SIZE]; /*name of symbol*/
    entry_node_ptr next; /*next node*/
}Entry_node;

/**
 * Insert a new entry node with the given symbol and address into the entry list
 * @param symbol The symbol to be stored in the new entry node
 * @param address The address to be stored in the new entry node
 * @param LineNumber The line number in the source file where the entry directive was encountered
 * @return A boolean flag indicating whether the insertion was successful
*/
void addNewToEntryList(char *symbol, int address, int LineNumber);

/**
 * Return the head node of the entry list
 * @param node A pointer to the entry node that will store the head node
*/
void returnEntryHeadNode(entry_node_ptr *node);

/**
 * Update the addresses of all entry nodes in the entry list based on their corresponding symbols in the symbol table
*/
void UpdateEntryList(); 

/**
 * Get the symbol stored in the given entry node
 * @param node A pointer to the entry node
 * @return The symbol stored in the given entry node
*/
char* getSymbolEntry(entry_node_ptr node); 

/**
 * Get the address stored in the given entry node
 * @param node A pointer to the entry node
 * @return The address stored in the given entry node
*/
int getTheEntryAddress(entry_node_ptr node);

/**
 * Get the next node after the given entry node
 * @param node A pointer to the entry node
 * @return A pointer to the next node after the given entry node (can be NULL)
*/
entry_node_ptr getNextNodeEntry(entry_node_ptr node);

/**
 * Free the memory allocated for the entry list
*/
void entryListFree();


        /****************************EXTERN LIST*************************/

typedef struct ext_node_struct * extern_node_ptr; /*a definition of a pointer to the extern node*/

typedef struct ext_node_struct{ /*linked list for the extern labels*/
    int address; /*address of symbol*/
    char symbol[MAX_SYMBOL_SIZE]; /*name of symbol*/
    extern_node_ptr next; /*next node*/
}ExternNode;

/**
 * Creates a new external symbol node and inserts it into the external symbols list.
 * @param symbol A string representing the external symbol name.
 * @param addres An integer representing the address of the external symbol.
*/
void addToExternList(char* symbol, int addres);

/**
 * Returns a pointer to the head of the external symbols list.
 * @param node An extern_node_ptr pointer that will be set to the head of the external symbols list.
*/
void returnExternNodeHead(extern_node_ptr *node);

/**
 * Returns the symbol string value of the given external symbol node.
 * @param node An extern_node_ptr representing the external symbol node.
 * @return A string representing the symbol name.
*/
char* getTheExternSymbol(extern_node_ptr node); 

/**
 * Returns the address value of the given external symbol node.
 * @param node An extern_node_ptr representing the external symbol node.
 * @return An integer representing the address value.
*/
int getExternNodeAddress(extern_node_ptr node); 

/**
 * Returns a pointer to the next external symbol node in the list.
 * @param node An extern_node_ptr representing the current external symbol node.
 * @return An extern_node_ptr representing the next external symbol node, or NULL if there is no next node.
*/
extern_node_ptr getNextNodeExtern(extern_node_ptr node);

/**
 * Frees all nodes in the external symbols list.
*/
void externListFree(); 

