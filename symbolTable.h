/*
 * SymbolTable.h:
 * ----------------------------------------------------
 * This is the header file for the SymbolTable.h data structure.
 * This is a red black binary search tree acting as the symbols table for the file scan.
 * It contain method declaration and definition, data strucure and macros used on symbolTable.c
 * 
 */

#ifndef PROJECT_SYMBOLTABLE_H
#define PROJECT_SYMBOLTABLE_H

#include "globalDeclarations.h"

#define GrandParent(x) ((x)->parent->parent) /*geting a nodes grandparant*/
#define RightUncle(x) ((x)->parent->parent->rightSon)/*geting a nodes  right uncle- the right son of its parent*/
#define LeftUncle(x) ((x)->parent->parent->leftSon)/*geting a nodes  left uncle- the left son of its parent*/
#define isLeftSon(x) ((x)==(x)->parent->leftSon) /*checking if a current node is the left son*/
#define isRightSon(x) ((x)==(x)->parent->rightSon)/*checking if a current node is the right son*/

typedef struct node* ptrOfSymbol; /*definition of a pointer to a node in the tree*/

typedef enum {BLACK,RED}colors; /*define the node colors used in the node structure*/

struct node{/*a structure defining a node of the tree */
    char symbol[MAX_SYMBOL_SIZE]; /*The symbol*/
    int address; /*The address of the symbol*/
    int lineNumber; /*The  Line number where the symbol was first found*/
    bool isExternal; /*is the symbol external*/
    bool isOperation;/*is the symbol coupled with an operation */
    bool isComplete; /*is the Label Complete*/
    colors color; /*The nodes color*/
    ptrOfSymbol parent; /*The nodes parent*/
    ptrOfSymbol leftSon; /*The nodes left son*/
    ptrOfSymbol rightSon;/*The nodes right son*/
};


/**
 * Initialize the Tree by creating the first node- the NIL node which is also the root
 */
void initializeSymbolTable(); 

/**
 * a method for inserting a symbol and all its accompanying values into the tree 
 * @param symbol symbol to insert
 * @param address address to inset to
 * @param lineNumber the line number of the sumbol
 * @param EXT the isExternal flag
 * @param OP the isOperation flag
 * @param complete the isComplete flag
 * */
void insertToSymbolTable(char *symbol, int address, int lineNumber, bool EXT, bool OP, bool complete);

/**
 * A method for returning the given nodes addreess
 * @param symbol_node the symbol to get the address from
 * @return the address of the given symbol
*/
int symbolAddress(ptrOfSymbol symbol_node);

/**
 * Method for getting the isComplete flag of the symbol
 * @param symbol_node the symbol to get the flag from
 * @return the plag isComplete of the given symbol
*/
bool isCompleteSymbol(ptrOfSymbol symbol_node);

/**
 * Method for returning the flag signaling if the symbol is external
 * @param symbol_node the symbol to get the flag from
 * @return the plag isExternal of the given symbol
*/
bool isExternalSymbol(ptrOfSymbol symbol_node);

/**
 * Method for getting the address of the symbol
 * @param symbol_node the symbol to get his address
 * @return the address of the given symbol
*/
int symbolLineNum(ptrOfSymbol symbol_node);

/**
 * A method for searching a symbol in the SymbolTable by the name of the symbol.
 * @param symbol the name of the symbol to search
 * @return the symbol pointer in the symbol table
*/
ptrOfSymbol SearchSymbol(char* symbol); 

/**
 * A method for updating the Address of the symbols which are related to .data or .string directive.
 * The methode is given the final IC Which is added to the corespoindin Symbols adress. 
 * @param IC the instructions counter
*/
void UpdateSymbols(int IC);

/**
 * Method for changing the given sybols pointer adress to the given one
 * @param symbol the symbol to change his address
 * @param address the address to change to
*/
void SetSymbolAdress(ptrOfSymbol symbol, int address);

/* */
/**
 * Method for setting the isComplete flag of the symbol to a given value
 * @param symbol the symbol to change his isComplete flag
 * @param isComplete the flag to change to
*/
void SetSymbolComplete(ptrOfSymbol symbol, bool isComplete); 

/**
 * Method for changing the given symbols pointer external status to the given one
 * @param symbol the symbol to change his isExternal flag
 * @param isExt the flag to change to
*/
void SetSymbolExt(ptrOfSymbol symbol, bool isExt);

/**
 * Method for changing the given symbols pointer operation coupeling status to the given one
 * @param symbol the symbol to change his isOperation flag
 * @param isOP the flag to change to
*/
void SetSymbolOP(ptrOfSymbol symbol, bool isOP); 



            /***********************Internal method for maintaining the table**********************/


/**
 * A recursive method for updating the Address of the symbols which are related to .data or .string directive.
 * The methode is given the final IC Which is added to the corespoindin Symbols adress. 
 * @param IC the instructions counter
 * @param node a node to update his address
*/
void Update(ptrOfSymbol node,int IC); 

/**
 * A method for searching a symbol in the Symbol Table by the name of the symbol.
 * @param name the name of the symbol to search
 * @param symbol the node to look for in the symbol table
 * @return the symbol pointer in the symbol table
*/
ptrOfSymbol Search(ptrOfSymbol symbol, char* name);

/**
 * A method for inserting a node into the symbols table
 * @param symbol the symbol to insert
*/
void insertNode(ptrOfSymbol * symbol);

/**
 * A method for maintaining the red black tree properties after the insertion of the the node to the red black tree
 * @param symbol the symbol to insert fixup
*/
void insertFixup(ptrOfSymbol* symbol);

/**
 * A method for left rotation of a given node- switching it (denoted as x) with its left son (detonated y)
 * while maintaining the tree red black properties.
 * @param symbol the symbol to rotate
*/
void rotateLeft(ptrOfSymbol symbol);

/**
 * A method for right rotation of a given node- switching it (denoted as x) with its left son (detonated y)
 * while maintaining the tree red black properties.
 * @param symbol the symbol to rotate
*/
void rotateRight(ptrOfSymbol );

/**
 * A method for creating a tree node and setting its values to the given ones
 * @param symbol the name of the symbol
 * @param address the address of the symbol in the memory
 * @param lineNumber the line number where the symbol defined
 * @param EXT the isExternal flag
 * @param OP the isOperation flag
 * @param complete the isComplete flag
 * @return the symbol the just created
 * */
ptrOfSymbol createNode(char *symbol, int address, int lineNumber, bool EXT, bool OP, bool complete);

/**
 * An internal recursive method for returning a pointer to the first symbol that was found to be incomplete in the table, meaning if was not defined.
 * The method scans the tree until a node is found that is incomplete. The method is called upon at the end of the firstPass method
 * to return all the incomplete symbols. Thus when A symbol is found, its isComplete flag is set to True so it will not be returned again.
 * @param symbol the symbol to find
 * @return the symbol that found 
*/
ptrOfSymbol find(ptrOfSymbol symbol);

/**
 * An external method for returning a pointer to the first symbol that was found to be incomplete in the table, meaning if was not defined.
 * The method calls upon the internal method find().
 * @return the symbol that found 
*/
ptrOfSymbol find_missing(); 

/**
 * An external method for freeing the tree. The method calls upon the internal method freeTable() with the root of the 
 * tree and free the root and NIL nodes after it's compleation
*/
void freeSymbolTable();

/**
 * An internal recursive method to free all the symbol table
 * @param root the head of the symbol table
*/
void freeTable(ptrOfSymbol root);

#endif
