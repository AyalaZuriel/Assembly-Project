/**
 * preAssembler.c
 * ------------------
 * A file containing function for the preprocessor phase of a given file
 *
*/

#include "globalDeclarations.h"
#include "preAssembler.h"
#include "fileFunctions.h"
#include "firstPass.h"

int preAssembler(FILE *sfp, char *file_name){
	FILE *dfp; /*the file after the macro parsing with ".am" extension*/
	mcrNode *head = NULL, *tail = NULL;	/*head and tail of the mcr linked list*/	
	char* command, *command_copy, *mcr_name; /*the full line command and copy of it. the mcr name*/
	int isInside = 0, mcrInFile = 0; /*flags to check if inside macro and if there is mcr in file respectively*/

	dfp = my_open(file_name, "w",am_EXTENTION);
	command = (char*)malloc(sizeof(char)*MAX_LINE_SIZE+1);
	if(command==NULL)
		return FALSE;

	command_copy = (char *)malloc(sizeof(char)*MAX_LINE_SIZE+1);
	if(command_copy==NULL){
		free(command);
		return FALSE;
	}

	while(fgets(command, MAX_LINE_SIZE, sfp)!=NULL){	
		strcpy(command_copy, command);
		if(isInside){/*if inside mcr add this line to mcr content*/
			if(isOneWord(command)==TRUE && strstr(command, "endmcr")){
				isInside = 0;
				continue;
			}
			enterMcrContent(command, &head, &tail);
			continue;
		}
		else if(strstr(command, "mcr")==NULL){/*line without "mcr"*/
			if(isOneWord(command)==TRUE)
				if(mcrInFile)
					if(writeMcrContent(command, head, dfp)==TRUE)/*if this is ncr name write and go to next line*/
						continue;
			fprintf(dfp, "%s", command_copy);
			continue;
		}
		else{   				
			if(isMcr(command_copy, head)==TRUE){/*if new mcr was found*/
				mcrInFile = 1;
				isInside = 1;
				if((mcr_name = takeMcrName(command))==NULL){
					break;
				}
				addNodeMcr(&head, &tail, mcr_name);
				continue;
			}
			else {
				fprintf(dfp, "%s", command_copy);
				continue;
			}
		}
	}
	free(command);
	free(command_copy);

	if(mcrInFile){
		freeList(head);
	}
	fclose(dfp);

	return TRUE;
}

void addNodeMcr(mcrNode** head, mcrNode** tail, char* name){
	mcrNode *new = (mcrNode*)malloc(sizeof(mcrNode)); /*the new node*/
	if(new==NULL)
		return;
	new->name = name;
	new->content = NULL;
	new->next = NULL;

	if(*head==NULL){/*if this is first node*/
		*head = new;
	}		
	else if((*head)->next == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = (*head)->next;
	}
	else {	
		(*tail)->next = new;
		*tail = new;
	}
}

void enterMcrContent(char* string, mcrNode** head, mcrNode** tail){
	mcrNode* ptr; /*pointer to the mcr linked list*/
	if(*tail==NULL){
		ptr = *head;
	}
	else ptr = *tail;
	if(((ptr)->content) == NULL) {
		(ptr)->content = (char*)malloc(strlen(string)+1);
		if(((ptr)->content)==NULL){
			return;
		}
		strcpy((ptr)->content, string);
	}
	else {
		(ptr)->content = (char*)realloc((ptr)->content, strlen((ptr)->content)+1+strlen(string)+1);
		if(((ptr)->content)==NULL){
			return;
		}
		strcat((ptr)->content, string);
	}
}

int isOneWord(char* string){
	char *copy;	/*the copy of the string*/
	char *white_space = " \t\v\f\r\n";/*all the possible white spaces*/
	copy = (char*)malloc(strlen(string)+1);
	if(copy == NULL){
		exit(0);
	}
	strcpy(copy, string);
	if(strtok(copy, white_space) == NULL){/*empy*/
		free(copy);
		return FALSE;
	}
	if(strtok(NULL, white_space) == NULL){/*one word line*/
		free(copy);		
		return TRUE;
	}
	free(copy);
	return FALSE;	/*more than one word*/
}

int isMcr(char* string, mcrNode* head)
{
	char *white_space = " \t\v\f\r";/*all the possible white spaces*/
	char *p, *copy; /*p is result of strtok function, copy is the copy of the string*/
	int i;

	copy = removeBlanks(string);

	for(i=0; i<strlen(copy); i++){/*if contains illegal punctuation mark in line*/
		if(ispunct(copy[i])){
			free(copy);
			return FALSE;
		}
	}

	p = strtok(copy, white_space);
	if(strcmp(p, "mcr")){/*if first word is not "mcr"*/
		free(copy);
		return FALSE;
	}
	if((p = strtok(NULL, white_space))==NULL){
		free(copy);
		return FALSE;
	}

	if((isDirective(p)!=FALSE) || (isCommand(p)!=FALSE)){
		free(copy);
		return FALSE;
	}

	if(checkMcrExist(p, head) == FALSE){
		free(copy);
		return FALSE;
	}

	if((p = strtok(NULL, white_space))!=NULL){
		free(copy);
		return FALSE;
	}

	free(copy);
	return TRUE;
}

char* takeMcrName(char* string){
	char* p, *new_line, *mcr_name;/*p is result of strtok function, new_line is the line after removeBlanks function, mcr_name is the name of the mcr*/
	char *white_space = " \t\v\f\r"; /*all the possible white spaces*/

	if((new_line = removeBlanks(string))==NULL){
		return NULL;
	}
	mcr_name = (char*)malloc(strlen(new_line)+1);
	if(mcr_name == NULL){
		return NULL;
	}
	p = strtok(new_line, white_space);
	p = strtok(NULL, white_space);
	strcpy(mcr_name, p);
	free(new_line);
	return mcr_name;
}

int writeMcrContent(char* string, mcrNode* head, FILE *dfp){
	mcrNode* ptr = head; 	/*pointer to pass the nodes in the mcr linked list*/
	char *white_space = " \t\v\f\r"; 	/*all the posible white spaces*/
	char *copy; 	/*copy of the given string*/
	copy = removeBlanks(string);
	strtok(copy, white_space);

	while(ptr!=NULL){
		if(!strcmp(ptr->name, copy)) {
			fprintf(dfp, "%s", ptr->content);
			free(copy);
			return TRUE;
		}
		ptr = ptr->next;
	}
	free(copy);
	return FALSE;
}

int checkMcrExist(char* name, mcrNode* head){
	mcrNode* ptr = head;/*pointer to pass the nodes in the mcr linked list*/
	if(head==NULL){
		return TRUE;	
	}
	while(ptr!=NULL){
		if(!strcmp(ptr->name, name)){
			return FALSE;
		}
		ptr = ptr->next;
	}
	return TRUE;
}

void freeList(mcrNode* head){
	mcrNode* ptr;/*pointer to pass the nodes in the mcr linked list*/
	while(head!=NULL){
		ptr = head;
		head = head->next;
		free(ptr->name);
		free(ptr->content);
		free(ptr);
	}
}
