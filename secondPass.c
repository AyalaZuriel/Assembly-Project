/**
 * secondPass.c:
 * ----------------------------
 * This file contains the secondPass() method that activated after successful first pass scan via firstPass() method.
 * 
*/

#include "globalDeclarations.h"
#include "lists.h"

#define MAX_FILE_SIZE 900

int secondPass(int IC, int DC){
    int flag=TRUE;
    if ((IC-INITIAL_IC + DC-INITIAL_DC) > MAX_FILE_SIZE){
        addToErrorList("The file size exceeded memory limits for translation", 0, ERROR);
        flag=FALSE;
    }
    else{
        firstPassListMoveToWordsList();
    }
    firstPasslistFree();
return flag;
}
