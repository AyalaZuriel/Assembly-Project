/*
 * This is the main of the entire program for starting it.
 * the main method accepts command line arguments specifying the names of the assembly files to be encoded.
 *
 * This is the first analysis of the input, it checks if the program was given file names, and then iteratively
 * tries to open each file via the my_open method, defined in the fileFunctions.c file.
 *
 * If the files are valid, they are passed to the preAssembler() method found in the preAssembler.c for the pre processing.
 * */

#include "fileFunctions.h"
#include "globalDeclarations.h"
#include "assembler.h"
#include "preAssembler.h"

int main(int argc, char * argv[]){
	int i = 0; 	/*index for argv array*/
	FILE * ifp;	/*the file with .as extension */
	FILE * dfp;	/*the file with .am extension, after the macros parse*/
	if(argc<2){
		fprintf(stderr,"Error: No files\n");
		return 1;
	}
	while(argv[++i]!=NULL){
		ifp = my_open(argv[i], "r",as_EXTENTION);
		
		if(ifp && dfp){
			if(preAssembler(ifp, argv[i])==FALSE){
				fprintf(stderr,"Error: Pre-processor failed\n");
				continue;
			}

			dfp = my_open(argv[i], "r",am_EXTENTION);	
			
			if(AssemblerActivate(dfp, argv[i])==TRUE){
			       printf("Assembly success for file: \"%s\"\n",argv[i]);
			}
			else{
			       printf("Assembly failure for file: \"%s\"\n",argv[i]);
			}
			fclose(ifp);
			fclose(dfp);
		}	
	}
	printf("Program end.\n");
	return 0;
}
