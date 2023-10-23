/**
 * SymbolTable.c
 * --------------------------------
 * A file defining the SymbolTable data structure, which is a red black binary search tree.
 * 
 */

#include "symbolTable.h"
#include "lists.h"

ptrOfSymbol NIL=NULL; /*The Tree NIL pointer which replaces the NULL definition in a red black tree*/
ptrOfSymbol ROOT=NULL; /*The root of the tree*/

void initializeSymbolTable() 
{
    NIL = (ptrOfSymbol) malloc(sizeof(struct node));

    if(NIL==NULL)
    {
        fprintf(stderr,"Memory allocation failure for the symbols table");
        exit(0);
    }

    strcpy(NIL->symbol,"NIL");
    NIL->color = BLACK;
    NIL->leftSon = NIL;
    NIL->rightSon = NIL;
    ROOT=NIL;
}


void insertToSymbolTable(char *symbol, int ADDRESS, int lineNumber, bool EXT, bool OP, bool complete)
{
    ptrOfSymbol tmp; /*the node to be insert*/
    if (NIL==NULL && ROOT==NULL)
        initializeSymbolTable();

    tmp= createNode(symbol, ADDRESS, lineNumber, EXT, OP, complete);
    insertNode(&tmp);
}

ptrOfSymbol createNode(char *symbol, int ADDRESS, int lineNumber, bool EXT, bool OP, bool complete)
{
    ptrOfSymbol tmp;/*the nose to be create*/
    tmp =(ptrOfSymbol)malloc(sizeof(struct node));
    if(!tmp)
    {
        fprintf(stderr,"Error Allocatin a node in the  Symbols Table\n");
        exit(1);
    }

    strcpy(tmp->symbol,symbol);
    tmp->address=ADDRESS;
    tmp->lineNumber=lineNumber;
    tmp->isExternal=EXT;
    tmp->isOperation=OP;
    tmp->isComplete=complete;

    tmp->parent=NULL;
    tmp->leftSon=NULL;
    tmp->rightSon=NULL;
    return tmp;
}


void insertNode(ptrOfSymbol *node)
{
    ptrOfSymbol z=*(node); /*the node to insert*/
    ptrOfSymbol y=NIL; /*the last node on the tree*/
    ptrOfSymbol x=ROOT; /*temporary node to get y*/

    while (x!=NIL) {
        y=x;/*y is set to be x becouse x will become one of its children*/

        if(strcmp(z->symbol,x->symbol)>0) /*if the symbol value of z is smaller than the one in the left son - set x to be the left son*/
            x=x->leftSon;
        else /*the node is the right son if x*/
            x=x->rightSon;
    }

    z->parent=y;/*when done, x is NIL and thus y is bo be the new parent of x*/

    if(y==NIL) /*The tree is empty- set the node as the root*/
        ROOT=z;
    else if(strcmp(z->symbol,y->symbol)>0) /*if the symbol value is smaller than y, the node is the left son*/
        y->leftSon=z;
    else /*the node is the right son if y*/
        y->rightSon=z;

    /*The node is now a leaf and so its values are as follows:*/
    z->leftSon=NIL;
    z->rightSon=NIL;
    z->color=RED;
    /*Maintain the red black property of the tree via the insertFixup() method */
    insertFixup(node);
}


void insertFixup(ptrOfSymbol* node)
{
    ptrOfSymbol z=*(node); /*pointer to the newly inserted node.*/
    ptrOfSymbol y; /*pointer to the uncle of z*/
    while ((z->parent->color)==RED)
    {
        if (isLeftSon(z->parent))
        {
            y=RightUncle(z);
            if((y->color)==RED)
            {
                (z->parent->color)=BLACK;
                y->color=BLACK;
                (GrandParent(z)->color)=RED;
                z=GrandParent(z);
                *(node)=z;
            }
            else
            {
                if (isRightSon(z))
                {
                    z=z->parent;
                    *(node)=z;
                    rotateLeft(*node);
                }
                z->parent->color=BLACK;
                (GrandParent(z)->color)=RED;
                rotateRight(GrandParent(z));
            }
        }
        else if (isRightSon(z->parent))
        {
            y = LeftUncle(z);
            if ((y->color) == RED)
            {
                (z->parent->color) = BLACK;
                y->color = BLACK;
                (GrandParent(z)->color) = RED;
                z = GrandParent(z);
                *(node)=z;
            }
            else
            {
                if (isLeftSon(z))
                {
                    z=z->parent;
                    *(node)=z;
                    rotateRight(*node);
                }
                z->parent->color=BLACK;
                (GrandParent(z)->color) = RED;
                rotateLeft(z->parent->parent);
            }

        }

    }
    ROOT->color=BLACK;
}


void rotateLeft(ptrOfSymbol x)
{
    ptrOfSymbol y=x->rightSon; /*set y to be the right son of x*/
    x->rightSon=y->leftSon;/*set x to be the left son of y*/

    if (y->leftSon!=NIL) /*if the left son of y isnt NILL set it to be the parent of x*/
        y->leftSon->parent=x;

    y->parent=x->parent; /*set the parent of y to be the parent of x*/

    if(x->parent==NIL) /*if the parent of x is NIL- it was the root of the tree, thus now y is the */
        ROOT=y;
    else if (isLeftSon(x)) /*if x was the left son of its parent set y to be the left son of x's parent*/
        x->parent->leftSon=y;
    else /*x was the right son of  its parent, thus set y to be the right son of its parent*/
        x->parent->rightSon=y;
    y->leftSon=x; /*y was the left son of x so now x is the left son of y*/
    x->parent=y;/*y is now the parent of x*/
}


void rotateRight(ptrOfSymbol x)
{
    ptrOfSymbol y=x->leftSon;
    x->leftSon=y->rightSon;

    if (y->rightSon!=NIL)
        y->rightSon->parent=x;

    y->parent=x->parent;

    if(x->parent==NIL)
        ROOT=y;
    else if (isRightSon(x))
        x->parent->rightSon=y;
    else
        x->parent->leftSon=y;
    y->rightSon=x;
    x->parent=y;
}


ptrOfSymbol SearchSymbol(char* symbol)
{
    return Search(ROOT,symbol);

}


void SetSymbolAdress(ptrOfSymbol Symbol, int add)
{
   Symbol->address=add;
}


void SetSymbolComplete(ptrOfSymbol Symbol, bool isComplete)
{
    Symbol->isComplete=isComplete;
}


void SetSymbolExt(ptrOfSymbol Symbol, bool isExt)
{
    Symbol->isExternal=isExt;
}


void SetSymbolOP(ptrOfSymbol Symbol, bool isOP)
{
    Symbol->isOperation=isOP;
}


ptrOfSymbol Search(ptrOfSymbol x,char* symbol)
{
    int temp; /*the result of the strcmpr function*/
   
 while ((x!=NIL) && ((temp=strcmp(symbol,x->symbol)) != 0)) /*search the symbol in the tree */
    {
        if(temp > 0) 
            x=(x->leftSon);
        else /*temp > 0*/
            x= (x->rightSon);
    }
    if (x==NIL) /*The symbol was not found*/
        x=NULL;
    

    return x;
}


void UpdateSymbols(int IC)
{
    Update(ROOT,IC);
}


void Update(ptrOfSymbol node, int IC)
{

    if (node != NIL)
    {
        if (node->isComplete == TRUE && node->isExternal == FALSE && node->isOperation == FALSE)
        {
            node->address += IC;
        }
        Update(node->leftSon, IC);
        Update(node->rightSon, IC);
    }

}


ptrOfSymbol find_missing()
{
    return find(ROOT);
}


ptrOfSymbol find(ptrOfSymbol node)
{
    ptrOfSymbol tmp;
    if (node != NIL)
    {
        if ( node->isComplete==FALSE) {/* if the node is not complete- set its complet to TRUE for the next time the find_missing function is called*/
            node->isComplete=TRUE;
	        return node;
		}
        else if (!(tmp=find(node->leftSon)))/*run the method on the left son*/
            return find(node->rightSon); /*if not found run on the right sub tree and get the right son*/
        else /*a node was found in the left sub tree*/
            return tmp;
    }
    else
        return NULL;
}


int symbolLineNum(ptrOfSymbol label_node)
{
    return label_node->lineNumber;
}


bool isCompleteSymbol(ptrOfSymbol symbol_node)
{
    return symbol_node->isComplete;
}


int symbolAddress(ptrOfSymbol label_node)
{
    return label_node->address;
}


bool isExternalSymbol(ptrOfSymbol symbol_node)
{
    return symbol_node->isExternal;
}


void freeTable(ptrOfSymbol root)
{
    ptrOfSymbol lt, rt;/*the left and the right son of root*/
    if (root!= NIL)
    {
        lt=root->leftSon;
        rt=root->rightSon;
        free(root);
        freeTable(lt);
        freeTable(rt);

    }
}


void freeSymbolTable()
{
    freeTable(ROOT);
    free(NIL);
    ROOT=NULL;
    NIL=NULL;
}


