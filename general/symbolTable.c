/**
 * SymbolTable.c
 * --------------------------------
 * A file defining the SymbolTable data structure, which is a red black binary search tree.
 * 
 */

#include "symbolTable.h"
#include "lists.h"

 /* The Tree NIL pointer which replaces the NULL definition in a red black tree */
ptrOfSymbol NIL = NULL;

/* The root of the tree */
ptrOfSymbol ROOT = NULL; 

void initializeSymbolTable() {
    NIL = (ptrOfSymbol) malloc(sizeof(struct node));

    if (NIL == NULL) {
        fprintf(stderr,"Memory allocation failure for the symbols table");
        exit(0);
    }

    strcpy(NIL->symbol, "NIL");
    NIL->color = BLACK;
    NIL->leftSon = NIL;
    NIL->rightSon = NIL;
    ROOT=NIL;
}

void insertToSymbolTable(char *symbol, int ADDRESS, int lineNumber, bool EXT, bool OP, bool complete) {
    /* The node to be insert */
    ptrOfSymbol tmp; 

    if (NIL == NULL && ROOT == NULL)
        initializeSymbolTable();

    tmp = createNode(symbol, ADDRESS, lineNumber, EXT, OP, complete);
    insertNode(&tmp);
}

ptrOfSymbol createNode(char *symbol, int ADDRESS, int lineNumber, bool EXT, bool OP, bool complete) {
    /* The node to create */
    ptrOfSymbol tmp;

    tmp = (ptrOfSymbol)malloc(sizeof(struct node));

    if(!tmp) {
        fprintf(stderr,"Error Allocatin a node in the  Symbols Table\n");

        exit(1);
    }

    strcpy(tmp->symbol,symbol);
    tmp->address = ADDRESS;
    tmp->lineNumber = lineNumber;
    tmp->isExternal = EXT;
    tmp->isOperation = OP;
    tmp->isComplete = complete;

    tmp->parent = NULL;
    tmp->leftSon = NULL;
    tmp->rightSon = NULL;

    return tmp;
}

void insertNode(ptrOfSymbol *node) {
    /* The node to insert */
    ptrOfSymbol z=*(node); 

    /* The last node on the tree */
    ptrOfSymbol y=NIL; 

    /* Temporary node to get y */
    ptrOfSymbol x=ROOT; 

    while (x! = NIL) {
        y = x;

        if(strcmp(z->symbol,x->symbol)>0)
            x = x->leftSon;
        else
            x = x->rightSon;
    }

    z->parent = y;

    if(y==NIL)
        ROOT = z;
    else if(strcmp(z->symbol,y->symbol)>0) 
        y->leftSon = z;
    else 
        y->rightSon = z;

    z->leftSon = NIL;
    z->rightSon = NIL;
    z->color = RED;

    insertFixup(node);
}

void insertFixup(ptrOfSymbol* node) {
    /* Pointer to the newly inserted node */
    ptrOfSymbol z = *(node); 

    /* Pointer to the uncle of z */
    ptrOfSymbol y; 

    while ((z->parent->color) == RED) {
        if (isLeftSon(z->parent)) {
            y = RightUncle(z);

            if((y->color) == RED) {
                (z->parent->color) = BLACK;
                y->color = BLACK;
                (GrandParent(z)->color) = RED;
                z=GrandParent(z);
                *(node) = z;
            }
            else {
                if (isRightSon(z)) {
                    z = z->parent;
                    *(node) = z;
                    rotateLeft(*node);
                }

                z->parent->color = BLACK;
                (GrandParent(z)->color) = RED;
                rotateRight(GrandParent(z));
            }
        }
        else if (isRightSon(z->parent)) {
            y = LeftUncle(z);

            if ((y->color) == RED) {
                (z->parent->color) = BLACK;
                y->color = BLACK;
                (GrandParent(z)->color) = RED;
                z = GrandParent(z);
                *(node) = z;
            }
            else {
                if (isLeftSon(z)) {
                    z = z->parent;
                    *(node) = z;
                    rotateRight(*node);
                }

                z->parent->color = BLACK;
                (GrandParent(z)->color) = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }

    ROOT->color = BLACK;
}

void rotateLeft(ptrOfSymbol x) {
    ptrOfSymbol y = x->rightSon; 
    x->rightSon = y->leftSon;

    if (y->leftSon != NIL) 
        y->leftSon->parent=x;

    y->parent = x->parent;

    if(x->parent == NIL) 
        ROOT = y;
    else if (isLeftSon(x))
        x->parent->leftSon = y;
    else 
        x->parent->rightSon = y;
    y->leftSon = x;
    x->parent = y;
}

void rotateRight(ptrOfSymbol x) {
    ptrOfSymbol y = x->leftSon;
    x->leftSon = y->rightSon;

    if (y->rightSon != NIL)
        y->rightSon->parent = x;

    y->parent = x->parent;

    if(x->parent == NIL)
        ROOT = y;
    else if (isRightSon(x))
        x->parent->rightSon = y;
    else
        x->parent->leftSon = y;
    y->rightSon = x;
    x->parent = y;
}

ptrOfSymbol SearchSymbol(char* symbol) {
    return Search(ROOT,symbol);
}

void SetSymbolAdress(ptrOfSymbol Symbol, int add) {
   Symbol->address=add;
}

void SetSymbolComplete(ptrOfSymbol Symbol, bool isComplete) {
    Symbol->isComplete=isComplete;
}

void SetSymbolExt(ptrOfSymbol Symbol, bool isExt) {
    Symbol->isExternal=isExt;
}

void SetSymbolOP(ptrOfSymbol Symbol, bool isOP) {
    Symbol->isOperation=isOP;
}

ptrOfSymbol Search(ptrOfSymbol x,char* symbol) {
    /* The result of the strcmpr function */
    int temp; 
   
    while ((x != NIL) && ((temp=strcmp(symbol,x->symbol)) != 0)) {
        if (temp > 0) 
            x = (x->leftSon);
        else 
            x = (x->rightSon);
    }

    if (x == NIL)
        x = NULL;

    return x;
}

void UpdateSymbols(int IC) {
    Update(ROOT,IC);
}

void Update(ptrOfSymbol node, int IC) {
    if (node != NIL) {
        if (node->isComplete == TRUE && node->isExternal == FALSE && node->isOperation == FALSE) {
            node->address += IC;
        }

        Update(node->leftSon, IC);
        Update(node->rightSon, IC);
    }
}

ptrOfSymbol find_missing() {
    return find(ROOT);
}

ptrOfSymbol find(ptrOfSymbol node)
{
    ptrOfSymbol tmp;
    if (node != NIL) {
        if (node->isComplete == FALSE) {
            node->isComplete = TRUE;
	        return node;
		}
        else if (!(tmp=find(node->leftSon)))
            return find(node->rightSon);
        else 
            return tmp;
    }
    else return NULL;
}

int symbolLineNum(ptrOfSymbol label_node) {
    return label_node->lineNumber;
}

bool isCompleteSymbol(ptrOfSymbol symbol_node) {
    return symbol_node->isComplete;
}

int symbolAddress(ptrOfSymbol label_node) {
    return label_node->address;
}

bool isExternalSymbol(ptrOfSymbol symbol_node) {
    return symbol_node->isExternal;
}

void freeTable(ptrOfSymbol root) {
    /* The left and the right sons of root */
    ptrOfSymbol lt, rt;
    if (root != NIL) {
        lt = root->leftSon;
        rt = root->rightSon;
        free(root);
        freeTable(lt);
        freeTable(rt);
    }
}

void freeSymbolTable() {
    freeTable(ROOT);
    free(NIL);
    ROOT=NULL;
    NIL=NULL;
}
