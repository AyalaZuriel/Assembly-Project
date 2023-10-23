/**
 * firstPass.c
 * ------------------
 * A file containing several function for the "firstPass" of a given file
 *
*/

#include "firstPass.h"
#include "globalDeclarations.h"
#include "lists.h"
#include "symbolTable.h"
#include "firstPassAssist.h"

#define isCommentLine(x) (((x)[0])== ';') /*macro to check if a line is a comment line (starts with ';')*/
#define NUM_OF_COMMANDS 16 /*number of commands possible*/
#define NUM_OF_DIRECTIVES 4  /*number of directives possible*/

char* COMMAND_STRING[NUM_OF_COMMANDS]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"}; /*all the commands names*/
char* DIRECTIVES[NUM_OF_DIRECTIVES]={".data",".string",".entry",".extern"}; /*all the directives names*/

void firstPass(FILE *file, int *newIC, int *newDC)
{

    char line[MAX_LINE_SIZE]; /*a line in the given file*/
    char *currentWord; /*specipic word in a line in the given file*/
    char* tempChar; /*temporary label name saver*/
    char* tempLine; /*temporary line saver*/
    int line_counter = 0; /*count the line number*/
    int flag; /*represent the type of directive or command.*/
    int n; /*store the length of a line*/
    char ch; /*used to read and discard characters from the file*/
    ptrOfSymbol newTempSymble; /*a pointer to a symbol in the symbol table.*/

    while (fgets(line, MAX_LINE_SIZE+1, file)){
        line_counter++;
        tempLine=line;
        if (isCommentLine(line) || deleteSpaces(&tempLine, FIRST_DELETE) == EMPTY_SPACE){
            continue;
        }

        n=strlen(line);
        if(n==MAX_LINE_SIZE){
            addToErrorList("Line exceeded the length limit", line_counter, ERROR);
	    ch=(char)getc(file);
            while(ch!='\n'){
                ch=(char)getc(file);
            }
            continue;
        }

	currentWord = myStrtok(tempLine, " \t\n");

        if ((flag=isDirective(currentWord)) != FALSE){
            directiveManager(flag, line_counter, newDC, FALSE, newIC);
        }
	else if ((flag=isCommand(currentWord)) != FALSE){
            cmdManager(flag, line_counter, newIC);
        }
	else if (isLabel(currentWord, line_counter, DEFINITION) != FALSE){
            tempChar=currentWord;
            newTempSymble=SearchSymbol(tempChar);
            currentWord = myStrtok(NULL, "\n");
            if (deleteSpaces(&currentWord, FIRST_DELETE) == EMPTY_SPACE){
                addToErrorList("No input detected after label Definition", line_counter, ERROR);
                continue;
            }

            currentWord = myStrtok(currentWord, " \t");

            if((flag=isCommand(currentWord)) != FALSE){

                if (!newTempSymble){
                    insertToSymbolTable(tempChar, *newIC, line_counter, FALSE, TRUE, TRUE);
                }

                else{
                    if (isExternalSymbol(newTempSymble)==TRUE){
                        addToErrorList("Defining a label which is External", line_counter, ERROR);
                        continue;
                    }
                    SetSymbolAdress(newTempSymble, *newIC);
                    SetSymbolComplete(newTempSymble, TRUE);
                    SetSymbolOP(newTempSymble, TRUE);
                }

                cmdManager(flag, line_counter, newIC);
            }

            else if((flag=isDirective(currentWord)) != FALSE){

                if (!newTempSymble){
                    insertToSymbolTable(tempChar, *newDC, line_counter, FALSE, FALSE, TRUE);
                }
                else{
                    SetSymbolAdress(newTempSymble, *newDC);
                    SetSymbolComplete(newTempSymble,TRUE);
                }

                directiveManager(flag, line_counter, newDC, TRUE, newIC);
            }
            else{
                addToErrorList("Unrecognized second operator", line_counter, ERROR);
            }
        }
    }

    UpdateSymbols(*newIC);

    if ((newTempSymble=find_missing())){
        while(newTempSymble){
            addToErrorList("Undeclared Symbol", symbolLineNum(newTempSymble), ERROR);
            newTempSymble=find_missing();
        }
    }
    else{
        UpdateDataAddress(*newIC);
        UpdateEntryList();
    }
}

int isDirective(char * word){
    int k;
    for(k=0;k<NUM_OF_DIRECTIVES;k++)
        if (strcmp(word,DIRECTIVES[k])==0)
            return k;
    return FALSE;
}

int isCommand(char * word){
    int k;
    for(k=0;k<NUM_OF_COMMANDS;k++)
        if (strcmp(word,COMMAND_STRING[k])==0)
            return k; 
    return FALSE;
}

int isLabel(char *temp_string, int line_counter, labelType typeOfLabel){
	int k; /*index*/
	int len=strlen(temp_string);/*length of the label name*/
	int flag =TRUE; /*flag to return if the given string is label or not*/

	if(!isalpha(temp_string[0])){
	   	addToErrorList("Label definition without an opening letter", line_counter, ERROR);
		flag =FALSE;
	}
	else if(typeOfLabel==DEFINITION &&temp_string[len-1]!= ':'){
   		addToErrorList("Label without \":\" character", line_counter, ERROR);
		flag=FALSE;
	}
 	else if(typeOfLabel==DEFINITION){
		len--;
		temp_string[len]= '\0';
	}
	else if(typeOfLabel==USAGE && temp_string[len-1]== ':'){
		addToErrorList("Label usage must be without \":\" character", line_counter, ERROR);
		flag=FALSE;
	}
	else{
		if (len>MAX_SYMBOL_SIZE){
			addToErrorList("Label length exceeds the limit", line_counter, ERROR);
			flag=FALSE;
        	}
		else{
			k=1;
			while (flag==TRUE && k<len){
				if (!isalpha(temp_string[k]) && !isdigit(temp_string[k])){
					addToErrorList("A label must be comprised form letters and digits only", line_counter, ERROR);
					flag=FALSE;
				}
 				k++;
			}
		}
	}
	return flag;
}

char *myStrtok(char *word, char const *delimiters)
{
	static char  *soruce = NULL; /*source string*/
	char  *ptr,  *stringToReturn = 0; /*pointer to string to return*/
	if (word != NULL){
		soruce = word;
	}
	if (soruce == NULL || soruce[0]=='\n'){
		return NULL;
	}
	else if ((ptr = strpbrk (soruce, delimiters)) != NULL){
		*ptr  = 0; 
		stringToReturn = soruce;
		soruce = ++ptr;
	}
	else if (*soruce){
		stringToReturn = soruce;
		soruce = NULL;
	}
	return stringToReturn;
}

int deleteSpaces(char **word, int RemoveCase)
{
	char *ptr;/*a pointer to a character, used to iterate over the given string.*/
	int flag=REMOVED_SPACES/*an integer value representing the flag indicating the operation success. The possible values are*/; 

	if (*word==NULL){
		flag=EMPTY_SPACE;
	}
	else{
		ptr=*word;
		while((*ptr)!='\"' && isspace(*ptr)){
			ptr++;
		}
		if (ptr[0]=='\0'){
			flag=EMPTY_SPACE;
		}
		else{
			*word=ptr;
			if (RemoveCase!=FIRST_DELETE){
				while(!isspace(*ptr) && *ptr!='\0'){
					ptr++;
				}
				if (*ptr!='\0'){
					*ptr=0;
					ptr++;
					while(isspace(*ptr)){
						ptr++;
					}
					if (*ptr!='\0'){
						flag=EXTRA_CONTENT;
					}
 				}
			}
		}
	}
	return flag;
}

char* removeBlanks(char* word)
{
	int i, start=0, end=0/*s an integers used to keep track of the indexs of the first and lest non-whitespace character in the string.*/;
	size_t len = strlen(word);/*len is a size_t variable holding the length of the input string word.*/
	char *new_word;/* is a pointer to a character array that will hold the modified string.*/

	new_word = (char*)malloc(len+1);
	if(new_word==NULL)
		return NULL;

	for(i=0; i<len; i++){
		if(!isspace(word[i])){
			start=i;	
			break;
		}
	}
	for(i=len-1; i>=0; i--){
		if(!isspace(word[i])){
			end=i;	
			break;
		}
	}
	memcpy(new_word,&word[start],end-start+1);
	new_word[end-start+1] = '\0';
	return new_word;
}

