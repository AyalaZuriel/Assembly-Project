/**
 * This file contains all the linked lists used on the project on the first and the second pass.
 * including: data list, error list, first pass list, instruction list, entry list and extern list.
 * each list has the functions to allocate memory, to insert node to the list,
*/

#include "globalDeclarations.h"
#include "lists.h"
#include "symbolTable.h"

        /***************************************DATA LIST***********************************/

data_ptr data_HEAD=NULL; /*head of data linked list*/
data_ptr data_TAIL=NULL; /*tail of data linked list*/

data_ptr DataNodeAllocate(int num,int addr)
{
    data_ptr node; /*node to allocate*/
    node=(data_ptr)malloc(sizeof(Data_node));
    if(!node){
        fprintf(stderr,"Error Creating a Data node!\n");
        exit(1);
    }
    node->address=addr;
    node->value=(unsigned)num;
    node->next=NULL;
    return node;
}

void insertToDataList(int number, int DC)
{
    data_ptr node; /*node to insert the list*/
    node=DataNodeAllocate(number,DC);

    if(data_HEAD==NULL){
        data_HEAD=node;
        data_TAIL=node;
    }
    else{
        data_TAIL->next=node;
        data_TAIL=node;
    }
}

void returnDataHead(data_ptr *node)
{
    *node=data_HEAD;
}

void UpdateDataAddress(int IC)
{
    data_ptr node; /*node to update the address for*/
    node=data_HEAD;
    while (node){
        node->address += IC;
        node=node->next;
    }
}

unsigned short getDataValue(data_ptr node)
{
    return (unsigned short)node->value;
}

data_ptr getNextData(data_ptr node)
{
    return node->next;
}

int getDataAddress(data_ptr node)
{
    return node->address;
}

void freeDataList()
{
    data_ptr tmp; /*temperary node to free*/
    while (data_HEAD){
        tmp=data_HEAD;
        data_HEAD=data_HEAD->next;
        free(tmp);
    }
}

        /*******************************ERROR LIST************************************/

error_node_ptr Error_Head=NULL; /*head of error linked list*/

char* ErrorType[2]={"ERROR:","WARNING:"}; /*string for type of error*/

void addToErrorList(char *text, int line_counter, errorTypes is_it_error){
    error_node_ptr new1,new2; /*new1 is the node to insert into the error list, node2 is a temporary node*/
    new1=AllocateNewErrorNode();

    new1->type=is_it_error;
    strcpy(new1->errorTxt,text);
    new1->lineNumber=line_counter;
    new1->next=NULL;

    if(Error_Head==NULL){ /*if the list is empty, the node becomes the head*/
        Error_Head=new1;
    }
    else{ /*the list is not empty, move the error to its positin determined by ascending line number*/
        new2=Error_Head;
        if (new2->lineNumber>=line_counter){ /*if the head's line number is smaller than the current node - the current node is to be the head*/
            new1->next=new2;
            Error_Head=new1;
        }
        else{ /*the node needs to be inserted to the list*/
            if (new2->next && new2->lineNumber < line_counter){ /*iterate as long there are nodes in the list and the current one's line number is greater then the given node*/
                while (new2->next && new2->next->lineNumber < line_counter) {
                    new2 = new2->next;
                }
            }
            if (new2->next){/*if the scan was terminated becuase the line number of the current node is greater than the number of the given node*/
                new1->next = new2->next;
                new2->next = new1;
            }
            else{/*the node is the last in the list*/
                new2->next = new1;
                }
        }
    }
}
error_node_ptr AllocateNewErrorNode()
{
    error_node_ptr new; /*new node to allocate*/
    new =(error_node_ptr)malloc(sizeof(struct errorNodeStruct));
    if(!new){
        printf("There is no more room in the error list\n");
        exit(1);
    }
    return new;
}

void errorListPrinting(char *fp)
{
    error_node_ptr new; /*temporary node*/
    new=Error_Head;
    if (new){
        fprintf(stderr,"Errors for the file %s.as:\n\n",fp);
        while(new){
            fprintf(stderr,"%s %s, line: %d\n",ErrorType[new->type],new->errorTxt,new->lineNumber);
            new=new->next;
        }
        printf("\n");
    }
}

errorTypes checkIfHasError()
{
     errorTypes flag=NOTHING; /*flag if there is an error*/
     error_node_ptr tmp; /*temporary node*/
     tmp=Error_Head;
    while(tmp){
        if (flag!=ERROR){
            flag=tmp->type;
        }

        tmp=tmp->next;
    }
    return flag;
}

void freeErrorList()
{
    error_node_ptr new; /*temporary node*/
    while (Error_Head){
        new=Error_Head;
        Error_Head=Error_Head->next;
        free(new);
    }
}


        /*******************************INSTRUCTION LIST*****************************/

instructionsListPtr word_HEAD=NULL; /*head of encoded word linked list*/
instructionsListPtr word_TAIL=NULL; /*tail of encoded word linked list*/

void addJumpCmds(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag, char *param1_char, int param2, int param2_flag, char *param2_char, int LineNum, int IC)
{
    instructionsListPtr First_Word = NULL, Second_Word = NULL, Third_Word = NULL, Forth_Word = NULL; /*words to create*/

    First_Word = allocateNewInstuctionNode(IC++);
    First_Word->type=FIRST_WORD;

    First_Word->current_word.FIRST.ERA = ABSOLUTE_ENCODING;
    First_Word->current_word.FIRST.DestAddressing = JUMP_ADDRESING;
    First_Word->current_word.FIRST.SourceAddressing = IMMEDIATE_ADDRESING;                                                                              
    First_Word->current_word.FIRST.Opcode=(unsigned)commandNum;
    Second_Word = allocateNewInstuctionNode(IC++);

    wordSetting(NIL_ADDRESING, LABEL_JMP, Second_Word, returnValueDest, IC, DestOperand);

    if(param1_flag==NIL_JMP){
        First_Word->current_word.FIRST.Parameter1 =0;
        First_Word->current_word.FIRST.Parameter2 =0;
    }
    else{
        First_Word->current_word.FIRST.Parameter1 = (unsigned)param1_flag;
        First_Word->current_word.FIRST.Parameter2 = (unsigned)param2_flag;

        Third_Word= allocateNewInstuctionNode(IC++); 

        if(param1_flag==REGISTER_JMP && param2_flag==REGISTER_JMP){
                registerWordSetting(Third_Word,SOURCE_OPERAND,  param1);
                Third_Word->current_word.reg.DestRegister=(unsigned)param2;
            }
        else{
            if (param1==REGISTER_JMP){
                registerWordSetting(Third_Word,DEST_OPERAND,  param1);
                Third_Word->current_word.reg.SourceRegister=0;
            }
            else{
                wordSetting(NIL_ADDRESING, param1_flag,Third_Word, param1, IC, param1_char);
            }

            Forth_Word= allocateNewInstuctionNode(IC++);

            if (param2_flag==REGISTER_JMP){
                registerWordSetting(Forth_Word,DEST_OPERAND,  param2);
                Forth_Word->current_word.reg.SourceRegister=0;
            }
            else{
                wordSetting(NIL_ADDRESING, param2_flag,Forth_Word,  param2, IC, param2_char);
            }
        }
    }

    insertToWordsList(First_Word); 
    insertToWordsList(Second_Word);

    if (Third_Word){
        insertToWordsList(Third_Word);
    }

    if (Forth_Word){
        insertToWordsList(Forth_Word);
    }

}

void addCmdsToWordsList(int command, char *SourceOperand, int SourceFlag, int returnValueSource, char *DestOperand,int DestFlag, int returnValueDest, int LineNum, int IC)
{
    instructionsListPtr First_Word = NULL, Second_Word = NULL, Third_Word = NULL; /*words to create*/

    First_Word = allocateNewInstuctionNode(IC++);
    First_Word->type=FIRST_WORD;
    First_Word->current_word.FIRST.ERA = ABSOLUTE_ENCODING;
    First_Word->current_word.FIRST.Opcode=(unsigned)command;
    First_Word->current_word.FIRST.Parameter1 = 0;
    First_Word->current_word.FIRST.Parameter2 = 0;

    if (SourceFlag == NIL_ADDRESING){
        First_Word->current_word.FIRST.SourceAddressing = IMMEDIATE_ADDRESING;
        if (DestFlag == NIL_ADDRESING){
            First_Word->current_word.FIRST.DestAddressing = IMMEDIATE_ADDRESING;
        }
        else{
            First_Word->current_word.FIRST.DestAddressing = (unsigned) DestFlag;
            Second_Word = allocateNewInstuctionNode(IC++); 
            if (DestFlag==REGISTER_ADDRESING){
                registerWordSetting(Second_Word,DEST_OPERAND, DestFlag);
                Second_Word->current_word.reg.SourceRegister=0;
            }
            else{
                wordSetting(DestFlag, NIL_JMP,Second_Word, returnValueDest, IC, DestOperand);
            }
        }
    }

    else{
        First_Word->current_word.FIRST.SourceAddressing = (unsigned) SourceFlag;
        First_Word->current_word.FIRST.DestAddressing=(unsigned)DestFlag;
        Second_Word = allocateNewInstuctionNode(IC++);

        if (SourceFlag==REGISTER_ADDRESING && DestFlag== REGISTER_ADDRESING){
            registerWordSetting(Second_Word,SOURCE_OPERAND, returnValueSource);
            Second_Word->current_word.reg.DestRegister=(unsigned)returnValueDest;
        }

        else{
            if (SourceFlag==REGISTER_ADDRESING){
                registerWordSetting(Second_Word,SOURCE_OPERAND, returnValueSource);
                Second_Word->current_word.reg.DestRegister=0;
            }
            else{
                wordSetting(SourceFlag, NIL_JMP,Second_Word, returnValueSource, IC, SourceOperand);
            }

            Third_Word= allocateNewInstuctionNode(IC++);

            if(DestFlag== REGISTER_ADDRESING){
                registerWordSetting(Third_Word,DEST_OPERAND, returnValueDest);
                Third_Word->current_word.reg.SourceRegister=IMMEDIATE_ADDRESING;
            }
            else{
                wordSetting(DestFlag, NIL_JMP,Third_Word, returnValueDest, IC, DestOperand);
            }
        }
    }

   insertToWordsList(First_Word);
    if(Second_Word){
        insertToWordsList(Second_Word);
    }

    if (Third_Word){
        insertToWordsList(Third_Word);
    }
}

instructionsListPtr allocateNewInstuctionNode(int address)
{
    instructionsListPtr node; /*node to allocate*/
    node = (instructionsListPtr) malloc(sizeof(Instruction_node));
    if (!node){
        fprintf(stderr,"Error Creating a Wordnode!\n");
        exit(1);
    }
    node->address = address;
    node->next=NULL;
    return node;
}

void insertToWordsList(instructionsListPtr node)
{
    if(word_HEAD==NULL){
        word_HEAD=node;
        word_TAIL=node;
    }
    else{
        word_TAIL->next=node;
        word_TAIL=node;
    }
}

void wordSetting(int Command_Operand_Flag,int Jump_Operand_flag, instructionsListPtr Current, int Value, int IC, char *str)
{
    if (Command_Operand_Flag==IMMEDIATE_ADDRESING || Jump_Operand_flag==IMMEDIATE_JMP){
        Current->type=IMMEDIATE_WORD;
        Current->current_word.FIRST.ERA = ABSOLUTE_ENCODING;
        Current->current_word.immediate.Immidiate=(unsigned)Value;
    }

    else if ( Command_Operand_Flag==DIRECT_ADDRESING || Jump_Operand_flag==LABEL_JMP){
        ptrOfSymbol symbol;/*a pointer to search the symbol in the symbols table*/
        Current->type = IMMEDIATE_WORD;
        symbol = SearchSymbol(str); /*getting the symbol*/
        Current->current_word.immediate.Immidiate = (unsigned) symbolAddress(symbol);

        if (isExternalSymbol(symbol) == TRUE){
            Current->current_word.immediate.ERA = EXTERNAL_ENCODING;
            addToExternList(str, IC - 1);
	}
        else{
            Current->current_word.immediate.ERA = RELOCATABLE_ENCODING;
        }
    }
}

void registerWordSetting( instructionsListPtr current_ptr,int type_of_operand, int current_value)
{
    current_ptr->type=REGISTER_WORD;
    current_ptr->current_word.reg.ERA=IMMEDIATE_ADDRESING;

    if (type_of_operand==SOURCE_OPERAND){
        current_ptr->current_word.reg.SourceRegister=(unsigned)current_value;
    }
    else{
        current_ptr->current_word.reg.DestRegister=(unsigned)current_value;
    }
}

unsigned short * getBinaryValueOfWord(instructionsListPtr new)
{
    unsigned short * word_binary_value=0; /*the binary word to get*/
    if (new->type==IMMEDIATE_WORD ){
        word_binary_value=(unsigned short *)(&(new->current_word.immediate));
    }
    else if(new->type==REGISTER_WORD){
        word_binary_value=(unsigned short *)(&(new->current_word.reg));
    }
    else if(new->type==FIRST_WORD){
        word_binary_value=(unsigned short *)(&(new->current_word.FIRST));
    }
    return word_binary_value;
}

int getInstructionAddress(instructionsListPtr new)
{
    return new->address;
}
void returnInstructionsHeadNode(instructionsListPtr *new)
{
    *new=word_HEAD;
}

instructionsListPtr getNextInstruction(instructionsListPtr new)
{
    return new->next;
}

void freeInstructionslist()
{
    instructionsListPtr new;
    while (word_HEAD){
        new=word_HEAD;
        word_HEAD=word_HEAD->next;
        free(new);
    }
}


        /*******************************FIRST PASS LIST********************************/

firstPassListPtr first_pass_HEAD=NULL; /*head of first pass linked list*/
firstPassListPtr first_pass_TAIL=NULL; /*tail of first pass linked list*/

firstPassListPtr First_pass_NodeAllocate()
{
    firstPassListPtr new;/*node to allocate*/
    new=(firstPassListPtr)malloc(sizeof(numberOfCmdNode));
    if(!new){
        printf("An error occurred creating the data node\n");
        exit(1);
    }
    new->next=NULL;
    return new;
}

void first_pass_node_insert(firstPassListPtr new)
{
    if(first_pass_HEAD==NULL){
        first_pass_HEAD=new;
        first_pass_TAIL=new;
    }
    else{
        first_pass_TAIL->next=new;
        first_pass_TAIL=new;
    }
}

void  addFirstPassJump(int cmd_num, char *operand_of_destination, int destination_return_value, int prm1, int flag_of_prm1, char *chr_of_prm1, int prm2, int flag_of_prm2, char *chr_of_prm2, int ine_counter, int IC)
{
    firstPassListPtr new;/*nose to insert*/
    new=First_pass_NodeAllocate();

    new->isJump=TRUE;

    new->current.jump_command.commandNumber=cmd_num;
    strcpy(new->current.jump_command.DestOperand,operand_of_destination);
    new->current.jump_command.returnValueDest=destination_return_value;
    new->current.jump_command.param1=prm1;
    new->current.jump_command.param1_flag=flag_of_prm1;

    new->current.jump_command.param2=prm2;
    new->current.jump_command.param2_flag=flag_of_prm2;
    new->current.jump_command.LineNumber=ine_counter;
    new->current.jump_command.IC=IC;

    if (flag_of_prm1==LABEL_JMP){
        strcpy(new->current.jump_command.param1_char,chr_of_prm1);
    }

    if (flag_of_prm2==LABEL_JMP){
        strcpy(new->current.jump_command.param2_char,chr_of_prm2);
    }
    first_pass_node_insert(new);
}

void addCmdsFirstPass(int cmd_num, char *operand_of_source, int flag_of_source, int source_return_value, char *operand_of_destination, int flag_of_destination, int destination_return_value, int line_counter, int IC)
{
    firstPassListPtr new;/*node to insert*/
    new=First_pass_NodeAllocate();

    new->isJump=FALSE; 
    new->current.command.commandNumber=cmd_num;
    new->current.command.SourceFlag=flag_of_source;
    new->current.command.returnValueSource=source_return_value;

    if (flag_of_source==DIRECT_ADDRESING){
        strcpy(new->current.command.SourceOperand,operand_of_source);
    }

    new->current.command.DestFlag=flag_of_destination;
    new->current.command.returnValueDest=destination_return_value;

    if (flag_of_destination==DIRECT_ADDRESING){
        strcpy(new->current.command.DestOperand,operand_of_destination);
    }

    new->current.command.LineNumber=line_counter;
    new->current.command.IC=IC;

    first_pass_node_insert(new);
}

void addFirstpassToInstructions(firstPassListPtr new)
{
    if (new->isJump==TRUE){

        int num_of_cmd1=new->current.jump_command.commandNumber;
        char* the_destination_operand=new->current.jump_command.DestOperand;
        int destination_rtn_value=new->current.jump_command.returnValueDest;

        int prm1=new->current.jump_command.param1;
        int flag_of_prm1=new->current.jump_command.param1_flag;
        char *chr_of_prm1=new->current.jump_command.param1_char;

        int prm2=new->current.jump_command.param2;
        int flag_of_prm2=new->current.jump_command.param2_flag;
        char *chr_of_prm2=new->current.jump_command.param2_char;

        int line_counter=new->current.jump_command.LineNumber;
        int IC=new->current.jump_command.IC;

        addJumpCmds(num_of_cmd1, the_destination_operand,destination_rtn_value,prm1, flag_of_prm1, chr_of_prm1, prm2, flag_of_prm2, chr_of_prm2, line_counter, IC);
    }
    else{
	int cmd_num=new->current.command.commandNumber;

        char* operand_of_src=new->current.command.SourceOperand;
        int flag_of_src=new->current.command.SourceFlag;
        int src_rtn_values=new->current.command.returnValueSource;

        char* operand_of_dest=new->current.command.DestOperand;
        int flag_of_dest=new->current.command.DestFlag;
        int dest_rtn_values=new->current.command. returnValueDest;

        int line_counter=new->current.command.LineNumber;
        int IC=new->current.command.IC;

        addCmdsToWordsList(cmd_num, operand_of_src, flag_of_src, src_rtn_values, operand_of_dest, flag_of_dest, dest_rtn_values, line_counter, IC);
    }
}

void firstPassListMoveToWordsList()
{
    firstPassListPtr tmp;
    tmp=first_pass_HEAD;
    while(tmp){
        addFirstpassToInstructions(tmp);
        tmp=tmp->next;
    }
}

void firstPasslistFree()
{
    firstPassListPtr tmp; /*temporary node*/
    while (first_pass_HEAD){
        tmp=first_pass_HEAD;
        first_pass_HEAD=first_pass_HEAD->next;
        free(tmp);
    }
}


        /****************************ENTRY LIST********************************/

entry_node_ptr Entry_head=NULL; /*head of internals table linked list*/

entry_node_ptr EntryNodeAllocate(char* node_symbol,int node_address)
{
    entry_node_ptr new;/*node to allocate*/
    new=(entry_node_ptr)malloc(sizeof(Entry_node));/*allocate space for the node */
    if(!new){
        fprintf(stderr,"The requested data node could not be created\n");
        exit(1);
    }
    new->address=node_address;
    strcpy(new->symbol,node_symbol);
    new->next=NULL;

    return new;
}

void addNewToEntryList(char *node_symbol, int node_address, int line_counter)
{
    bool flag=TRUE;/*flag if there is error*/
    entry_node_ptr current_node, tmp_node; /*current word to insert and temporary node*/
    current_node=EntryNodeAllocate(node_symbol,node_address);

    if(Entry_head==NULL){ /*The list is empty- the entry is the new head*/
        current_node->next=NULL;
        Entry_head=current_node;
    }
    else{
        tmp_node=Entry_head;
        if (tmp_node->next==NULL && strcmp(tmp_node->symbol,node_symbol)==0 ){ /*if there is only one node and it is has the same symbol as the current node*/
            flag=FALSE;
            addToErrorList("Unable to set Entry again",line_counter,ERROR);
        }
        else { /*There are more than one node in the list and the head symbol is not the same as the current one*/
            while (flag==TRUE && tmp_node->next!=NULL){
                if (strcmp(tmp_node->symbol,node_symbol)==0){ /*The symbol is found in the list*/
                    flag=FALSE;
                    addToErrorList("Unable to set Entry again",line_counter,ERROR);
                    free(current_node);
                }
                tmp_node=tmp_node->next; 
            }
        }
        if (flag==TRUE){ /*the end of the list was reached without errors*/
            current_node->next=NULL;
            tmp_node->next=current_node;
        }
    }
}

void returnEntryHeadNode(entry_node_ptr *new)
{
    *new=Entry_head;
}

void UpdateEntryList()
{
    ptrOfSymbol s_temp; /*sympol temporary node*/
    entry_node_ptr e_temp; /*entry temporary node*/
    e_temp=Entry_head;
    while (e_temp){
        s_temp=SearchSymbol(e_temp->symbol);
        e_temp->address=symbolAddress(s_temp);
        e_temp=e_temp->next;
    }
}

char* getSymbolEntry(entry_node_ptr new)
{
    return new->symbol;
}

int getTheEntryAddress(entry_node_ptr new)
{
    return new->address;
}

entry_node_ptr getNextNodeEntry(entry_node_ptr new_node)
{
    return new_node->next;
}

void entryListFree()
{
    entry_node_ptr new; /*temporary node*/
    while (Entry_head){
        new=Entry_head;
        Entry_head=Entry_head->next;
        free(new);
    }
}


        /****************************EXTERN LIST********************************/

extern_node_ptr Extern_HEAD=NULL; /*head of extenals table linked list*/
extern_node_ptr Extern_TAIL=NULL; /*tail of extenals table linked list*/

extern_node_ptr ExternNodeAllocate(char* label,int the_address)
{
    extern_node_ptr new;
    new=(extern_node_ptr)malloc(sizeof(ExternNode));
    if(!new){
        fprintf(stderr,"An external node cannot be created\n");
        exit(1);
    }
    new->address=the_address;
    strcpy(new->symbol,label);
    new->next=NULL;
    return new;
}

void addToExternList(char* label, int the_addres){
    extern_node_ptr new;
    new=ExternNodeAllocate(label,the_addres);

    if(Extern_HEAD==NULL){
        Extern_HEAD=new;
        Extern_TAIL=new;
    }
    else{
        Extern_TAIL->next=new;
        Extern_TAIL=new;
    }
}

void returnExternNodeHead(extern_node_ptr *new){
    *new=Extern_HEAD;
}

char* getTheExternSymbol(extern_node_ptr new){
    return new->symbol;
}

int getExternNodeAddress(extern_node_ptr new){
    return new->address;
}

extern_node_ptr getNextNodeExtern(extern_node_ptr new){
    return new->next;
}

void externListFree(){
    extern_node_ptr new;
    while (Extern_HEAD){
        new=Extern_HEAD;
        Extern_HEAD=Extern_HEAD->next;
        free(new);
    }
}

