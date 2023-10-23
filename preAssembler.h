/**
 * preAssembler.h:
 * ---------------------------------
 * This is the header file for the preAssembler.c file, it contain method declaration and definition and data strucure used on preAssembler.c
 * 
*/

typedef struct node_m{ /*structure to mcrs*/
	char* name; /*the mcr name*/
	char* content; /*the mcr content*/
	struct node_m* next;
}mcrNode;

/**
 * preprocessor method to parse the mcrs in the file.
 * @param sfp - the source file pointer
 * @param file_name - the name of the file wothout extension
 * @return TRUE if the process succeed, otherwise returns FALSE
*/
int preAssembler(FILE *sfp,char *file_name);

/**
 * method to add a node to the mcr linked list.
 * @param head - the head of the mcr linked list
 * @param tail - the tail of the mcr linked list
 * @param name - the name of the mcr to add
*/
void addNodeMcr(mcrNode **head, mcrNode **tail, char *name);

/**
 * method to insert content to a node in the mcr linked list.
 * @param head - the head of the mcr linked list
 * @param tail - the tail of the mcr linked list
 * @param string - the content of the mcr to insert
*/
void enterMcrContent(char *string, mcrNode **head, mcrNode **tail);

/**
 * method to check if an input string contains only one word
 * @param string the string to check in
 * @return TRUE if the string contains one word, otherwise FALSE.
*/
int isOneWord(char *string);

/**
 * method to check if an input word is a legal mcr name
 * @param word the word to check
 * @param head the head of the mcr linked list
 * @return TRUE if the word is a legal mcr name, otherwise FALSE
*/
int isMcr(char *word, mcrNode *head);

/**
 * method that get a full line command and take the mcr name from it
 * @param string the full line command to take from
 * @return the mcr name, NULL if did not success 
*/
char* takeMcrName(char *string);

/**
 * method that checks if a given mcr name already exist on the mcr linked list
 * @param name the mcr name to check about
 * @param head the head of the mcr linked list
 * @return TRUE if the name does not exist on the mcr linked list, otherwise FALSE
*/
int checkMcrExist(char *name, mcrNode *head);

/**
 * method to write to content of the mcr to the file
 * @param string the mcr name that found in the file
 * @param head the head of the mcr linked list
 * @param dfp the file to write to
 * @return TRUE if succeed to write the mcr content, otherwise FALSE
*/
int writeMcrContent(char *string, mcrNode *head, FILE *dfp);

/**
 * Function to free all the mcr linked list
 * @param head the head of the mcr linked list
*/
void freeList(mcrNode *head);
