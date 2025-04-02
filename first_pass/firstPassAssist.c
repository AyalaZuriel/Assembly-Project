/**
 * firstPassAssist.c
 * ------------------
 * A file containing several function for the handler functions that analysed a given line to determine if it maches any
 * of the criteria for.
*/

#include "firstPassAssist.h"
#include "globalDeclarations.h"
#include "firstPass.h"
#include "lists.h"
#include "symbolTable.h"

/* Max possible integer on 12 bits */
#define MAX_NUM_IMMEDIATE (4096) 

/* Min possible integer on 12 bits */
#define MIN_NUM_IMMEDIATE (-4096) 

/* Number of registers */
#define REGISTERS_NUM 8 

/* Base of a regular number */
#define BASE_NUM 10 

static char* REGISTERS_ARR[REGISTERS_NUM] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/**
 * This function handles the different directives: ".data", ".string", ".entry" and ".extern".
 * @param directive The integer value that represents the current directive.
 * @param line_counter The current line number in the source file.
 * @param newDC A pointer to the current value of the data counter.
 * @param ifHasLabelFlag A boolean value that indicates whether the line has a label or not.
 * @param newIC A pointer to the current value of the instruction counter.
 */
void directiveManager(int directive, int line_counter, int *newDC, bool ifHasLabelFlag, int *newIC) {
    char *currentChar; /*a character pointer to a string that holds the current line being processed.*/
    bool flag=TRUE; /*a boolean value used as a flag to indicate if a condition is met.*/
    int flagRemove; /*an integer variable used to indicate the result of the `deleteSpaces` function. */

    if (directive == DATA_DIRECTIVE){
        int numOfCommas; /* an integer value that represents the number of commas in a `.data` directive.*/
        int num_of_data_words=0; /*an integer value that represents the number of data words in a `.data` directive.*/
        int tmp_num; /*an integer value that holds the value of the current numeric token being processed.*/

        if (ifHasLabelFlag==FALSE){
            addToErrorList("Data directive without a label", line_counter, WARNING);
        }

        currentChar=myStrtok(NULL,"\n");
        if (deleteSpaces(&currentChar, FIRST_DELETE)==EMPTY_SPACE){
            addToErrorList("directive with no input", line_counter, ERROR);
        }
        else {
            numOfCommas = commasNumber(currentChar);
            currentChar = myStrtok(currentChar,",");

            if ((flagRemove=deleteSpaces(&currentChar, NORMAL_DELETE)) != REMOVED_SPACES){
                if (flagRemove==EMPTY_SPACE) 
                    addToErrorList("Comma detected before first number in .data directive", line_counter, ERROR);
                else 
                    addToErrorList("No comma after number in .data directive", line_counter, ERROR);
            }
            else {
                while (flag != FALSE && currentChar) {
                    tmp_num = checkIfIsImmediate(currentChar, line_counter);
                    if (tmp_num >= MAX_NUM_IMMEDIATE) {
                        flag = FALSE;
                        addToErrorList("Not a valid number detected at .data directive", line_counter, ERROR);
                    }
                    else {
                        if (currentChar[0] == '#') {
                            addToErrorList("numbers of data directive are written without the # sign", line_counter, ERROR);
                            flag = FALSE;
                        }
                        else {
                            insertToDataList(tmp_num, (*newDC)++);
                            num_of_data_words++;
                            currentChar = myStrtok(NULL, ",");

                            if (currentChar && (flagRemove=deleteSpaces(&currentChar, NORMAL_DELETE)) != REMOVED_SPACES){
                                flag = FALSE;

                                if (flagRemove == EMPTY_SPACE) 
                                    addToErrorList("Double comma (',') detected in .data directive", line_counter, ERROR);
                                else
                                    addToErrorList("No comma after number in .data directive", line_counter, ERROR);
                            }
                        }
                    }
                }
            }   
                
            if (flag == TRUE && numOfCommas >= num_of_data_words)
                addToErrorList("Comma detected after the last data number", line_counter, ERROR);
        }
    }

    else if (directive == STRING_DIRECTIVE){
        if (ifHasLabelFlag==FALSE)
            addToErrorList("String directive without a label", line_counter, WARNING);
        
        currentChar=myStrtok(NULL,"\n");

        if (deleteSpaces(&currentChar, FIRST_DELETE) !=REMOVED_SPACES)
            addToErrorList(".string operand with no given string", line_counter, ERROR);
        else {
            int i=0;
            
            if (currentChar[i]!='\"') {
                addToErrorList("A string in the string directive needs to begin with double quoets", line_counter, ERROR);
            }
            else {
                i++;
                
                while (currentChar[i]!='\0' && currentChar[i]!='\"') {
                    insertToDataList(currentChar[i],(*newDC)++);
                    i++;
                }

                if (currentChar[i]!='\"') 
                    addToErrorList("A string in the string directive needs to end with double quoets", line_counter, ERROR);
                else {
                    i++;
                    
                    while(currentChar[i]!='\0' && flag==TRUE) {
                        if(isspace(currentChar[i]))
                            i++;
                        else {
                            addToErrorList("Extrenaous content after .string directive", line_counter, ERROR);
                            flag=FALSE;
                        }
                    }

                    if (flag==TRUE)
                        insertToDataList('\0',(*newDC)++);
                }
            }
        }
    }
    
    else if (directive == ENTRY_DIRECTIVE) {
        ptrOfSymbol tmpLabel; /*temporary label pointer*/

        currentChar = myStrtok(NULL,"\n");

        if (ifHasLabelFlag==TRUE)
            addToErrorList("Label definition before  .enty directive is meaningless", line_counter, WARNING);

        if ((flagRemove= deleteSpaces(&currentChar, NORMAL_DELETE)) != REMOVED_SPACES) {
            if (flagRemove == EXTRA_CONTENT)
                addToErrorList("Extrenaous content after  label", line_counter, ERROR);
            else
                addToErrorList("No Label detected for .entry directive", line_counter, ERROR);
        }
        else {
            tmpLabel = SearchSymbol(currentChar);

            if (!tmpLabel)
                insertToSymbolTable(currentChar, *newIC, line_counter, FALSE, FALSE, FALSE);
            
            addNewToEntryList(currentChar, *newIC, line_counter);
        }
    }

    else if (directive == EXTERN_DIRECTIVE) {
        ptrOfSymbol tmpLabel; /*temporary label pointer*/

        currentChar = myStrtok(NULL,"\n");

        if (ifHasLabelFlag == TRUE)
            addToErrorList("Label definition before of .extern directive is meaningless", line_counter, WARNING);

        if ((flagRemove= deleteSpaces(&currentChar, NORMAL_DELETE)) != REMOVED_SPACES) {
            if (flagRemove == EXTRA_CONTENT) 
                addToErrorList("Extrenaous content after label in extetn directive", line_counter, ERROR);
            else
                addToErrorList("No Label detected for .extern directive", line_counter, ERROR);
        }
        else {
            if ((isLabel(currentChar, line_counter, USAGE) == TRUE)) {
                tmpLabel = SearchSymbol(currentChar);
                if (!tmpLabel) {
                    insertToSymbolTable(currentChar, 0, line_counter, TRUE, FALSE, TRUE);
                }
                else {
                    if (isExternalSymbol(tmpLabel) ==TRUE)
                        addToErrorList("Extern directive is already defined!",line_counter,ERROR);
                    else {
                        SetSymbolAdress(tmpLabel,0);
                        SetSymbolExt(tmpLabel, TRUE);
                        SetSymbolComplete(tmpLabel, TRUE);
                    }
                }
            }
        }
    }
}

void cmdManager(int commandNumber, int line_counter, int *newIC) {
    char *operandSrc = NULL; /*representing the source operand*/
    char *operandDest = NULL;/*representing the destination operand*/
    char *prm1chr = NULL, *prm2chr = NULL; /*representing the first and the second parameter*/
    int returnSrcValue = FALSE, returnDestValue = FALSE; /*representing the value of the source and the destination operands for return*/
    int ICfirst = *newIC; /*representing the value of the first instruction counter value*/
    int flagRemove; /*representing whether spaces exists in the line*/
    ptrOfSymbol symbol;/*representing a symbol in the program*/
    int currentFlag = TRUE, srcFlag = NIL_ADDRESING, DestFlag = NIL_ADDRESING, bothFlags = TWO_OPERANDS; /*addresing method reset*/
    char *tempString; /*the string to be read and tokenized*/
    int prm1 = FALSE, prm2 = FALSE, prm1flag = FALSE, prm2flag = FALSE; /*representing the first and the second paramters and their flags*/

    if (commandNumber == JMP_COMMAND || commandNumber == BNE_COMMAND || commandNumber == JSR_COMMAND){
        operandDest = myStrtok(NULL,"(");

        if (!(tempString = myStrtok(NULL,","))){
             if ((flagRemove = deleteSpaces(&operandDest, NORMAL_DELETE)) != REMOVED_SPACES){
                currentFlag = FALSE;

                if (flagRemove == EXTRA_CONTENT)
                    addToErrorList("Invalid label for command of type 'jump'", line_counter, ERROR);
                else
                    addToErrorList("No label was detected for the .extern directive", line_counter, ERROR);
            }
        }

        else{
            if ((flagRemove= deleteSpaces(&operandDest, FIRST_DELETE)) != REMOVED_SPACES){
                currentFlag = FALSE;

                if (flagRemove == EXTRA_CONTENT)
                    addToErrorList("Invalid label for command of type 'jump'", line_counter, ERROR);
                else
                    addToErrorList("No label was detected for the .extern directive", line_counter, ERROR);
            }

            if ( isHasSpace(operandDest)==TRUE){
                currentFlag = FALSE;
                if(tempString)
                    addToErrorList("A space was detected in the Label of the 'jump' command type", line_counter, ERROR);
                else
                    addToErrorList("Extrenaous content after the 'jump' command type", line_counter, ERROR);
            }
        }

        if (currentFlag==TRUE){
            if (isLabel(operandDest, line_counter, USAGE) != FALSE){
                (*newIC) += 2;
                symbol = SearchSymbol(operandDest);
                if (!symbol){
                    insertToSymbolTable(operandDest, *newIC, line_counter, FALSE, FALSE, FALSE);
                    returnDestValue = FALSE;
                }
                else{
                    returnDestValue = isCompleteSymbol(symbol);
                }

                if (tempString){
                    if (isHasSpace(tempString)==TRUE){
                        addToErrorList("Space in parameter 1 section of the 'Jump' command type", line_counter, ERROR);
                    }
                    else{
                        (*newIC) += 2;
                        if ((prm1 = checkIfIsImmediate(tempString, line_counter)) < MAX_NUM_IMMEDIATE){
                            if (tempString[0]!='#'){
                                addToErrorList("An immediate number must be written with a # sign", line_counter, ERROR);
                            }
                            else{
                                prm1flag = IMMEDIATE_JMP;
                            }
                        }
                        else if ((prm1 = checkIfIsRegister(tempString)) != FALSE){
                            prm1flag = REGISTER_JMP;
                        }
                        else if ((prm1 = isLabel(tempString, line_counter, USAGE)) != FALSE){
                            prm1flag = LABEL_JMP;
                            prm1chr = tempString;
                            symbol = SearchSymbol(prm1chr);
                            if(!symbol){
                                insertToSymbolTable(prm1chr, *newIC, line_counter, FALSE, FALSE, FALSE);
                            }
                        }

                        tempString = myStrtok(NULL, "\n");

                        if (!tempString) 
                            addToErrorList("There is no second operator for the 'jump' type command", line_counter, ERROR);
                        else if (!strchr(tempString,')')) 
                            addToErrorList("The sign ')' is after the second operand", line_counter, ERROR);
                        else {
                            tempString = myStrtok(tempString, ")");
                            if (isHasSpace(tempString)==TRUE)
                                addToErrorList("Space in parameter 2 section of the 'jump' type command",line_counter, ERROR);
                            else{
                                if ((prm2 = checkIfIsImmediate(tempString, line_counter)) < MAX_NUM_IMMEDIATE){
                                    if (tempString[0]!='#'){
                                        addToErrorList("An immediate number must be written with a # sign",line_counter, ERROR);
                                    }
                                    else {
                                        prm2flag = IMMEDIATE_JMP;
                                    }
                                }
                                else if ((prm2 = checkIfIsRegister(tempString)) != FALSE){
                                    prm2flag = REGISTER_JMP;
                                    if (prm1flag==REGISTER_JMP){
                                        (*newIC) -= 1;
                                    }
                                }
                                else if ((prm2 = isLabel(tempString, line_counter, USAGE)) != FALSE){
                                    prm2flag = LABEL_JMP;
                                    prm2chr = tempString;
                                    symbol = SearchSymbol(prm2chr);

                                    if(!symbol){
                                        insertToSymbolTable(prm2chr, *newIC, line_counter, FALSE, FALSE, FALSE);
                                    }
                                }

                                tempString = myStrtok(NULL, "\n");
                                if ((deleteSpaces(&tempString, FIRST_DELETE)) != EMPTY_SPACE){
                                    addToErrorList("Extrenaous content after the 'jump' command type", line_counter, ERROR);
                                }
                            }
                        }
                    }
                }
                else {
                    prm1flag = NIL_JMP;
                    prm2flag = NIL_JMP;
                }

               addFirstPassJump(commandNumber, operandDest, returnDestValue, prm1, prm1flag, prm1chr, prm2, prm2flag, prm2chr, line_counter, ICfirst);
            }
        }
    }
    else{
        if (commandNumber == STOP_COMMAND || commandNumber == RTS_COMMAND){
            operandSrc = myStrtok(NULL, "\n");
            bothFlags = NO_OPERANDS;
            (*newIC) += 1;

            if (deleteSpaces(&operandSrc, NORMAL_DELETE) !=EMPTY_SPACE){
                addToErrorList("Foreign content without operand commands", line_counter, ERROR);
                currentFlag = FALSE;
            }
        }
        else if (commandNumber == NOT_COMMAND || commandNumber == CLR_COMMAND || commandNumber == INC_COMMAND ||
                 commandNumber == DEC_COMMAND || commandNumber == RED_COMMAND || commandNumber == PRN_COMMAND) {
            operandSrc = myStrtok(NULL, "\n");
	        bothFlags = ONE_OPERAND;
            flagRemove = deleteSpaces(&operandSrc, NORMAL_DELETE);

            if(flagRemove != REMOVED_SPACES ) {
                currentFlag = FALSE;

            if (flagRemove==EMPTY_SPACE)
                addToErrorList("No destination operand detected for", line_counter, ERROR);
            else
                addToErrorList("Extrenaous content detected after one operand", line_counter, ERROR);
            }
        }

        if (currentFlag == TRUE && bothFlags != NO_OPERANDS){
            if (bothFlags == TWO_OPERANDS){
                operandSrc = myStrtok(NULL, ",");
                if ((flagRemove= deleteSpaces(&operandSrc, NORMAL_DELETE)) != REMOVED_SPACES){
                    currentFlag = FALSE;
                    if (flagRemove == EXTRA_CONTENT){
                        addToErrorList("No comma was detected after the first operand of the command", line_counter, ERROR);
                    }
                    else {
                        addToErrorList("A comma was detected before the first operand of the command", line_counter, ERROR);
                    }
                }
            }

            if (currentFlag == TRUE){
                if ((returnSrcValue = checkIfIsRegister(operandSrc)) != FALSE){
                    srcFlag = REGISTER_ADDRESING;
                }
                else if ((returnSrcValue = checkIfIsImmediate(operandSrc, line_counter)) < MAX_NUM_IMMEDIATE) {
                    if (operandSrc[0]!='#'){
                        addToErrorList("An immediate number must be written with a # sign", line_counter, ERROR);
                        currentFlag = FALSE;
                    }
                    else {
                        srcFlag = IMMEDIATE_ADDRESING;
                    }
                }
                else if (isLabel(operandSrc, line_counter, USAGE) != FALSE){
                    srcFlag = DIRECT_ADDRESING;
                    symbol = SearchSymbol(operandSrc);

                    if (!symbol){
                        insertToSymbolTable(operandSrc, *newIC, line_counter, FALSE, FALSE, FALSE);
                    }
                }
                else {
                    addToErrorList("Unrecognized operand", line_counter, ERROR);
                    currentFlag = FALSE;
                }

                if (currentFlag == TRUE && commandNumber == LEA_COMMAND && srcFlag != DIRECT_ADDRESING){
                    addToErrorList("The source operand of the LEA command must be a label", line_counter, ERROR);
                    currentFlag = FALSE;
                }

                if (currentFlag == TRUE){
                    if (bothFlags == ONE_OPERAND){
                       if (srcFlag==IMMEDIATE_ADDRESING && commandNumber != PRN_COMMAND){
                           addToErrorList("An immediate address cannot be obtained for this command",
                                   line_counter, ERROR);
                           currentFlag = FALSE;
                       }
                       else {
                           operandDest = operandSrc;
                           DestFlag = srcFlag;
                           returnDestValue = returnSrcValue;

                           operandSrc = NULL;
                           srcFlag = NIL_ADDRESING;
                           returnSrcValue = FALSE;

                           (*newIC) += 2;
                       }
                    }
                    else {
                        operandDest = myStrtok(NULL, "\n");

                        if (strchr(operandDest,',')){
                            currentFlag=FALSE;
                            addToErrorList("Extra comma is found after second operand", line_counter, ERROR);
                        }
                        else if ((flagRemove= deleteSpaces(&operandDest, NORMAL_DELETE)) != REMOVED_SPACES){
                            currentFlag = FALSE;
                            if (flagRemove == EXTRA_CONTENT){
                                addToErrorList("External content after second operand of command", line_counter, ERROR);
                            }
                            else{
                                addToErrorList("No second operand found", line_counter, ERROR);
                            }
                        }
                    }
                }
                if (currentFlag == TRUE && bothFlags == TWO_OPERANDS){
                    (*newIC) += 3;
                    if ((returnDestValue = checkIfIsRegister(operandDest)) != FALSE){
                        DestFlag = REGISTER_ADDRESING;
                        if (srcFlag == REGISTER_ADDRESING){
                            (*newIC)--;
                        }
                    }
                    else if ((returnDestValue = checkIfIsImmediate(operandDest, line_counter)) < MAX_NUM_IMMEDIATE) {
                        if (operandDest[0] != '#'){
                            addToErrorList("An immediate number must be written with a # sign", line_counter, ERROR);
                            currentFlag = FALSE;
                        }
                        else  if (commandNumber != CMP_COMMAND){
                            addToErrorList("The second operand of an instruction cannot be immediate", line_counter, ERROR);
                            currentFlag = FALSE;
                        }
                        else{
                            DestFlag = IMMEDIATE_ADDRESING;
                        }
                    }
                    else if (isLabel(operandDest, line_counter, USAGE) != FALSE){
                        DestFlag = DIRECT_ADDRESING;
                        symbol = SearchSymbol(operandDest);
                        if (!symbol){
                            insertToSymbolTable(operandDest, ICfirst, line_counter, FALSE, FALSE, FALSE);
                            returnDestValue = FALSE;
                        }
                        else {
                            returnDestValue = isCompleteSymbol(symbol);
                        }
                    }
                    else {
                        addToErrorList("The second operand of not a valid operand",line_counter, ERROR);
                        currentFlag = FALSE;
                    }
                }

                if (currentFlag == TRUE && commandNumber == LEA_COMMAND && DestFlag==IMMEDIATE_ADDRESING){
                    addToErrorList("The Destination Operand of the LEA instruction must be a label or a register", line_counter, ERROR);
                    currentFlag = FALSE;
                }
            }
        }

        if (currentFlag == TRUE){
            addCmdsFirstPass(commandNumber, operandSrc, srcFlag, returnSrcValue, operandDest, DestFlag, returnDestValue, line_counter, ICfirst);
        }
    }
}

int checkIfIsRegister(char * word){
    int k; 

    for(k=0;k<REGISTERS_NUM;k++)
        if (strcmp(word,REGISTERS_ARR[k])==0)
            return k;

    return FALSE;
}

int checkIfIsImmediate(char * mystr,int line_counter){
    char *endptr = NULL;
    int tmp_num; 

    if (mystr[0] == '#'){
        tmp_num = strtol(mystr+1, &endptr, BASE_NUM);
    }
    else{
        tmp_num = strtol(mystr, &endptr, BASE_NUM);
    }

    if (*endptr != '\0')
        tmp_num=MAX_NUM_IMMEDIATE + 1;
    else if ((tmp_num < MIN_NUM_IMMEDIATE) || (tmp_num > MAX_NUM_IMMEDIATE)){
        addToErrorList("Exceeded the allowed size", line_counter, ERROR);
        tmp_num=MAX_NUM_IMMEDIATE+1;
    }

    return tmp_num;
}

bool isHasSpace(char * strArr){
    int len = strlen(strArr);
    int k;

    for (k = 0;k < len; k++){
        if (isspace(strArr[k])){
            return TRUE;
        }
    }

    return FALSE;
}

int commasNumber(char* strArr){
    int k;
    int len;
    int counter = 0;
    len = strlen(strArr);

    for (k = 0; k < len; k++){
        if (strArr[k] == ','){
            counter++;
        }
    }

    return counter;
}
